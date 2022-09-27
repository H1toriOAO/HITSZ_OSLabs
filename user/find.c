#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, char* exp)
{
	char buf[512];
  	int fd;
  	struct dirent de;
  	struct stat st;

  	if ((fd = open(path, 0)) < 0)
	{
  		fprintf(2, "find: cannot open %s\n", path);
  		return;
  	}

  	if (fstat(fd, &st) < 0)
	{
  		fprintf(2, "find: cannot stat %s\n", path);
  	  	close(fd);
  	  	return;
  	}

    if (st.type != T_DIR)
    {
        fprintf(2, "find: the first arg must be dir path\n");
        close(fd);
        return;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        strcpy(buf, path);
        char* p = buf + strlen(buf);
        *p++ = '/';

        if (de.inum == 0)
        {
            continue;
        }

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            printf("find: cannot stat %s\n", buf);
            continue;
        }

        switch(st.type) 
        {
            case T_FILE:
                if (strcmp(exp, de.name) == 0) 
                {
                    printf("%s\n", buf);
                }
                break;

            case T_DIR:
                if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) 
                {
                    find(buf, exp);
                }
                break;
        }
    }
    close(fd);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("usage: find <path> <experssion>\n");
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}
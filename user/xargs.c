#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(2, "usage: xargs <cmd> ...\n");
        exit(0);
    }

    while (1)
    {
        int i = 0, j = 0, first_blank = 0;
        char args[MAXARG][32];
        memset(args, 0, sizeof(args));

        for (j = 1; j < argc; j++)
        {
            strcpy(args[i++], argv[j]);
        }

        j = 0;
        while (i < MAXARG - 1)
        {
            char buf;
            if (read(0, &buf, 1) <= 0)
            {
                int status;
                wait(&status);
                exit(0);
            }

            if (buf == '\n')
            {
                break;
            }

            if (buf == ' ')
            {
                if (first_blank)
                {
                    i++;
                    j = 0;
                    first_blank = 0;
                }

                continue;
            }

            args[i][j++] = buf;
            first_blank = 1;
        }

        char* p[MAXARG];
        for (i = 0; i < MAXARG - 1; i++)
        {
            p[i] = args[i];
        }
        p[MAXARG - 1] = 0;

        if (fork() == 0)
        {
            exec(argv[1], p);
            exit(0);
        }
    }

    exit(0);
}
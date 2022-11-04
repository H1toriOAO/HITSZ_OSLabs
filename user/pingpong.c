#include "kernel/types.h"
#include "user.h"

int main(int argc, char* argv[])
{
    if (argc != 1) { // 校验参数数量
        printf("PINGPONG NEEDS NO ARGUMENTS!\n");
        exit(-1);
    }

    int p_father[2], p_child[2]; // 创建管道
    pipe(p_father);
    pipe(p_child);

    char msg[4];
    if (fork() == 0) // 子线程
    {
        if (read(p_father[0], msg, 4) != 4) {
            printf("UNABLE TO READ PING!\n");
            exit(-1);
        } else {
            printf("%d: received ping\n", getpid());
        }

        strcpy(msg, "pong");
        if (write(p_child[1], msg, 4) != 4) {
            printf("UNABLE TO WRITE PONG!\n");
            exit(-1);
        }

        exit(0);
    } else // 父线程
    {
        strcpy(msg, "ping");

        if (write(p_father[1], msg, 4) != 4) {
            printf("UNABLE TO WRITE PING!\n");
            exit(-1);
        }

        if (read(p_child[0], msg, 4) != 4) {
            printf("UNABLE TO READ PONG!\n");
            exit(-1);
        } else {
            printf("%d: received pong\n", getpid());
        }

        exit(0);
    }

    exit(0);
}
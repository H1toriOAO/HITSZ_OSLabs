#include "kernel/types.h"
#include "user.h"

int main(int argc, char* argv[]) {
    if (argc != 1) {        // 校验参数数量
        printf("PINGPONG NEEDS NO ARGUMENTS!\n");
        exit(-1);
    }

    int p[2];       // 创建管道
    pipe(p);

    int ret = fork();       // 创建子线程
    char msg[4];
    if (ret == 0) {         // 子线程
        int pid = getpid();
        int r_len = read(p[0], msg, 4);
        if (r_len != 4) {
            printf("UNABLE TO READ PING!\n");
            exit(-1);
        } else {
            printf("%d: reveived ping\n", pid);
        }
        sleep(5);

        strcpy(msg, "pong");
        int w_len = write(p[1], msg, 4);
        if (w_len != 4) {
            printf("UNABLE TO WRITE PONG!\n");
            exit(-1);
        }

        exit(0);
    } else {        // 父线程
        int pid = getpid();
        strcpy(msg, "ping");
        int w_len = write(p[1], msg, 4);
        if (w_len != 4) {
            printf("UNABLE TO WRITE PING!\n");
            exit(-1);
        }
        sleep(5);

        int r_len = read(p[0], msg, 4);
        if (r_len != 4) {
            printf("UNABLE TO READ PONG!\n");
            exit(-1);
        } else {
            printf("%d: received pong\n", pid);
        }

        exit(0);
    }
    
    exit(0);
}
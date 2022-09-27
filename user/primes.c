#include "kernel/types.h"
#include "user.h"

void solve_prime(int fd)
{
    int base;
    if (read(fd, &base, sizeof(int)) == 0)
    {
        exit(0);
    }

    printf("prime %d\n", base);
    int p[2];
    pipe(p);

    if (!fork())
    {
        close(p[1]);
        solve_prime(p[0]);
        close(p[0]);
    }
    else 
    {
        close(p[0]);
        int n, eof;
        do 
        {
            eof = read(fd, &n, sizeof(int));
            if (n % base)
            {
                write(p[1], &n, sizeof(int));
            }
        } while (eof);
        close(p[1]);
    }

    int status;
    wait(&status);
    exit(0);
}

int main(int argc, char* argv[])
{
    if (argc != 1)
    {
        printf("primes needs no arguments\n");
        exit(0);
    }

    int parent_p[2];
    pipe(parent_p);

    if (fork())
    {
        close(parent_p[0]);
        for (int i = 2; i < 36; i++)
        {
            write(parent_p[1], &i, sizeof(int));
        }
        close(parent_p[1]);
    }
    else
    {
        close(parent_p[1]);
        solve_prime(parent_p[0]);
        close(parent_p[0]);
    }
    
    int status;
    wait(&status);
    exit(0);
}
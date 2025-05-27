#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

int main()
{
    char bin[256], param[256];
    struct timeval t0, t1, t2, t3;
    double total = 0.0;
    int ret, child;

    gettimeofday(&t0, NULL);

    while (scanf("%255s %255s", bin, param) == 2)
    {
        gettimeofday(&t2, NULL);
        child = fork();
        if (child == 0)
        {
            execl(bin, bin, param, (char *)NULL);
            printf("> Erro: %s\n", strerror(errno));
            exit(2);
        }
        else
        {
            waitpid(child, &ret, 0);
            gettimeofday(&t3, NULL);
            double dt = (t3.tv_sec - t2.tv_sec) + (t3.tv_usec - t2.tv_usec) / 1e6;
            dt = ((int)(dt * 10 + 0.5)) / 10.0;
            int code = WIFEXITED(ret) ? WEXITSTATUS(ret) : ret;
            printf("> Demorou %.1f segundos, retornou %d\n", dt, code);
            total += dt;
        }
    }

    gettimeofday(&t1, NULL);
    double ttotal = (t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) / 1e6;
    ttotal = ((int)(ttotal * 10 + 0.5)) / 10.0;
    printf(">> O tempo total foi de %.1f segundos\n", ttotal);

    return 0;
}
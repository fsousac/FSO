#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>

#define MAX_STR 256

double diff_time(struct timeval *start, struct timeval *end)
{
    double s = (double)(end->tv_sec - start->tv_sec);
    double us = (double)(end->tv_usec - start->tv_usec) / 1000000.0;
    return s + us;
}

int main()
{
    char path[MAX_STR], arg[MAX_STR];
    struct timeval total_start, total_end;
    double total = 0.0;

    gettimeofday(&total_start, NULL);

    while (scanf("%255s %255s", path, arg) == 2)
    {
        struct timeval start, end;
        gettimeofday(&start, NULL);

        pid_t pid = fork();
        int status;
        if (pid == 0)
        {
            execl(path, path, arg, (char *)NULL);
            // Se execl falhar, imprime erro e sai com 2
            fprintf(stderr, "> Erro: %s\n", strerror(errno));
            exit(2);
        }
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);
            gettimeofday(&end, NULL);
            double elapsed = diff_time(&start, &end);
            // Arredondar para 1 casa decimal sem math.h
            elapsed = ((int)(elapsed * 10 + 0.5)) / 10.0;

            if (WIFEXITED(status))
            {
                int code = WEXITSTATUS(status);
                // Se o filho saiu com 2, pode ter sido erro de exec
                if (code == 2 && access(path, X_OK) != 0)
                {
                    // Já imprimiu erro no filho, só imprime tempo 0
                    printf("> Demorou 0.0 segundos, retornou 2\n");
                    total += 0.0;
                }
                else
                {
                    printf("> Demorou %.1f segundos, retornou %d\n", elapsed, code);
                    total += elapsed;
                }
            }
            else
            {
                printf("> Demorou %.1f segundos, retornou %d\n", elapsed, status);
                total += elapsed;
            }
        }
        else
        {
            // Erro no fork
            fprintf(stderr, "> Erro: %s\n", strerror(errno));
        }
    }

    gettimeofday(&total_end, NULL);
    double elapsed_total = diff_time(&total_start, &total_end);
    elapsed_total = ((int)(elapsed_total * 10 + 0.5)) / 10.0;
    printf(">> O tempo total foi de %.1f segundos\n", elapsed_total);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <sys/shm.h>

int main(void)
{

    char cmd[255], arg[255];
    pid_t pid = 1;
    struct timeval global_start, global_end, temp_start, temp_end;
    int status;

    gettimeofday(&global_start, NULL);

    while (scanf("%s %s", cmd, arg) != EOF)
    {
        fflush(stdout);
        pid = fork();
        gettimeofday(&temp_start, NULL);

        if (pid == 0)
        {
            execl(cmd, cmd, arg, NULL);
            if (errno != 0)
                printf("> Erro: %s\n", strerror(errno));
            fflush(stdout);
            int erro = errno;
            fclose(stdin);
            exit(erro);
        }
        else
        {
            waitpid(pid, &status, WUNTRACED);
            int erro = WEXITSTATUS(status);
            gettimeofday(&temp_end, NULL);

            double tempo_execucao = (temp_end.tv_sec - temp_start.tv_sec) + 1e-6 * (temp_end.tv_usec - temp_start.tv_usec);
            printf("> Demorou %0.1lf segundos, retornou %i\n", tempo_execucao, erro);
            fflush(stdout);
        }
    }
    gettimeofday(&global_end, NULL);

    double tempo_total = (global_end.tv_sec - global_start.tv_sec) + 1e-6 * (global_end.tv_usec - global_start.tv_usec);
    if (pid != 0)
        printf(">> O tempo total foi de %0.1lf segundos\n", tempo_total);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int pid;
    long long remaining;
} Process;

int main()
{
    int N, T;
    scanf("%d %d", &N, &T);

    Process *queue = (Process *)malloc(N * sizeof(Process));
    int *finish_order = (int *)malloc(N * sizeof(int));
    long long *finish_time = (long long *)malloc(N * sizeof(long long));
    int finished = 0;

    // Leitura dos processos
    for (int i = 0; i < N; i++)
    {
        int pid, t;
        scanf("%d %d", &pid, &t);
        queue[i].pid = pid;
        queue[i].remaining = (long long)t * 1000; // Converter para milissegundos
    }

    long long current_time = 0;
    int active = N;  // Número de processos ativos
    int current = 0; // Índice do processo atual

    // Loop principal otimizado
    while (active > 0)
    {
        Process *p = &queue[current];

        if (p->remaining > 0)
        {
            // Calcula o quantum real
            long long slice = (p->remaining < T) ? p->remaining : T;
            current_time += slice;
            p->remaining -= slice;

            // Verifica se o processo terminou
            if (p->remaining == 0)
            {
                finish_order[finished] = p->pid;
                finish_time[finished] = current_time;
                finished++;
                active--;
            }
        }

        // Move para o próximo processo
        current = (current + 1) % N;
    }

    // Impressão dos resultados
    for (int i = 0; i < finished; i++)
    {
        printf("%d (%lld)\n", finish_order[i], finish_time[i]);
    }

    // Liberação de memória
    free(queue);
    free(finish_order);
    free(finish_time);

    return 0;
}
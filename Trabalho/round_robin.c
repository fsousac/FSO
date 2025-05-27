#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int id;
    int tempo;
} Process;

static void round_robin(Process processes[], int N, int quantum)
{
    if (N == 1)
    {
        printf("%d (%d)\n", processes[0].id, processes[0].tempo);
        return;
    }

    int exec = 0, atual = 0, fatia;
    int max = N - 1;

    while (max > 0)
    {
        fatia = processes[atual].tempo < quantum ? processes[atual].tempo : quantum;
        exec += fatia;
        processes[atual].tempo -= fatia;
        if (processes[atual].tempo == 0)
        {
            printf("%d (%d)\n", processes[atual].id, exec);

            for (int i = atual; i < max; ++i)
            {
                processes[i] = processes[i + 1];
            }
            max--;
        }
        else
        {
            atual++;
        }

        if (atual > max)
        {
            atual = 0;
        }
    }

    printf("%d (%d)\n", processes[0].id, exec + processes[0].tempo);
}

int main(void)
{
    int N;
    if (scanf("%d", &N) != 1 || N <= 0)
        return 0;

    int quantum;
    scanf("%d", &quantum);

    Process processes[100];
    for (int i = 0; i < N; ++i)
    {
        scanf("%d %d", &processes[i].id, &processes[i].tempo);
        processes[i].tempo *= 1000;
    }

    round_robin(processes, N, quantum);
    return 0;
}
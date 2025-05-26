#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

volatile sig_atomic_t signal_received = 0;

void handler(int signo)
{
    signal_received = 1;
}

int main()
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    // 1. Aguarda primeiro sinal
    while (!signal_received)
        pause();
    signal_received = 0;

    // 2. Cria processo zumbi
    pid_t pid = fork();
    if (pid == 0)
    {
        // Filho termina imediatamente
        _exit(0);
    }

    // 3. Aguarda segundo sinal
    while (!signal_received)
        pause();
    signal_received = 0;

    // 4. Elimina o zumbi
    waitpid(pid, NULL, 0);

    // 5. Aguarda terceiro sinal
    while (!signal_received)
        pause();

    // 6. Encerra com sucesso
    return 0;
}
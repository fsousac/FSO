#include <pthread.h>

// Contador global de trabalhos já distribuídos
int count = 0;

// Mutex global para proteger o contador
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

// Struct de argumento para as threads
struct thread_arg
{
    int vezes;
};

// Protótipo da função work (não implemente aqui)
int work(int id);

// Função executada por cada thread
void *thread_func(void *arg)
{
    struct thread_arg *targ = (struct thread_arg *)arg;
    for (int i = 0; i < targ->vezes; i++)
    {
        int id;
        // Região crítica: obter um id único
        pthread_mutex_lock(&mut);
        id = count;
        count++;
        pthread_mutex_unlock(&mut);

        // Fora da região crítica: processa o trabalho
        work(id);
    }
    return NULL;
}
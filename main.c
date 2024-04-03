// para compilar: make
// para executar: make run

/*  Integrantes
    Lucas Sales Duarte  11734490
    Daniel Filho        13677114
    Daniel Umeda        dwdw
    Lucas Corlete       dawd
    Manoel Thomaz       adwdw
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void criador(char *nome)
{
    printf("\nCriador iniciado\t%s\n", nome);
}

int main()
{

    pthread_t thread_list[6];

    /*  RANKS:
            thread_list[0] = Criador
            thread_list[1] = Depósito de Matéria Prima
            thread_list[2] = Célula de Fabricação de Canetas
            thread_list[3] = Controle
            thread_list[4] = Depósito de canetas
            thread_list[5] = Comprador
    */
    char *thread_arg = "Lucas";

    if (pthread_create(&thread_list[0], NULL, (void *)criador, (void *)thread_arg) != 0)
    {
        fprintf(stderr, "Erro ao criar o Criador\n");
        return 1;
    }
    if (pthread_join(thread_list[0], NULL) != 0)
    {
        fprintf(stderr, "erro ao iniciar a thread\n");
        return 1;
    }
    // printf("teste");
    return 0;
}
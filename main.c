// para compilar: make
// para executar: make run

/*  Integrantes
    Lucas Sales Duarte  11734490
    Daniel Filho        13677114
    Daniel Umeda        13676541
    Lucas Corlete       dawd
    Manoel Thomaz       adwdw
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_DEPOSITO_CANETA 100
#define MATERIA_PRIMA 100000


pthread_t thread_list[6];

int slots_disponiveis = MAX_DEPOSITO_CANETA;
int materia_prima_disponivel = MAX_DEPOSITO_CANETA;


/*
Suposições lógicas para descrição do projeto
    -> Produtor tem um mini armazém de matéria prima, mas fabrica e envia 1 a 1 caneta por vez 
        ( não tem armazem de produto pronto na fábrica), pois é o que faz mais sentido.
    
    ->  Se há n canetas disponíveis no depósito e o comprador pedir m canetas, tal que m > n, o depósito envia apenas n canetas
        Pois não há como ser diferente.

*/

/*  RANKS:
        thread_list[0] = Criador
        thread_list[1] = Depósito de Matéria Prima
        thread_list[2] = Célula de Fabricação de Canetas
        thread_list[3] = Controle
        thread_list[4] = Depósito de canetas
        thread_list[5] = Comprador
*/

int verifica_depos_caneta(){
    int quant_slot = slots_disponiveis;
    return quant_slot;
}

void depos_madeira(void)
{
    printf("\niniciado depos_madeira");
}

void celula_fabrica(void)
{
    printf("\niniciado celula_fabrica");
}


void controle(void)
{
    printf("\niniciado controle");
    int prod_disponivel = 0;
    
    // regiao critica
    prod_disponivel = verifica_depos_caneta();
    // regiao critica

}

void depos_caneta(void)
{
    printf("\niniciado depos_caneta");

    
}

void comprador(void)
{
    printf("\niniciado comprador");
}

void criador(void)
{

    printf("Criador iniciado\n");

    if (pthread_create(&thread_list[1], NULL, (void *)depos_madeira, 0) != 0)
    {
        fprintf(stderr, "Erro ao criar o Célula de depósito de materia prima\n");
        return;
    }
    if (pthread_join(thread_list[1], NULL) != 0)
    {
        fprintf(stderr, "erro ao iniciar a thread\n");
        return;
    }
    if (pthread_create(&thread_list[2], NULL, (void *)celula_fabrica, 0) != 0)
    {
        fprintf(stderr, "Erro ao criar o celula de fabricao de canetas\n");
        return;
    }
    if (pthread_join(thread_list[2], NULL) != 0)
    {
        fprintf(stderr, "erro ao iniciar a thread\n");
        return;
    }

    if (pthread_create(&thread_list[3], NULL, (void *)controle, 0) != 0)
    {
        fprintf(stderr, "Erro ao criar o Controle\n");
        return;
    }
    if (pthread_join(thread_list[3], NULL) != 0)
    {
        fprintf(stderr, "erro ao iniciar a thread\n");
        return;
    }
    if (pthread_create(&thread_list[4], NULL, (void *)depos_caneta, 0) != 0)
    {
        fprintf(stderr, "Erro ao criar o Depósito de canetas\n");
        return;
    }
    if (pthread_join(thread_list[4], NULL) != 0)
    {
        fprintf(stderr, "erro ao iniciar a thread\n");
        return;
    }
    if (pthread_create(&thread_list[5], NULL, (void *)comprador, 0) != 0)
    {
        fprintf(stderr, "Erro ao criar o Comprador\n");
        return;
    }
    if (pthread_join(thread_list[5], NULL) != 0)
    {
        fprintf(stderr, "erro ao iniciar a thread\n");
        return;
    }
}

int main()
{


    if (pthread_create(&thread_list[0], NULL, (void *)criador, NULL) != 0)
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
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
#include <unistd.h>    //  Para o uso da função sleep()
#include <pthread.h>   //  Para uso das threads
#include <semaphore.h> //  Para uso dos semaforos

#define DELAY_COMPRA_CONSUMIDOR 1
#define COMPRA_POR_INTERACAO 1
#define ENVIO_MATERIA_INTERACAO 3
#define TEMPO_ENVIO_DEPFAB 1
#define TEMPO_PRODUCAO_CANETA 1
#define MAX_DEPOSITO_CANETA 8
#define MAX_MATERIA_PRIMA 200

pthread_t thread_list_ID[5];

int slots_disponiveis = MAX_DEPOSITO_CANETA;
int materia_prima_disponivel = MAX_MATERIA_PRIMA;

int emProducao = 0;
pthread_mutex_t acessar_deposito_caneta = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t acessar_deposito_materiaPrima = PTHREAD_MUTEX_INITIALIZER;
sem_t decrementa_materiaPrima;
sem_t produza;
sem_t enviarCaneta;
// pthread_mutex_t sla;
// pthread_cond_t produza;

/*
Suposições lógicas para descrição do projeto
    -> Produtor tem um mini armazém de matéria prima, mas fabrica e envia 1 a 1 caneta por vez
        ( não tem armazem de produto pronto na fábrica), pois é o que faz mais sentido.

    ->  Se há n canetas disponíveis no depósito e o comprador pedir m canetas, tal que m > n, o depósito envia apenas n canetas
        Pois não há como ser diferente.

*/

/*  RANKS:
        thread_list_ID[0] = Depósito de Matéria Prima
        thread_list_ID[1] = Célula de Fabricação de Canetas
        thread_list_ID[2] = Controle
        thread_list_ID[3] = Depósito de canetas
        thread_list_ID[4] = Comprador
*/
// -------------------------- Funções acessoras --------------------------
int verifica_depos_caneta()
{
    int quant_slot = slots_disponiveis;
    return quant_slot;
}
int verifica_depos_materiaPrima()
{
    int quant_slot = materia_prima_disponivel;
    return quant_slot;
}

// -------------------------- Threads --------------------------
void depos_madeira(void)
{
    printf("\niniciado depos_madeira");

    while (1)
    {
        sem_wait(&decrementa_materiaPrima);
        pthread_mutex_lock(&acessar_deposito_materiaPrima);
        materia_prima_disponivel--;
        // printf("\nDEPMAT: materia prima disponivel: %d", materia_prima_disponivel);
        pthread_mutex_unlock(&acessar_deposito_materiaPrima);
    }
}

void celula_fabrica(void)
{
    printf("\nIniciado celula_fabrica");

    while (1)
    {
        sem_wait(&produza);
        emProducao = 1;
        printf("\nFAB:  fábrica começou a produzir\n");
        // int buffer_produzido = 0;
        // int buffer_aProduzir = ENVIO_MATERIA_INTERACAO;
        // for (int i = 0; i < ENVIO_MATERIA_INTERACAO; i++)
        // {
        // buffer_aProduzir--;
        sleep(TEMPO_PRODUCAO_CANETA);
        // buffer_produzido++;
        emProducao = 0;
        sem_post(&enviarCaneta);
        // }
        // sem_post(&produza);
    }
}

void depos_caneta(void)
{
    printf("\niniciado depos_caneta");
    while (1)
    {
        sem_wait(&enviarCaneta);

        pthread_mutex_lock(&acessar_deposito_caneta);
        slots_disponiveis--;
        printf("\nDEPCAN:   quantidade de canetas no depósito são %d e slots %d", MAX_DEPOSITO_CANETA - slots_disponiveis, slots_disponiveis);
        pthread_mutex_unlock(&acessar_deposito_caneta);
    }
}

void comprador(void)
{
    printf("\niniciado comprador");
}

void controle(void)
{
    printf("\niniciado controle");

    // Inicio da região crítica
    while (1)
    {

        pthread_mutex_lock(&acessar_deposito_caneta);
        pthread_mutex_lock(&acessar_deposito_materiaPrima);
        int slot_disponivel_local = slots_disponiveis;
        int estoque_materiaPrima_local = materia_prima_disponivel;
        pthread_mutex_unlock(&acessar_deposito_materiaPrima);
        pthread_mutex_unlock(&acessar_deposito_caneta);
        if (estoque_materiaPrima_local <= 0 && slot_disponivel_local <= 0)

        {
            //  Finalizar programa
            printf("\n\tCódigo finalizado. Estoque de materia prima acabou e o depósito de canetas está vazio");
            break;
        }
        if (slot_disponivel_local > 0 && estoque_materiaPrima_local > 0)
        {
            // printf("\nsem_getValue da variavel ");
            if (!emProducao)
            {
                sem_post(&decrementa_materiaPrima);

                for (int i = 0; i < ENVIO_MATERIA_INTERACAO; i++) //  Decrementa x unidades do estoque de materia prima
                {
                    pthread_mutex_lock(&acessar_deposito_materiaPrima);
                    materia_prima_disponivel--;
                    pthread_mutex_unlock(&acessar_deposito_materiaPrima);
                }
                sem_post(&produza);
            }
        }
    }
}
int main()
{
    //  Inicializar os semáforos
    sem_init(&decrementa_materiaPrima, 0, 0); //  Inicia o semaforo para controle de estoque de materia prima
    sem_init(&produza, 0, 0);                 //  Inicia o semaforo para não produzindo
    sem_init(&enviarCaneta, 0, 0);            //  Inicia o semaforo para o envio das canetas

    //  Criação das threads
    {
        if (pthread_create(&thread_list_ID[0], 0, (void *)depos_madeira, 0) != 0)
        {
            fprintf(stderr, "Erro ao criar o Célula de depósito de materia prima\n");
        }
        if (pthread_create(&thread_list_ID[1], 0, (void *)celula_fabrica, 0) != 0)
        {
            fprintf(stderr, "Erro ao criar o celula de fabricao de canetas\n");
        }

        if (pthread_create(&thread_list_ID[2], 0, (void *)controle, 0) != 0)
        {
            fprintf(stderr, "Erro ao criar o Controle\n");
        }
        if (pthread_create(&thread_list_ID[3], 0, (void *)depos_caneta, 0) != 0)
        {
            fprintf(stderr, "Erro ao criar o Depósito de canetas\n");
        }
        if (pthread_create(&thread_list_ID[4], 0, (void *)comprador, 0) != 0)
        {
            fprintf(stderr, "Erro ao criar o Comprador\n");
        }
        {
            //  Threads iniciadas e agora serão dadas o join

            // if (pthread_join(thread_list_ID[0], NULL) != 0)
            // {
            //     fprintf(stderr, "erro ao iniciar a thread\n");
            // }
            // if (pthread_join(thread_list_ID[1], NULL) != 0)
            // {
            //     fprintf(stderr, "erro ao iniciar a thread\n");
            // }
            // if (pthread_join(thread_list_ID[2], NULL) != 0)
            // {
            //     fprintf(stderr, "erro ao iniciar a thread\n");
            // }
            // if (pthread_join(thread_list_ID[3], NULL) != 0)
            // {
            //     fprintf(stderr, "erro ao iniciar a thread\n");
            // }
            // if (pthread_join(thread_list_ID[4], NULL) != 0)
            // {
            //     fprintf(stderr, "erro ao iniciar a thread\n");
            // }
        }
    }
    controle();
    /* coisas a acertar no código:
        acertar essa linha pra == 0            if (estoque_materiaPrima <= 0 && slot_disponivel <= 0)



        destruir os semaforos aqui no final
    */

    return 0;
}
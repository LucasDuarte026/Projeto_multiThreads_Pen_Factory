// para compilar: make
// para executar: make run

/*  Integrantes
    Lucas Sales Duarte  11734490
    Daniel Filho        13677114
    Daniel Umeda        13676541
    Manoel Thomaz       13676392
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>    //  Para o uso da função sleep()
#include <pthread.h>   //  Para uso das threads
#include <semaphore.h> //  Para uso dos semaforos

#define DELAY_COMPRA_CONSUMIDOR 3
#define COMPRA_POR_INTERACAO 2
#define ENVIO_MATERIA_INTERACAO 3
#define TEMPO_ENVIO_DEPFAB 2
#define TEMPO_PRODUCAO_CANETA 1
#define MAX_DEPOSITO_CANETA 8
#define MAX_MATERIA_PRIMA 200

pthread_t thread_list_ID[5];

int slots_disponiveis = MAX_DEPOSITO_CANETA;
int materia_prima_disponivel = MAX_MATERIA_PRIMA;

int emProducao = 0;
pthread_mutex_t acessar_deposito_materiaPrima = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t acessar_deposito_caneta = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t enviarDepMat = PTHREAD_COND_INITIALIZER;
pthread_cond_t fabrique = PTHREAD_COND_INITIALIZER;

sem_t decrementa_materiaPrima;
sem_t produza;
sem_t enviarCaneta;
int canetasCompradas = 0;

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

// -------------------------- Threads --------------------------
void controle(void)
{
    printf("\niniciado controle");

    // Inicio da região crítica
    while (1)
    {
        sleep(3); //  Tempo suficiente para as outras threads começarem
        pthread_mutex_lock(&acessar_deposito_caneta);
        pthread_mutex_lock(&acessar_deposito_materiaPrima);
        int slot_disponivel_local = slots_disponiveis;
        int estoque_materiaPrima_local = materia_prima_disponivel;
        pthread_mutex_unlock(&acessar_deposito_materiaPrima);
        pthread_mutex_unlock(&acessar_deposito_caneta);
        // printf("\nControle.| Slots: %d | |materiaPRima: %d|", slot_disponivel_local, estoque_materiaPrima_local);
        if (estoque_materiaPrima_local <= 0 && slot_disponivel_local <= 0)
        {
            //  Finalizar programa
            printf("\n\tCódigo finalizado. Estoque de materia prima acabou e o depósito de canetas está vazio");
            break;
        }
        if (slot_disponivel_local > 0 && estoque_materiaPrima_local > 0)
        {
            if (emProducao == 0) //  caso produção ociosa (0), pode começar a a produzir
            {
                // sem_post(&decrementa_materiaPrima);
                for (int i = 0; i < ENVIO_MATERIA_INTERACAO; i++) //  Decrementa x unidades do estoque de materia prima
                {
                    pthread_mutex_lock(&acessar_deposito_materiaPrima);
                    materia_prima_disponivel--;
                    pthread_mutex_unlock(&acessar_deposito_materiaPrima);
                    pthread_cond_signal(&enviarDepMat);
                    sleep(TEMPO_ENVIO_DEPFAB);

                    // sem_post(&produza);
                    pthread_cond_signal(&fabrique);
                } 
            }
        }
    }
}

void depos_madeira(void)
{
    printf("\niniciado depos_madeira");

    while (1)
    {
        if (1)
        {
            pthread_mutex_lock(&acessar_deposito_materiaPrima);
            pthread_cond_wait(&enviarDepMat, &acessar_deposito_materiaPrima);
            // sem_wait(&decrementa_materiaPrima);

            materia_prima_disponivel--;
            // printf("\nDEPMAT: materia prima disponivel: %d", materia_prima_disponivel);
            pthread_mutex_unlock(&acessar_deposito_materiaPrima);
            // sleep(TEMPO_ENVIO_DEPFAB);
        }
    }
}

void celula_fabrica(void)
{
    printf("\nIniciado celula_fabrica");

    while (1)
    {
        pthread_mutex_lock(&acessar_deposito_caneta);
        if (slots_disponiveis > 0)
        {
            // sem_wait(&produza);
            pthread_cond_wait(&fabrique, &acessar_deposito_caneta);
            emProducao = 1;
            printf("\nFAB:  fábrica começou a produzir");
            sleep(TEMPO_PRODUCAO_CANETA);
            emProducao = 0;

            sem_post(&enviarCaneta);
        }
        pthread_mutex_unlock(&acessar_deposito_caneta);
    }
}

void depos_caneta(void)
{
    printf("\niniciado depos_caneta");
    while (1)
    {
        pthread_mutex_lock(&acessar_deposito_caneta);
        if (slots_disponiveis > 0)
        {
            sem_wait(&enviarCaneta);

            slots_disponiveis--;
            printf("\nDEPCAN:   quantidade de canetas no depósito são %d e slots %d", MAX_DEPOSITO_CANETA - slots_disponiveis, slots_disponiveis);
        }
        pthread_mutex_unlock(&acessar_deposito_caneta);
    }
}

void comprador(void)
{
    printf("\niniciado comprador");
    while (1)
    {
        pthread_mutex_lock(&acessar_deposito_caneta);
        int slot_caneta_local = slots_disponiveis;
        pthread_mutex_unlock(&acessar_deposito_caneta);
        int local_max = MAX_DEPOSITO_CANETA - slot_caneta_local;
        if (local_max > 0)
        {
            if (local_max < COMPRA_POR_INTERACAO) //  Há menos canetas disponíveis do que o pedido
            {
                pthread_mutex_lock(&acessar_deposito_caneta);
                for (int i = 0; i < local_max; i++)
                {
                    slots_disponiveis++;
                    canetasCompradas++;
                    sleep(DELAY_COMPRA_CONSUMIDOR);
                }
                pthread_mutex_unlock(&acessar_deposito_caneta);
                printf("\nComprador acaba de comprar %d canetas | total: %d", local_max, canetasCompradas);
            }
            else
            {
                pthread_mutex_lock(&acessar_deposito_caneta);
                for (int i = 0; i < COMPRA_POR_INTERACAO; i++)
                {
                    slots_disponiveis++;
                    canetasCompradas++;
                    sleep(DELAY_COMPRA_CONSUMIDOR);
                }
                pthread_mutex_unlock(&acessar_deposito_caneta);
                printf("\nCOMPRADOR: acaba de comprar %d canetas | Total: %d", COMPRA_POR_INTERACAO, canetasCompradas);
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
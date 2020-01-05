/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Pedro Kiyuna e Daniel Escobar
 *
 *  Created on 9 de Maio de 2019, 16:35
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "fila.h"
#include "priorityQueue.h"

typedef struct little_ {
    double tempo_anterior;
    double soma_areas;
    double qtd_pacotes;
} little;

typedef struct saidaInfo_ {
    int tipo;
    int has_pct;
    double tempoSaida;
} saidaInfo;

/**
 * 
 * @return numero aleatorio entre (0,1]
 */
double aleatorio() {
    double u;
    u = rand() % RAND_MAX;
    u = u / RAND_MAX;
    u = 1.0 - u;

    return (u);
}

/**
 * 
 * @param l parametro da exponencial
 * @return intervalo de tempo, com media tendendo ao intervalo
 * informado pelo usuario.
 */
double exponencial_tempo(double l) {
    return ((-1.0 / l) * log(aleatorio()));
}

/**
 * 
 * @return tamanho do pacote que acabou de chegar,
 * seguindo a proporcao aproximada de 50% = 550 Bytes,
 * 40% = 40 Bytes e 10% = 1500 Bytes.
 */
double gera_tam_pct() {
    double a = aleatorio();
    //tamanhos convertidos para Mb
    if (a <= 0.5) {
        return ((550.0 * 8.0) / (1000000.0));
    } else if (a <= 0.9) {
        return ((40.0 * 8.0) / (1000000.0));
    }
    return ((1500.0 * 8.0) / (1000000.0));
}

/**
 * 
 * @param a valor
 * @param b valor
 * @return menor dentre os valosres
 */
double minimo(double a, double b) {
    if (a <= b)
        return a;
    return b;
}

/**
 * 
 * @param l variavel de little que sera iniciada
 */
void inicia_little(little * l) {
    l->qtd_pacotes = 0.0;
    l->soma_areas = 0.0;
    l->tempo_anterior = 0.0;
}

/*
 * 
 */
int main() {
    pacote ** inicio = malloc(sizeof (pacote *));
    pacote ** fim = malloc(sizeof (pacote *));

    *inicio = NULL;
    *fim = NULL;

    PriorityQueue * priorityQueue = createQueue();

    PriorityQueue * WebQueue = createQueue();
    PriorityQueue * CBRQueue = createQueue();

    //variavel para en
    little en;
    //variavel para ew_chegada
    little ew_chegada;
    //variavel para ew_saida
    little ew_saida;

    inicia_little(&en);
    inicia_little(&ew_chegada);
    inicia_little(&ew_saida);
    
    //variavel para en
    little en_web;
    //variavel para ew_chegada
    little ew_chegada_web;
    //variavel para ew_saida
    little ew_saida_web;

    inicia_little(&en_web);
    inicia_little(&ew_chegada_web);
    inicia_little(&ew_saida_web);
    
    
    //variavel para en
    little en_cbr;
    //variavel para ew_chegada
    little ew_chegada_cbr;
    //variavel para ew_saida
    little ew_saida_cbr;

    inicia_little(&en_cbr);
    inicia_little(&ew_chegada_cbr);
    inicia_little(&ew_saida_cbr);

    //iniciando a semente 
    //para a geracao dos numeros
    //pseudoaleatorios

    int semente = time(NULL);
    //int semente = 1556915527;
    printf("Semente: %d\n", semente);
    srand(semente);
    //tempo atual
    double tempo = 0.0;
    //tempo total
    double tempo_total;
    printf("Informe o tempo total de simulacao: ");
    scanf("%lF", &tempo_total);

    //intervalo medio entre chegadas
    double intervalo_web;
    printf("Informe o intervalo médio de tempo (em segundos) entre pacotes: ");
    scanf("%lF", &intervalo_web);
    //ajustando parametro para a exponencial
    intervalo_web = 1.0 / intervalo_web;

    //intervalo medio entre ligações
    double intervalo_ligacoes;
    printf("Informe o intervalo médio de tempo (em segundos) entre ligações: ");
    scanf("%lF", &intervalo_ligacoes);
    //ajustando parametro para a exponencial
    intervalo_ligacoes = 1.0 / intervalo_ligacoes;


    //tempo médio de ligações
    double tempo_ligacoes;
    printf("Informe o tempo médio das ligações: ");
    scanf("%lF", &tempo_ligacoes);
    double media_ponderada = (0.5 * ((550.0 * 8.0)));
    media_ponderada += (0.4 * ((40.0 * 8.0)));
    media_ponderada += (0.1 * ((1500.0 * 8.0)));


    //ajustando parametro para a exponencial
    tempo_ligacoes = 1.0 / tempo_ligacoes;

    //tamanho do pacote gerado
    double tam_pct;

    //tamanho do pacote cbr gerado
    double tam_pct_cbr = ((1200.0 * 8.0) / (1000000.0));

    //tamanho do link de saida do roteador
    double link;
    printf("Informe o tamanho do link (Mbps): ");
    scanf("%lF", &link);

    //fila, onde fila == 0 indica
    //roteador vazio; fila == 1
    //indica 1 pacote, ja em transmissao;
    //fila > 1 indica 1 pacote em transmissao,
    //e demais em espera
    //double fila = 0.0;


    //tempo de chegada do proximo pacote
    //cbr ao sistema, com intervalo
    //de 20 ms entre pacotes
    //double chegada_proximo_pct_cbr = 0.02;

    //tempo de saida do pacote que esta
    //sendo atendido atualmente
    saidaInfo saidaInfo;
    saidaInfo.tipo = 0;
    saidaInfo.has_pct = 0;
    saidaInfo.tempoSaida = 0.0;

    //variavel para diferenciar web, cbr ou saida atendimento
    int tipo;


    //variavel para o calculo da ocupacao
    //do roteador
    double ocupacao = 0.0;


    DataNode data;
    data.chegada_pct = exponencial_tempo(intervalo_web);
    data.tamanho_pct = gera_tam_pct();
    data.tempo_duracao = tempo_total;
    data.tipo = 0;
    push(priorityQueue, data, data.chegada_pct);

    data.chegada_pct = exponencial_tempo(intervalo_ligacoes);
    data.tamanho_pct = tam_pct_cbr;
    data.tempo_duracao = exponencial_tempo(tempo_ligacoes);
    data.tipo = 2;
    push(priorityQueue, data, data.chegada_pct);


    Node* minimo = (Node*) malloc(sizeof (Node));
    while (tempo <= tempo_total) {

        //roteador vazio. Logo, avanco
        //no tempo para a chegada do
        //proximo pacote.
        if (*inicio == NULL) {
            free(minimo);
            minimo = newNode(priorityQueue->head->data, priorityQueue->head->data.chegada_pct);
            //tempo = priorityQueue->head->priority;
            //tipo = priorityQueue->head->data.tipo;
            tempo = minimo->data.chegada_pct;
            tipo = minimo->data.tipo;

        } else {
            //ha fila!
            if (saidaInfo.has_pct) {
                tempo = saidaInfo.tempoSaida;
                tipo = saidaInfo.tipo;
            } else {
                free(minimo);
                if (CBRQueue->head->priority <= (WebQueue->head->priority * 2)) {
                    minimo = newNode(CBRQueue->head->data, CBRQueue->head->priority);
                    tempo = minimo->data.chegada_pct + minimo->priority;
                    tipo = minimo->data.tipo;
                } else {
                    minimo = newNode(WebQueue->head->data, WebQueue->head->priority);
                    tempo = minimo->data.chegada_pct + minimo->priority;
                    tipo = minimo->data.tipo;
                }
            }
        }
        //chegada de pacote
        if (tipo == 0) {

            if (*inicio == NULL) {
                //gerando o tempo de atendimento
                saidaInfo.tempoSaida = tempo + minimo->data.tamanho_pct / link;
                saidaInfo.tipo = 3;
                //calculo da ocupacao
                ocupacao += saidaInfo.tempoSaida - tempo;
            }
            //pacote colocado na fila
            inserir(inicio, fim, minimo->data.tamanho_pct);
            saidaInfo.has_pct = 1;

            //gerar o tempo de chegada do proximo
            minimo->data.chegada_pct = minimo->data.chegada_pct + exponencial_tempo(intervalo_web);
            minimo->priority = minimo->data.chegada_pct;
            //gero o tamanho do pacote
            minimo->data.tamanho_pct = gera_tam_pct();
            edit(priorityQueue, minimo);
            //list->head->data.chegada_pct = list->head->data.chegada_pct + exponencial_tempo(intervalo_web);
            //chegada_proximo_pct = tempo + exponencial_tempo(intervalo_web);         
            
            //calculo little -- E[N]
            en_web.soma_areas += en_web.qtd_pacotes * (tempo - en_web.tempo_anterior);
            en_web.qtd_pacotes++;
            en_web.tempo_anterior = tempo;

            //calculo little -- E[W] chegada
            ew_chegada_web.soma_areas += ew_chegada_web.qtd_pacotes * (tempo - ew_chegada_web.tempo_anterior);
            ew_chegada_web.qtd_pacotes++;
            ew_chegada_web.tempo_anterior = tempo;
            

            //calculo little -- E[N]
            en.soma_areas += en.qtd_pacotes * (tempo - en.tempo_anterior);
            en.qtd_pacotes++;
            en.tempo_anterior = tempo;

            //calculo little -- E[W] chegada
            ew_chegada.soma_areas += ew_chegada.qtd_pacotes * (tempo - ew_chegada.tempo_anterior);
            ew_chegada.qtd_pacotes++;
            ew_chegada.tempo_anterior = tempo;
        } else if (tipo == 1) {
            //printf("CBR chegou!\n");

            if (*inicio == NULL) {
                //gerando o tempo de atendimento
                saidaInfo.tempoSaida = tempo + minimo->data.tamanho_pct / link;
                saidaInfo.tipo = 4;
                //calculo da ocupacao
                ocupacao += saidaInfo.tempoSaida - tempo;
            }
            //pacote colocado na fila
            inserir(inicio, fim, minimo->data.tamanho_pct);
            saidaInfo.has_pct = 1;

            //gerar o tempo de chegada do proximo
            minimo->data.chegada_pct = minimo->data.chegada_pct + 0.02;
            minimo->priority = minimo->data.chegada_pct;
            minimo->data.tempo_duracao = minimo->data.tempo_duracao - 0.02;
            if (minimo->data.tempo_duracao < 0.02) {
                pop(priorityQueue);
            } else {
                edit(priorityQueue, minimo);
            }
            
            //calculo little -- E[N]
            en_cbr.soma_areas += en_cbr.qtd_pacotes * (tempo - en_cbr.tempo_anterior);
            en_cbr.qtd_pacotes++;
            en_cbr.tempo_anterior = tempo;

            //calculo little -- E[W] chegada
            ew_chegada_cbr.soma_areas += ew_chegada_cbr.qtd_pacotes * (tempo - ew_chegada_cbr.tempo_anterior);
            ew_chegada_cbr.qtd_pacotes++;
            ew_chegada_cbr.tempo_anterior = tempo;

            //calculo little -- E[N]
            en.soma_areas += en.qtd_pacotes * (tempo - en.tempo_anterior);
            en.qtd_pacotes++;
            en.tempo_anterior = tempo;

            //calculo little -- E[W] chegada
            ew_chegada.soma_areas += ew_chegada.qtd_pacotes * (tempo - ew_chegada.tempo_anterior);
            ew_chegada.qtd_pacotes++;
            ew_chegada.tempo_anterior = tempo;
        } else if (tipo == 2) {
            //printf("CBR chegou!\n");

            if (*inicio == NULL) {
                //gerando o tempo de atendimento
                saidaInfo.tempoSaida = tempo + priorityQueue->head->data.tamanho_pct / link;
                saidaInfo.tipo = 4;
                //calculo da ocupacao
                ocupacao += saidaInfo.tempoSaida - tempo;
            }
            //pacote colocado na fila
            inserir(inicio, fim, priorityQueue->head->data.tamanho_pct);
            saidaInfo.has_pct = 1;

            minimo->data.chegada_pct = minimo->data.chegada_pct + 0.02;
            minimo->priority = minimo->data.chegada_pct;
            minimo->data.tempo_duracao = minimo->data.tempo_duracao - 0.02;
            minimo->data.tipo = 1;
            if (minimo->data.tempo_duracao < 0.02) {
                pop(priorityQueue);
            } else {
                edit(priorityQueue, minimo);
            }

            DataNode data_cbr2;

            if ((tempo + exponencial_tempo(intervalo_ligacoes)) < tempo_total) {
                data_cbr2.chegada_pct = tempo + exponencial_tempo(intervalo_ligacoes);
                data_cbr2.tamanho_pct = tam_pct_cbr;
                data_cbr2.tempo_duracao = exponencial_tempo(tempo_ligacoes);
                data_cbr2.tipo = 2;
                push(priorityQueue, data_cbr2, data_cbr2.chegada_pct);
            }
            
            //calculo little -- E[N]
            en_cbr.soma_areas += en_cbr.qtd_pacotes * (tempo - en_cbr.tempo_anterior);
            en_cbr.qtd_pacotes++;
            en_cbr.tempo_anterior = tempo;

            //calculo little -- E[W] chegada
            ew_chegada_cbr.soma_areas += ew_chegada_cbr.qtd_pacotes * (tempo - ew_chegada_cbr.tempo_anterior);
            ew_chegada_cbr.qtd_pacotes++;
            ew_chegada_cbr.tempo_anterior = tempo;

            //calculo little -- E[N]
            en.soma_areas += en.qtd_pacotes * (tempo - en.tempo_anterior);
            en.qtd_pacotes++;
            en.tempo_anterior = tempo;

            //calculo little -- E[W] chegada
            ew_chegada.soma_areas += ew_chegada.qtd_pacotes * (tempo - ew_chegada.tempo_anterior);
            ew_chegada.qtd_pacotes++;
            ew_chegada.tempo_anterior = tempo;
        } else if (tipo == 3) {//saida de pacote
            //printf("Saida de pacote no tempo: %lF\n", tempo);
            remover(inicio);
            //printf("Fila: %lF\n", fila);

            if (*inicio != NULL) {
                //obtem o tamanho do pacote
                tam_pct = (*inicio)->tamanho;
                //gerando o tempo em que o pacote
                //atual saira do sistema
                saidaInfo.tempoSaida = tempo + tam_pct / link;

                if (saidaInfo.tempoSaida > minimo->data.chegada_pct) {
                    push(WebQueue, minimo->data, saidaInfo.tempoSaida - minimo->data.chegada_pct);
                }

                ocupacao += saidaInfo.tempoSaida - tempo;
            }
            saidaInfo.has_pct = 0;
            
            //calculo little -- E[N]
            en_web.soma_areas += en_web.qtd_pacotes * (tempo - en_web.tempo_anterior);
            en_web.qtd_pacotes--;
            en_web.tempo_anterior = tempo;

            //calculo little -- E[W] saida
            ew_saida_web.soma_areas += ew_saida_web.qtd_pacotes * (tempo - ew_saida_web.tempo_anterior);
            ew_saida_web.qtd_pacotes++;
            ew_saida_web.tempo_anterior = tempo;

            //calculo little -- E[N]
            en.soma_areas += en.qtd_pacotes * (tempo - en.tempo_anterior);
            en.qtd_pacotes--;
            en.tempo_anterior = tempo;

            //calculo little -- E[W] saida
            ew_saida.soma_areas += ew_saida.qtd_pacotes * (tempo - ew_saida.tempo_anterior);
            ew_saida.qtd_pacotes++;
            ew_saida.tempo_anterior = tempo;
        } else if (tipo == 4) {//saida de pacote
            //printf("Saida de pacote no tempo: %lF\n", tempo);
            remover(inicio);
            //printf("Fila: %lF\n", fila);

            if (*inicio != NULL) {
                //obtem o tamanho do pacote
                tam_pct = (*inicio)->tamanho;
                //gerando o tempo em que o pacote
                //atual saira do sistema
                saidaInfo.tempoSaida = tempo + tam_pct / link;

                if (saidaInfo.tempoSaida > minimo->data.chegada_pct) {
                    push(CBRQueue, minimo->data, saidaInfo.tempoSaida - minimo->data.chegada_pct);
                }

                ocupacao += saidaInfo.tempoSaida - tempo;
            }
            saidaInfo.has_pct = 0;

            //calculo little -- E[N]
            en_cbr.soma_areas += en_cbr.qtd_pacotes * (tempo - en_cbr.tempo_anterior);
            en_cbr.qtd_pacotes--;
            en_cbr.tempo_anterior = tempo;

            //calculo little -- E[W] saida
            ew_saida_cbr.soma_areas += ew_saida_cbr.qtd_pacotes * (tempo - ew_saida_cbr.tempo_anterior);
            ew_saida_cbr.qtd_pacotes++;
            ew_saida_cbr.tempo_anterior = tempo;
            
            //calculo little -- E[N]
            en.soma_areas += en.qtd_pacotes * (tempo - en.tempo_anterior);
            en.qtd_pacotes--;
            en.tempo_anterior = tempo;

            //calculo little -- E[W] saida
            ew_saida.soma_areas += ew_saida.qtd_pacotes * (tempo - ew_saida.tempo_anterior);
            ew_saida.qtd_pacotes++;
            ew_saida.tempo_anterior = tempo;
        }
        //printf("==========================\n");
        //getchar();
    }
    
    ew_saida_web.soma_areas += ew_saida_web.qtd_pacotes * (tempo - ew_saida_web.tempo_anterior);
    ew_chegada_web.soma_areas += ew_chegada_web.qtd_pacotes * (tempo - ew_chegada_web.tempo_anterior);
    
    ew_saida_cbr.soma_areas += ew_saida_cbr.qtd_pacotes * (tempo - ew_saida_cbr.tempo_anterior);
    ew_chegada_cbr.soma_areas += ew_chegada_cbr.qtd_pacotes * (tempo - ew_chegada_cbr.tempo_anterior);
    
    ew_saida.soma_areas += ew_saida.qtd_pacotes * (tempo - ew_saida.tempo_anterior);
    ew_chegada.soma_areas += ew_chegada.qtd_pacotes * (tempo - ew_chegada.tempo_anterior);

    double en_final_web = en_web.soma_areas / tempo;
    double ew_web = ew_chegada_web.soma_areas - ew_saida_web.soma_areas;
    ew_web = ew_web / ew_chegada_web.qtd_pacotes;
    
    double en_final_cbr = en_cbr.soma_areas / tempo;
    double ew_cbr = ew_chegada_cbr.soma_areas - ew_saida_cbr.soma_areas;
    ew_cbr = ew_cbr / ew_chegada_cbr.qtd_pacotes;
    
    double en_final = en.soma_areas / tempo;
    double ew = ew_chegada.soma_areas - ew_saida.soma_areas;
    ew = ew / ew_chegada.qtd_pacotes;

    double lambda_web = ew_chegada_web.qtd_pacotes / tempo;
    
    double lambda_cbr = ew_chegada_cbr.qtd_pacotes / tempo;
    
    double lambda = ew_chegada.qtd_pacotes / tempo;
    
    printf("Ocupacao: %lF\n\n\n", ocupacao / tempo);
    printf("\n======Little WEB======\n");
    printf("E[N] = %lF\n", en_final_web);
    printf("E[W] = %lF\n", ew_web);
    printf("Lambda = %lF\n", lambda_web);
    printf("\n==================\n");
    printf("Validacao Little: %.20lF\n", en_final_web - (lambda_web * ew_web));
    
    //printf("Ocupacao: %lF\n", ocupacao / tempo);
    printf("\n======Little CBR======\n");
    printf("E[N] = %lF\n", en_final_cbr);
    printf("E[W] = %lF\n", ew_cbr);
    printf("Lambda = %lF\n", lambda_cbr);
    printf("\n==================\n");
    printf("Validacao Little: %.20lF\n", en_final_cbr - (lambda_cbr * ew_cbr));

    //printf("Ocupacao: %lF\n", ocupacao / tempo);
    printf("\n======Little Geral======\n");
    printf("E[N] = %lF\n", en_final);
    printf("E[W] = %lF\n", ew);
    printf("Lambda = %lF\n", lambda);
    printf("\n==================\n");
    printf("Validacao Little: %.20lF\n", en_final - (lambda * ew));

    //	printf("Pacotes gerados: %lF\n", cont_pcts);
    //	printf("Media do intervalo: %lF\n", tempo/cont_pcts);	

    return (EXIT_SUCCESS);
}


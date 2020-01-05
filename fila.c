/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fila.c
 * Author: Pedro Kiyuna e Daniel Escobar
 *
 * Created on 9 de Maio de 2019, 16:35
 */

#include <stdlib.h>
#include <stdio.h>
#include "fila.h"

pacote * aloca_pct() {
    return (malloc(sizeof (pacote)));
}

fila * aloca_fila() {
    return (malloc(sizeof (fila)));
}

/**
 * 
 * @param inicio aponta o primeiro da fila
 * @param fim aponta o ultimo da fila
 * @param tamanho do pacote a ser inserido
 * @return 1 caso insira, 0 caso erro
 */
int inserir(pacote ** inicio, pacote ** fim, double tamanho) {
    //fila vazia
    if (*inicio == NULL) {
        *inicio = aloca_pct();
        if (*inicio == NULL) {
            return 0;
        }
        *fim = *inicio;
        (*inicio)->tamanho = tamanho;
        (*inicio)->prox = NULL;
        return 1;
    } else {
        pacote * tmp = aloca_pct();
        if (tmp == NULL) {
            return 0;
        }
        tmp->tamanho = tamanho;
        tmp->prox = NULL;

        (*fim)->prox = tmp;
        (*fim) = tmp;
        return 1;
    }
}

/*
 * @param 
 * @param 
 * @param 
 * @return 1 caso insira, 0 caso erro
*/

int inserirFila(fila ** inicio, fila ** fim, DataNode data, double atraso) {
    //fila vazia
    if (*inicio == NULL) {
        *inicio = aloca_fila();
        if (*inicio == NULL) {
            return 0;
        }
        *fim = *inicio;
        (*inicio)->atraso = atraso;
        (*inicio)->chegada_pct = data.chegada_pct;
        (*inicio)->tamanho_pct = data.tamanho_pct;
        (*inicio)->tempo_duracao = data.tempo_duracao;

        (*inicio)->prox = NULL;
        return 1;
    } else {
        fila * tmp = aloca_fila();
        if (tmp == NULL) {
            return 0;
        }
        
        tmp->atraso = atraso;
        tmp->chegada_pct = data.chegada_pct;
        tmp->tamanho_pct = data.tamanho_pct;
        tmp->tempo_duracao = data.tempo_duracao;
        tmp->prox = NULL;

        (*fim)->prox = tmp;
        (*fim) = tmp;
        return 1;
    }
}


/**
 * 
 * @param inicio da fila
 * @return 0 caso fila vazia, tamanho do pct caso contrario
 */
double remover(pacote ** inicio) {
    if (*inicio == NULL) {
        return 0;
    } else {
        pacote * tmp = *inicio;
        double tamanho = (*inicio)->tamanho;
        (*inicio) = (*inicio)->prox;
        free(tmp);
        return tamanho;
    }
}

double removerFila(fila ** inicio) {
    if (*inicio == NULL) {
        return 0;
    } else {
        fila * tmp = *inicio;
        double atraso = (*inicio)->atraso;
        (*inicio) = (*inicio)->prox;
        free(tmp);
        return atraso;
    }
}

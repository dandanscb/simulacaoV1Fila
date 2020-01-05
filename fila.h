/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fila.h
 * Author: Pedro Kiyuna e Daniel Escobar
 *
 * Created on 9 de Maio de 2019, 16:35
 */

#ifndef FILA_H
#define FILA_H

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#include "priorityQueue.h"

typedef struct pacote_ {
    double tamanho;
    struct pacote_ * prox;
} pacote;

typedef struct fila_ {
    double atraso;
    double chegada_pct;
    double tamanho_pct;
    double tempo_duracao;
    struct fila_ * prox;
} fila;

int inserir(pacote ** inicio, pacote ** fim, double tamanho);
double remover(pacote ** inicio);
pacote * aloca_pct();

int inserirFila(fila ** inicio, fila ** fim, DataNode data, double atraso);
double removerFila(fila ** inicio);
fila * aloca_fila();


#endif /* FILA_H */


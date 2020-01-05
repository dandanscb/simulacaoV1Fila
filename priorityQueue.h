/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   lista.h
 * Author: Pedro Kiyuna e Daniel Escobar
 *
 * Created on 29 de Maio de 2019, 18:42
 */

#ifndef LISTA_H
#define LISTA_H

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

typedef struct dataNode {
    int tipo;
    double chegada_pct;
    double tamanho_pct;
    double tempo_duracao;
} DataNode;

typedef struct node {
    DataNode data;
    double priority;
    struct node* prox;
} Node;

typedef struct priorityQueue {
    int size;
    Node* head;
} PriorityQueue;

PriorityQueue* createQueue();
void push(PriorityQueue* priorityQueue, DataNode data, double priority);
void addAtraso(PriorityQueue* priorityQueue, DataNode data, double priority);
int isNotEmpty(PriorityQueue* priorityQueue);
void pop(PriorityQueue* priorityQueue);
Node* indexNode(PriorityQueue* priorityQueue, int index);
int nodeIndex(PriorityQueue* priorityQueue, Node* node);
void popIndex(PriorityQueue* priorityQueue, int index);
void pushIndex(PriorityQueue* priorityQueue, DataNode data, int index);
void changeNode(PriorityQueue* priorityQueue, Node* nodeA, Node* nodeB);
Node* min(PriorityQueue* priorityQueue, int index);
void sort(PriorityQueue* priorityQueue);
void edit(PriorityQueue* priorityQueue, Node* node);
Node* newNode(DataNode data, double priority);


#endif /* LISTA_H */


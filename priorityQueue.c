/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   priorityQueue.c
 * Author: Pedro Kiyuna e Daniel Escobar
 *
 * Created on 29 de Maio de 2019, 18:42
 */

#include <stdio.h>
#include <stdlib.h>
#include"priorityQueue.h"

/*
 * 
 */

PriorityQueue* createQueue() {
    PriorityQueue* priorityQueue = (PriorityQueue*) malloc(sizeof (PriorityQueue));

    priorityQueue->size = 0;
    priorityQueue->head = NULL;

    return priorityQueue;
}

void push(PriorityQueue* priorityQueue, DataNode data, double priority) {
    Node* start = priorityQueue->head;

    if (!isNotEmpty(priorityQueue)) {
        Node* node = (Node*) malloc(sizeof (Node));

        node->data = data;
        node->priority = data.chegada_pct;
        node->prox = priorityQueue->head;
        priorityQueue->head = node;
    } else {
        // Create new Node 
        Node* temp = newNode(data, priority);

        if (priorityQueue->head->priority > priority) {

            // Insert New Node before head 
            temp->prox = priorityQueue->head;
            priorityQueue->head = temp;
        } else {

            // Traverse the list and find a 
            // position to insert new node 
            while (start->prox != NULL &&
                    start->prox->priority < priority) {
                start = start->prox;
            }

            // Either at the ends of the list 
            // or at required position 
            temp->prox = start->prox;
            start->prox = temp;
        }
    }

    priorityQueue->size++;
}

void addAtraso(PriorityQueue* priorityQueue, DataNode data, double priority) {
    Node* start = priorityQueue->head;

    if (!isNotEmpty(priorityQueue)) {
        Node* node = (Node*) malloc(sizeof (Node));

        node->data = data;
        node->priority = data.chegada_pct;
        node->prox = priorityQueue->head;
        priorityQueue->head = node;
    } else {
        // Create new Node 
        Node* temp = newNode(data, priority);

        if (priorityQueue->head->priority < priority) {

            // Insert New Node before head 
            temp->prox = priorityQueue->head;
            priorityQueue->head = temp;
        } else {

            // Traverse the list and find a 
            // position to insert new node 
            while (start->prox != NULL &&
                    start->prox->priority > priority) {
                start = start->prox;
            }

            // Either at the ends of the list 
            // or at required position 
            temp->prox = start->prox;
            start->prox = temp;
        }
    }

    priorityQueue->size++;
}

int isNotEmpty(PriorityQueue* priorityQueue) {
    return priorityQueue->size;
}

void pop(PriorityQueue* priorityQueue) {
    if (isNotEmpty(priorityQueue)) {
        Node* node = priorityQueue->head;

        priorityQueue->head = node->prox;
        free(node);
        priorityQueue->size--;
    }
}

Node* indexNode(PriorityQueue* priorityQueue, int index) {
    if (index >= 0 && index < priorityQueue->size) {
        Node* node = priorityQueue->head;
        for (int i = 0; i < index; i++)
            node = node->prox;
        return node;
    }

    return NULL;
}

int nodeIndex(PriorityQueue* priorityQueue, Node* node) {
    if (node != NULL) {
        Node* pointer = priorityQueue->head;
        int index = 0;

        while (pointer != node && pointer != NULL) {
            pointer = pointer->prox;
            index++;
        }

        if (pointer)
            return index;
    }

    printf("Nó nao pertencente a priority queue\n");
    return -1;
}

void popIndex(PriorityQueue* priorityQueue, int index) {
    if (index == 0)
        pop(priorityQueue);
    else {
        Node* atual = indexNode(priorityQueue, index);

        if (atual != NULL) {
            Node* anterior = indexNode(priorityQueue, index - 1);
            anterior->prox = atual->prox;

            free(atual);
            priorityQueue->size--;
        }
    }
}

void pushIndex(PriorityQueue* priorityQueue, DataNode data, int index) {
    if (index == 0)
        push(priorityQueue, data, data.chegada_pct);
    else {
        Node* atual = indexNode(priorityQueue, index);
        if (atual != NULL) {
            Node* anterior = indexNode(priorityQueue, index - 1);

            Node* novoNode = (Node*) malloc(sizeof (Node));
            novoNode->data = data;

            anterior->prox = novoNode;
            novoNode->prox = atual;
            priorityQueue->size++;
        }

    }

}

void changeNode(PriorityQueue* priorityQueue, Node* nodeA, Node* nodeB) {

    if (nodeA == nodeB) {
        return;
    }

    int indexA = nodeIndex(priorityQueue, nodeA);
    int indexB = nodeIndex(priorityQueue, nodeB);

    if (indexA > indexB) {
        nodeA = nodeB;
        nodeB = indexNode(priorityQueue, indexA);

        indexA = indexB;
        indexB = nodeIndex(priorityQueue, nodeB);
    }

    Node* pb = indexNode(priorityQueue, indexB - 1);

    if (nodeA == priorityQueue->head) {
        priorityQueue->head = nodeB;
    } else {
        Node* pa = indexNode(priorityQueue, indexA - 1);
        pa->prox = nodeB;
    }

    pb->prox = nodeA;


    Node* pointer = nodeA->prox;
    nodeA->prox = nodeB->prox;
    nodeB->prox = pointer;

}

Node* min(PriorityQueue* priorityQueue, int index) {
    Node* pointer = indexNode(priorityQueue, index);

    if (pointer != NULL) {
        Node* minNode = pointer;

        while (pointer != NULL) {
            if (pointer->data.chegada_pct < minNode->data.chegada_pct) {
                minNode = pointer;
            }
            pointer = pointer->prox;
        }

        return minNode;
    }
}

void sort(PriorityQueue* priorityQueue) {
    for (int i = 0; i < priorityQueue->size - 1; i++) {
        changeNode(priorityQueue, indexNode(priorityQueue, i), min(priorityQueue, i));
    }
}

void edit(PriorityQueue* priorityQueue, Node* node) {
    /*Node* pointer = indexNode(priorityQueue, index);
    
    pointer->data.chegada_pct = node->data.chegada_pct;
    pointer->data.tamanho_pct = node->data.tamanho_pct;
    pointer->data.tempo_duracao = node->data.tempo_duracao;
    pointer->data.tipo = node->data.tipo;*/


    if (node->data.chegada_pct > priorityQueue->head->prox->data.chegada_pct) {
        Node* head = priorityQueue->head;

        priorityQueue->head = head->prox;
        free(head);

        Node* pointer = (Node*) malloc(sizeof (Node));

        pointer->data.chegada_pct = node->data.chegada_pct;
        pointer->data.tamanho_pct = node->data.tamanho_pct;
        pointer->data.tempo_duracao = node->data.tempo_duracao;
        pointer->data.tipo = node->data.tipo;

        pointer->priority = node->data.chegada_pct;
        pointer->prox = priorityQueue->head->prox;
        priorityQueue->head->prox = pointer;



    } else {
        priorityQueue->head->data.chegada_pct = node->data.chegada_pct;
        priorityQueue->head->data.tamanho_pct = node->data.tamanho_pct;
        priorityQueue->head->data.tempo_duracao = node->data.tempo_duracao;
        priorityQueue->head->data.tipo = node->data.tipo;
    }


}

Node* newNode(DataNode data, double priority) {
    Node* temp = (Node*) malloc(sizeof (Node));
    temp->data = data;
    temp->priority = priority;
    temp->prox = NULL;

    return temp;
}



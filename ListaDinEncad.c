#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListaDinEncad.h"  //inclui os Protótipos

Lista* cria_lista()
{
    Lista* li = (Lista*) malloc(sizeof(Lista));
    if(li != NULL)
        *li = NULL;
    return li;
}

void libera_lista(Lista* li)
{
    if(li != NULL){
        Elem* no;
        while((*li) != NULL){
            no = *li;
            *li = (*li)->prox;
            free(no);
        }
        free(li);
    }
}


Elem* consulta_lista_nome(Lista* li, char str[], Elem **el)
{
    if(li == NULL)
        return ERRO;
    Elem *no = *li;
    while(no != NULL && strcmp(no->descricao, str)!= 0){
        no = no->prox;
    }
    if(no == NULL)
        return no;
    else{
        *el = no;
        return no;
    }
}



int insere_lista_ordenada(Lista* li, int dia, int mes, int ano, char* str)
{

    if(li == NULL)
        return ERRO;
    Elem *no = (Elem*) malloc(sizeof(Elem));
    if(no == NULL)
        return ERRO;
    no->dia = dia;
    no->mes = mes;
    no->ano = ano;
    strcpy(no->descricao, str);

    if((*li) == NULL){ //lista vazia: insere início
        no->prox = NULL;
        *li = no;
        return OK;
    }
    else{
        Elem *ant, *atual = *li;
        int fim = 0;
        while(atual != NULL && !fim){
            if(atual->ano <= ano)
               {
                   if(atual->mes <= mes)
                   {
                       if(atual->dia < dia)
                       {
                            ant = atual;
                            atual = atual->prox;
                       }
                       else
                       {
                           fim = 1;
                       }
                   }
                   else
                   {
                       fim = 1;
                   }
               }
               else
               {
                   fim = 1;
               }
        }
        if(atual == *li){ //insere início
            no->prox = (*li);
            *li = no;
        }else{
            no->prox = atual;
            ant->prox = no;
        }
        return OK;
    }
}

int remove_lista(Lista* li, int dia, int mes, int ano, char* str)
{
    if(li == NULL)
        return ERRO;
    if((*li) == NULL)//lista vazia
        return ERRO;
    Elem *ant, *no = *li;
    while(no != NULL && no->ano != ano && no->mes != mes && no->dia != dia && strcmp(no->descricao, str)!= 0){
        ant = no;
        no = no->prox;
    }
    if(no == NULL) //não encontrado
    {
        printf("Evento nao encontrado!");
        return ERRO;
    }

    if(no == *li) //remover o primeiro?
        *li = no->prox;
    else
        ant->prox = no->prox;
    free(no);
    return OK;
}


int tamanho_lista(Lista* li)
{
    if(li == NULL)
        return 0;
    int cont = 0;
    Elem* no = *li;
    while(no != NULL){
        cont++;
        no = no->prox;
    }
    return cont;
}

int lista_vazia(Lista* li)
{
    if(li == NULL)
        return 1;
    if(*li == NULL)
        return 1;
    return 0;
}

void imprime_lista(Lista* li)
{
    if(li == NULL)
        return;
    Elem* no = *li;
    printf("\nLista de eventos cadastrados:\n");
    while(no != NULL){
        printf("Evento: %s  Data: %d/%d/%d\n", no->descricao, no->dia, no->mes, no->ano);
        no = no->prox;
    }
}

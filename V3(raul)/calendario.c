#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calendario.h"

Lista* li;

int verBissesto(int ano);
//Confirmar se o algoritmo de inserir ordenado esta correto.
void inicio()
{
    //Teste Lista:
    li = cria_lista();

    printf("-----Calendario Iniciado-----\n\n");
}

void fim() {
  libera_lista(li);

  printf("-----Calendario Finalizado-----\n\n");
}
//OBS: Essa funcao cadastro � um misto da funcao cadastro do cliente e do servidor.
int cadastro(char *evento, char *data)// A funcao cadastro do cliente deve mandar um send com as informacoes. Seria legal criar uma struct para isso.
{
    int dia = -1, mes = -1, ano = -1;
    char *token;
    char corte[2] = "/";
    int i = 0;
    event test;

    token = strtok(data, corte);

    while (token != NULL) {
      switch (i) {
        case 0:
          dia = atoi(token);
          i++;
          break;
        case 1:
          mes = atoi(token);
          i++;
          break;
        case 2:
          ano = atoi(token);
          i++;
          break;
        case 3:
          dia = -1;
          break;
      }

      token = strtok(NULL, corte);
    }

    if (dia == -1 || mes == -1 || ano == -1) {
      return 0;
    }

    if (mes < 1 || mes > 12 || dia < 1 || dia > 31) {
      return 0;
    }
    else {
      if (mes == 2 || mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        if (dia  > 30) {
          return 0;
        }
        if (mes == 2) {
          if (verBissesto(ano)) {
            if (dia > 29) {
              return 0;
            }
          }
          else {
            if (dia > 28) {
              return 0;
            }
          }
        }
      }
    }

    test = consulta(evento);

    if (test.dia != -1) {
      return -1;
    }

    insere_lista_ordenada(li, dia, mes, ano, evento);

    return 1;
}

listt imprime() {
    listt ev;
    Elem* no;
    int i = 0;

    ev.events = 0;
    ev.erro = 0;

    if(li == NULL)
        return ev;

    no = *li;

    while(no != NULL){
        ev.events++;
        no = no->prox;
    }

    ev.desc = (char **) malloc(ev.events * sizeof(char *));

    if (ev.desc == NULL) {
      ev.erro = 1;
      return ev;
    }

    no = *li;

    while(no != NULL){
        ev.desc[i] = (char *) malloc(100 * sizeof(char));
        if (ev.desc[i] == NULL) {
          ev.erro = 2;
          ev.events = i;
          return ev;
        }
        strcpy(ev.desc[i++], no->descricao);
        no = no->prox;
    }

    for (int i = 0; i < ev.events; i++) {
      printf("-%s\n", ev.desc[i]);
    }

    return ev;
}

event consulta (char *evento)
{
    Elem* e = NULL;
    event novo;
    consulta_lista_nome(li, evento, &e);

    if (e != NULL){
      novo.dia = e->dia;
      novo.mes = e->mes;
      novo.ano = e->ano;
      strcpy(novo.descricao, evento);
    }
    else{
      novo.dia = -1;
    }

    return novo;
}

int verBissesto(int ano) {
  if (ano % 400 == 0) {
    return 1;
  }

  if (ano % 4 == 0 && ano % 100 != 0) {
    return 1;
  }

  return 0;
}

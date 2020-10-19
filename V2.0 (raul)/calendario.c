#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calendario.h"

Lista* li;
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
      printf("%d/%d/%d %s\n", dia, mes, ano, evento);
      return 0;
    }

    insere_lista_ordenada(li, dia, mes, ano, evento);
    //imprime_lista(li);

    return 1;
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

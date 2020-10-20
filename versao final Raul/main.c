#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListaDinEncad.h"

int cadastro(Lista* li);
int consulta (Lista* li);
//Confirmar se o algoritmo de inserir ordenado esta correto.
int main()
{
    //Teste Lista:
    Lista* li = cria_lista();
    insere_lista_ordenada(li, 10, 02, 2022, "abcde");
    insere_lista_ordenada(li, 10, 02, 2020, "Reuniao Redes");
     insere_lista_ordenada(li, 15, 03, 2020, "Reuniao Redes");
    imprime_lista(li);
    remove_lista(li, 10, 02, 2020, "abcde");
    imprime_lista(li);

    //Parte do cliente:
    int funcao;
    printf("Voce deseja:\n\(1) Criar um novo evento para a empresa\n(2) Consultar algum evento especifico\n");
    scanf("%d", &funcao);
    if(funcao == 1)
    {
        cadastro(li);//Aqui estou colocando a lista, mas na aplicacao isso nao deve ocorrer, pois a lista estaria no servidor e nao no cliente.
    }
    if(funcao == 2)
    {
        consulta(li);
    }

    imprime_lista(li);


    libera_lista(li);
    return 0;
}
//OBS: Essa funcao cadastro � um misto da funcao cadastro do cliente e do servidor.
int cadastro(Lista* li)// A funcao cadastro do cliente deve mandar um send com as informacoes. Seria legal criar uma struct para isso.
{
    int dia, mes, ano;
    char evento[100];

    printf("Digite a descricao do evento:\n");
    //scanf("%s\n", evento);
    getchar();
    fgets(evento, 100, stdin);
    evento[strlen(evento) - 1] = '\0';
    printf("digite Dia/mes/ano\n");
    scanf("%d%*c%d%*c%d", &dia, &mes, &ano);
    insere_lista_ordenada(li, dia, mes, ano, evento);
    imprime_lista(li);
    return 0;

}

int consulta (Lista* li)
{
    printf("\nDigite O nome do evento que voce quer buscar:\n");
    char evento[100];
    getchar();
    fgets(evento, 100, stdin);
    evento[strlen(evento) - 1] = '\0';
    Elem* e = NULL;
    consulta_lista_nome(li, evento, &e);

    if (e != NULL)
      printf("\nO evento: %s esta marcado para: %d/%d/%d\n", evento, e->dia, e->mes, e->ano);
    else
      printf("\nEvento nao encontrado!\n");
    return 0;
}

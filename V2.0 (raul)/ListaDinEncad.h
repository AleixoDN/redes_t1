//Arquivo ListaDinEncad.h

#define FALSO      0
#define VERDADEIRO 1

#define OK         1
#define ERRO       0



//Definição do tipo lista
struct elemento{
    int dia;
    int mes;
    int ano;
    char descricao[100];
    struct elemento *prox;
};

typedef struct elemento Elem;

typedef struct elemento* Lista;

Lista* cria_lista();
void libera_lista(Lista* li);
int insere_lista_ordenada(Lista* li, int dia, int mes, int ano, char* str);
int remove_lista(Lista* li, int dia, int mes, int ano, char* str);
int tamanho_lista(Lista* li);
int lista_vazia(Lista* li);
void imprime_lista(Lista* li);
//Elem* consulta_lista_dado(Lista* li, int dia, int mes, int ano,char* str, Elem **el);
Elem* consulta_lista_nome(Lista* li, char str[], Elem **el);

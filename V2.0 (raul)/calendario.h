#include "ListaDinEncad.h"

typedef struct e{
    int dia;
    int mes;
    int ano;
    char descricao[100];
} event;

int cadastro(char *evento, char *data);
event consulta (char *evento);
void inicio();
void fim();

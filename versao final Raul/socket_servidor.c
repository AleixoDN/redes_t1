#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#include "calendario.h"

#define TRUE 1
#define FALSE 0
#define PORT 8000

char * itoa(int value, int base);

int main() {

	int opt = TRUE;
	int sock_servidor;
	int addrlen;
	int novo_cliente;
	int sock_cliente[30];
	int max_clientes = 30;
	int activity;
	int i;
	int valread;
	int sd;
	int max_sd;
	char buffer[1025];

	fd_set readfds;


	char *message = "Servidor Teste Trabalho Redes\n\tDigite -help para mais informacoes\n";
	char *informacoes = "Comandos\n\t-insere {descricao} - {dia}/{mes}/{ano}\nAdiciona um evento\n\t-consulta {descricao}\nConsulta um evento\n\t-listar\nLista todos os eventos salvos\n\t-remove {descricao}\nRemove um evento\n\t-editar {descricao} - {nova descricao} - {dia}/{mes}/{ano}\nEdita o evento para a nova data e descricao";


	inicio();

	// Inicializa os servidores para 0 (não verificado)
	for (i = 0; i < max_clientes; i++)
	{
		sock_cliente[i] = 0;
	}

	// Cria o socket do servidor e verifica erros
	if((sock_servidor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("ERRO. Não foi possível criar o socket.\n");
        return 1;
    }

    /*
        AF_INET = Indica que o socket é para endereços do tipo IPV4.
        SOCK_STREAM = Usado para indicar que o socket o protocolo utilizado é orientado a conexao.
        IPPROTO_TCP = Usado para indicar que o socket é um protocolo TCP.
        A funcao socket() retorna -1 se falhar
    */

    // Define os parametros de endereçamento do servidor no padrão IPv4
    struct sockaddr_in addr;                            // Struct com os dados no padrão
    addr.sin_family      = AF_INET;                     // Define tipo de endereçamento como IPv4
    addr.sin_port        = htons(PORT);                 // Define a porta do servidor
    addr.sin_addr.s_addr = INADDR_ANY;  	            // Define o IP como o ip do computador
    memset(&addr.sin_zero,0,sizeof(addr.sin_zero));     // Set de zeros para uso do sistema

    /*
    sockaddr_in: é uma struct utilizada para guardar informaçoes sobre os sockets.
    memset() garante que informacoes da struct que nao serao utilizadas contenham o valor 0.
    AF_INET indica que o socket contém um endereço IPV4
    a funcao htons() garante que o numero da porta esta no formatado de maneira adequada.
    */

	// Adere o valor da porta a ser usada à porta do computador, e verifica sucesso
	if(bind(sock_servidor, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        printf("\n ERRO. Socket não atrelado à porta.\n");
        return 1;
    }
	printf("Utilizando porta %d para conexao\n", PORT);

	// Define como socket que vai esperar um cliente a se conector
	if (listen(sock_servidor, 5) == -1) {
        printf("\n ERRO. Socket não definido como servidor.\n");
        return 1;
    }

	addrlen = sizeof(addr);
	puts("Esperando conexoes...");

	// O server entrará em loop para o gerenciamento de threads e clientes
	while(TRUE)
	{
		// Inicia o conjunto de clientes
		FD_ZERO(&readfds);

		// Adiciona o socket servidor ao conjunto de clientes
		FD_SET(sock_servidor, &readfds);
		max_sd = sock_servidor;

		// Adiciona os clientes que se conectarem ao conjunto de sockets
		for ( i = 0 ; i < max_clientes ; i++) {

			// Adiciona os cleintes não adicionados ao conjunto de sockets
			sd = sock_cliente[i];
			if(sd > 0)
				FD_SET( sd , &readfds);

			if(sd > max_sd)
				max_sd = sd;
		}

		// Espera indefinidamente até ter uma atividade em algum sócket do conjunto
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
		if (activity == -1) {
			printf("ERRO. Problema na seleção de clientes.");
		}

		// Verifica atividades no servidor, nesse caso a atividade será de novos clientes se connectando
		if (FD_ISSET(sock_servidor, &readfds)) {

			if ((novo_cliente = accept(sock_servidor, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) == -1) {
		        printf("\n ERRO. Socket não definido como servidor.\n");
				return 1;
			}

			printf("Nova conexao - Socket %d, ip: %s, porta: %d \n", novo_cliente, inet_ntoa(addr.sin_addr) , ntohs (addr.sin_port));

			// Mensagem de confirmação
			if( send(novo_cliente, message, strlen(message), 0) != strlen(message) ) {
				perror("ERRO. Não foi possível enviar mensagem de confirmação.");
			}

			// Adiciona o  novo cliente ao conjunto
			for (i = 0; i < max_clientes; i++) {

				if( sock_cliente[i] == 0 ) {
					sock_cliente[i] = novo_cliente;
					printf("Adicionado a lista de clientes como %d\n", i);

					break;

				}
			}
		}

		// Se não for uma atividade no servidor, é uma atividade em um dos clientes
		for (i = 0; i < max_clientes; i++)
		{

			// Roda cliente por cliente
			sd = sock_cliente[i];
			if (FD_ISSET( sd , &readfds)) {

				// Verifica se a atividade foi de desconexão
				if ((valread = read( sd , buffer, 1024)) == 0) {

					// Colhe a info do cliente que encerrou e encerra as threads dele do lado do servidor
					getpeername(sd , (struct sockaddr*)&addr , (socklen_t*)&addrlen);
					printf("Cliente %d desconectado, ip: %s, porta: %d\n", i ,inet_ntoa(addr.sin_addr) , ntohs(addr.sin_port));
					close( sd );
					sock_cliente[i] = 0;
				}

				// Se a ativiadade não foi de desconexão, ela foi de envio de mensagem
				else {

					buffer[valread] = '\0';
					printf("Mensagem %s recebida de usuário %d\n", buffer, i);

					if (!strcmp(buffer, "-help")) {
						send(sd, informacoes, strlen(informacoes), 0);
					}
					else {
						char *token;
						char evento[100];
						char corte[2] = " ";
						char backup[1025];
						int i = 0;

						strcpy(backup, buffer);

						token = strtok(buffer, corte);

						while (token != NULL) {
							if (!strcmp(token, "-consulta")) {
								i = 1;
							}
							else if (!strcmp(token, "-insere")) {
								i = 2;
							}
							else if (!strcmp(token, "-listar")) {
								i = 5;
							}
							else if (!strcmp(token, "-remove")) {
								i = 6;
							}
							else if (!strcmp(token, "-editar")) {
								i = 7;
							}
							else {
								char *msg = "Opcao invalida!\n";

								send(sd, msg, strlen(msg), 0);
							}

							break;
						}

						if (i == 1) {
							char *tt = strchr(backup, ' ');
							tt++;

							event novo = consulta(tt);

							char msg[500];

							if (novo.dia == -1) {
								strcpy(msg, "Evento nao encontrado!");
							}
							else {
								strcpy(msg, "O evento '");
								strcat(msg, novo.descricao);
								strcat(msg, "' esta marcado para ");
								strcat(msg, itoa(novo.dia, 10));
								strcat(msg, "/");
								strcat(msg, itoa(novo.mes, 10));
								strcat(msg, "/");
								strcat(msg, itoa(novo.ano, 10));
							}

							send(sd, msg, strlen(msg), 0);
						} // consultar

						if (i == 2) {
							strcpy(corte, "-");

							short passou = 0;

							char *tt = strchr(backup, ' ');
							tt++;

							token = strtok(tt, corte);

							while (token != NULL) {
								if (i == 2) {
									strcpy(evento, token);
									evento[strlen(evento) - 1] = '\0';
									i++;
								}
								else {
									char *msg;

									int c = cadastro(evento, token);

									if (c == 1) {
										msg = "Cadastro Efeituado!";
									}
									else {
										if (c == -1) {
											msg = "Nome do evento ja existente!";
										}
										else {
											msg = "Erro na realizacao do cadastro!";
										}
									}

									send(sd, msg, strlen(msg), 0);

									passou = 1;

									break;
								}

								token = strtok(NULL, corte);
							}

							if (!passou) {
								char *msg = "Escrita errada para cadastro";

								send(sd, msg, strlen(msg), 0);
							}
						} // inserir

						if (i == 5) {
							listt ev;
							char msg[10000];

							ev = imprime();

							switch (ev.erro) {
								case 0:
									printf("Caso 0\n");

									if (ev.events == 0) {
										strcpy(msg, "Nenhum evento registrado");
									}
									else {
										strcpy(msg, "Eventos:");

										printf("%d\n", ev.events);

										for (int i = 0; i < ev.events; i++) {
											printf("%s\n", ev.desc[i]);
											strcat(msg, "\n");
											strcat(msg, ev.desc[i]);
											free(ev.desc[i]);
										}

										free(ev.desc);
									}

									break;
								case 1:
									printf("Caso 1\n");
									strcpy(msg, "Erro na Listagem dos eventos");
									break;
								case 2:
									printf("Caso 2\n");
									strcpy(msg, "Erro na Listagem dos eventos");

									for (int i = 0; i < ev.events; i++) {
										free(ev.desc[i]);
									}

									free(ev.desc);
									break;
							}

							send(sd, msg, strlen(msg), 0);
						} // listar

						if (i == 6) {
							char *tt = strchr(backup, ' ');
							tt++;

							int ver = remover(tt);

							printf("%d\n", ver);

							char msg[500];

							if (ver == 0) {
								strcpy(msg, "Evento nao encontrado!");
							}
							else if (ver == -1) {
								strcpy(msg, "Erro na remocao do evento");
							}
							else {
								strcpy(msg, "O evento '");
								strcat(msg, tt);
								strcat(msg, "' foi removido com sucesso");
							}

							send(sd, msg, strlen(msg), 0);
						} // remover

						if (i == 7) {
							strcpy(corte, "-");

							short passou = 0;
							char *novoEvento = (char *) malloc(100 * sizeof(char));

							if (novoEvento == NULL) {
								char *msg = "Erro na edicao do evento";
								send(sd, msg, strlen(msg), 0);
							}
							else {

								char *tt = strchr(backup, ' ');
								tt++;

								token = strtok(tt, corte);

								while (token != NULL) {
									if (i == 7) {
										strcpy(evento, token);
										evento[strlen(evento) - 1] = '\0';
										i++;
									}
									else if (i == 8) {
										strcpy(novoEvento, token);
										novoEvento++;
										novoEvento[strlen(novoEvento) - 1] = '\0';
										i++;
									}
									else {
										char *msg;

										int c = edicao(evento, novoEvento, token);

										switch (c) {
											case 1:
												msg = "Edicao Efeituada!";
												break;
											case 0:
												msg = "Erro na nova data do evento";
												break;
											case -1:
												msg = "Evento nao encontrado";
												break;
											case -2:
												msg = "Nova descricao de evento ja registrada";
												break;
										}

										send(sd, msg, strlen(msg), 0);

										passou = 1;

										break;
									}

									token = strtok(NULL, corte);
								}

								if (!passou) {
									char *msg = "Escrita errada para cadastro";

									send(sd, msg, strlen(msg), 0);
								}
							}
						} // editar

						printf("Mensagem Enviada\n");
					}
				}
			}
		}
	}

	fim();

	return 0;
}


char * itoa(int val, int base) {
	static char buf[32] = {0};

	int i = 30;

	for (; val && i ; --i, val /= base) {
		buf[i] = "0123456789abcdef"[val % base];
	}

	return &buf[i + 1];
}

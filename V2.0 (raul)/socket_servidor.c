//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "calendario.h"

#define TRUE 1
#define FALSE 0
#define PORT 8000

char * itoa(int value, int base);

int main(int argc , char *argv[])
{
	inicio();

	int opt = TRUE;
	int master_socket , addrlen , new_socket , client_socket[30] ,
		max_clients = 30 , activity, i , valread , sd;
	int max_sd;
	struct sockaddr_in address;

	char buffer[1025]; //data buffer of 1K

	//set of socket descriptors
	fd_set readfds;

	//a message
	char *message = "Servidor Teste Trabalho Redes\n\tDigite -help para mais informacoes\n";
	char *informacoes = "Comandos\n\t-insere {descricao} - {dia}/{mes}/{ano}\nAdiciona um evento\n\t-consulta {descricao}\nConsulta um evento";

	//initialise all client_socket[] to 0 so not checked
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}

	//create a master socket
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("Erro de Socket");
		exit(EXIT_FAILURE);
	}

	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	//bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("Erro no Bind");
		exit(EXIT_FAILURE);
	}
	printf("Utilizando porta %d para conexao\n", PORT);

	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("Erro Listen");
		exit(EXIT_FAILURE);
	}

	//accept the incoming connection
	addrlen = sizeof(address);
	puts("Esperando conexoes...");

	while(TRUE)
	{
		//clear the socket set
		FD_ZERO(&readfds);

		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		//add child sockets to set
		for ( i = 0 ; i < max_clients ; i++)
		{
			//socket descriptor
			sd = client_socket[i];

			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);

			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}

		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

		if ((activity < 0) && (errno!=EINTR))
		{
			printf("Erro de selecao");
		}

		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket,
					(struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
			{
				perror("Erro de Acept");
				exit(EXIT_FAILURE);
			}

			//inform user of socket number - used in send and receive commands
			//printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs (address.sin_port));
			printf("Nova conexao - Socket %d, ip: %s, porta: %d \n", new_socket, inet_ntoa(address.sin_addr) , ntohs (address.sin_port));

			//send new connection greeting message
			if( send(new_socket, message, strlen(message), 0) != strlen(message) )
			{
				perror("Erro envio");
			}

			//add new socket to array of sockets
			for (i = 0; i < max_clients; i++)
			{
				//if position is empty
				if( client_socket[i] == 0 )
				{
					client_socket[i] = new_socket;
					printf("Adicionado a lista de clientes como %d\n", i);

					break;
				}
			}
		}

		//else its some IO operation on some other socket
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];

			if (FD_ISSET( sd , &readfds))
			{
				//Check if it was for closing , and also read the
				//incoming message
				if ((valread = read( sd , buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
					//printf("Host disconnected , ip %s , port %d \n" ,inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
					printf("Cliente %d desconectado, ip: %s, porta: %d\n", i ,inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

					//Close the socket and mark as 0 in list for reuse
					close( sd );
					client_socket[i] = 0;
				}

				//Echo back the message that came in
				else
				{
					//set the string terminating NULL byte on the end
					//of the data read
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
						}

						if (i == 2) {
							strcpy(corte, "-");

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

											if (cadastro(evento, token)) {
												msg = "Cadastro Efeituado!";
											}
											else {
												msg = "Erro na realizacao do cadastro!";
											}

											send(sd, msg, strlen(msg), 0);

											break;
										}

								token = strtok(NULL, corte);
							}
						}

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

#include <stdio.h>
#incldue <stdlib.h>
//#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main()
{
    //Dados do servidor.
    char* servidorIP;
    in_port_t servidorPorta;


    //Criacao do socket.
    int servSock;
    if((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))< 0)
    {
        printf("Erro na criacao do socket");
        return 0;
    }

    //criando a struct
    struct sockaddr_in servidorAddr;                    //Struct que vai guardar o endereço do servidor
    memset(&servidorAddr, 0, sizeof(servidorAddr));     //zerando a struct.
    servidorAddr.sin_family = AF_INET;                  //Definindo IP do tipo IPV4.
    servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);   //Definindo o endereço do servidor.
    servidorAddr.sin_port = htons(servidorPorta);       //Porta do servidor.

    //Bind
    if(bind(servSock, (struct sockaddr*) &servidorAddr, sizeof(servidorAddr)) < 0)
    {
        printf("Erro na funcao bind()");
        return 0;
    }

    //Esperando pedido de conexao
    if(listen(servsock, 1) < 0)
    {
        printf("Erro na funcao listen()");
        return;
    }

    //Desenvolvendo a conexao: (OBS: a partir daqui podemos comecar um laco para fazer a troca de informacoes.)

        //Criando uma estrutura para armazenar os dados do socket cliente:
    struct sockaddr_in clientAddr;
    socklen_t tamanho = sizeof(clientAddr);

        //Aceitando a conexao:
    int clientSock = accept(servSock, (struct sockaddr*) &clientAddr, &tamanho);
    if(clientSock < 0)
    {
        printf("Erro na funcao accept()");
        return 0;
    }

        //Com isso, a conexao esta feita. Podemos usar send() e recv()


        //Fechando sockets
        close(servsock);
        close(clientSock);


}

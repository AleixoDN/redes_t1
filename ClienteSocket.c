#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
#include <sys/types.h> //Necessaria
#include <sys/socket.h> //Necessaria
#include <netinet/in.h> //Necessaria
#include <arpa/inet.h> //Necessaria

int main()
{
    //Recebendo informaçoes importantes:
        //Podemor tanto receber quando definirmos nosso proprio valor.
    char* servidorIP; // A string inserida no caso do IPV4 é 4 octetos separados por pontos '.'
    in_port_t servidorPorta; // Numero da porta

    //Criando o socket TCP:
    int sock = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);// Criacao do socket cliente.
    if(sock < 0)
    {
        DieWithSystemMessage("Erro na criacao do socket");
    }
    /*
        AF_INET =
        SOCK_STREAM =
        IPPROTO_TCP =
        A funcao socket() ...
        DiewithMessage() é uma funcao para tratamento de erros.
    */

    //Criando a struct que vai guardar informaçoes do servidor:
    struct sockaddr_in servidorAddr; //Endereço do servidor
    memset(&servidorAddr, 0, sizeof(servidorAddr)); //Zerando a string.
    servidorAddr.sin_family = AF_INET; //Declarando que é da familia IPV4
    servidorAddr.sin_addr.s_addr = inet_addr(servidorIP)// Definindo o endereço do servidor
    servidoAddr.sin_port = htons(servidorPorta); //declarando a porta do servidor

    /*
    Explicando funcoes:

    */

    //Estabelecendo conexao com o servidor:
    if(connect(sock, (Struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0)
    {
        DieWithSystemMessage("Erro de conexao");
    }

    /*
        PQ temos que fazer o cast:
    */

    //Com a conexao estabelecida, temos que realizar nossa aplicacao.
    //Gostaria que essa aplicacao fosse modularizada e chamassemos sua funcao.
    /*Agora temos que usar as funcoes send() e rcv() para a troca de mensagens
        Prototipo send():
            int bytesEnviados = send(sock, String, TamanhoString, 0);
            NAO ESQUEÇA DE TRATAR OS ERROS.
        Prototipo recv():
            int bytesRecebidos = rcv(sock, StringBuffer, BUFSIZE - 1, 0);
        OBS: Podemos ter um parametro se tudo ocorreu bem analizando o valor de bytes enviados e recebidos.
        OBS2: Temos que por '\0' no final da string que ira receber a mensagem(StringBuffer).
            Basta fazermos StringBuffer[BUFSIZE] = '\0';
    */

    //Por fim, fechamos o socket:
    close(sock);
    return 0;

}

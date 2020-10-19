#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        printf("\nErro na criacao do socket\n");
        exit(1);
    }
    /*
        AF_INET = Indica que o socket é para endereços do tipo IPV4.
        SOCK_STREAM = Usado para indicar que o socket o protocolo utilizado é orientado a conexao.
        IPPROTO_TCP = Usado para indicar que o socket é um protocolo TCP.
        A funcao socket() retorna -1 se falhar
    */

    //Criando a struct que vai guardar informaçoes do servidor:
    struct sockaddr_in servidorAddr; //Endereço do servidor
    memset(&servidorAddr, 0, sizeof(servidorAddr)); //Zerando a string.
    servidorAddr.sin_family = AF_INET; //Declarando que é da familia IPV4
    servidorAddr.sin_addr.s_addr = inet_addr(servidorIP);// Definindo o endereço do servidor
    servidorAddr.sin_port = htons(servidorPorta); //declarando a porta do servidor

    /*
    sockaddr_in: é uma struct utilizada para guardar informaçoes sobre os sockets.
    memset() garante que informacoes da struct que nao serao utilizadas contenham o valor 0.
    AF_INET indica que o socket contém um endereço IPV4
    a funcao htons() garante que o numero da porta esta no formatado de maneira adequada.

    */

    //Estabelecendo conexao com o servidor:
    if(connect(sock, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0)
    {
      printf("\nErro ao conectar o socket com o servidor\n");
      exit(1);
    }

    /*
        PQ temos que fazer o cast: O cast tem que ser feito pq a API do socket espera que enviemos um socket genérico
        para ela, porém, estamos trabalhando com um socket IPV4. Portanto, o cast é necessario  para tratarmos nosso
        socket no formato generico.
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
    char* str = "abcd";
    int tamStr = strlen(str);
    int bytesEnviados = send(sock, str, tamStr, 0); // Envia string.
    if(bytesEnviados != tamStr)
    {
      printf("\nErro ao enviar string\n");
      return 0;
    }
    char strRecebida[5];
    int bytesRecebidos = 0;
    while(bytesRecebidos < tamStr)
    {
       bytesRecebidos += rcv(sock, strRecebida, 4, 0);
       strRecebida[4] = '\0';
       fputs(strRecebida, stdout);
    }
    //Por fim, fechamos o socket:
    close(sock);
    return 0;

}

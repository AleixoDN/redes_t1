#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>


int    sock_cliente;
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;

void *sendmessage();
void *listener();
int done=1;


int main() {

    // Cria o socket e verifica erros
    int sock_server;
    if((sock_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("\n ERRO. Socket não criado.\n");
        return 1;
    }

    /*
        AF_INET = Indica que o socket é para endereços do tipo IPV4.
        SOCK_STREAM = Usado para indicar que o socket o protocolo utilizado é orientado a conexao.
        IPPROTO_TCP = Usado para indicar que o socket é um protocolo TCP.
        A funcao socket() retorna -1 se falhar
    */

    // Define os parametros de endereçamento do servidor no padrão IPv4
    struct sockaddr_in servidorAddr;                                    // Struct com os dados no padrão
    servidorAddr.sin_family      = AF_INET;                             // Define tipo de endereçamento como IPv4
    servidorAddr.sin_port        = htons(8888);                         // Define a porta do servidor
    servidorAddr.sin_addr.s_addr = inet_addr("10.0.2.15"); //htons(INADDR_ANY);                   // Define o IP como o ip do computador
    memset(&servidorAddr.sin_zero, 0, sizeof(servidorAddr.sin_zero));   // Set de zeros para uso do sistema

    /*
    sockaddr_in: é uma struct utilizada para guardar informaçoes sobre os sockets.
    memset() garante que informacoes da struct que nao serao utilizadas contenham o valor 0.
    AF_INET indica que o socket contém um endereço IPV4
    a funcao htons() garante que o numero da porta esta no formatado de maneira adequada.
    */

    // Define os parametros dos clientes
    struct sockaddr_in clientAddr;
    socklen_t tamanho = sizeof(clientAddr);

    // Adere o valor da porta a ser usada à porta do computador, e verifica sucesso
    if(bind(sock_server, (struct sockaddr*) &servidorAddr, sizeof(servidorAddr)) == -1) {
        printf("\n ERRO. Socket não atrelado à porta.\n");
        return 1;
    }

    // Define como socket que vai esperar um cliente a se conector
    if(listen(sock_server,2) == -1) {
        printf("\n ERRO. Socket não definido como servidor.\n");
        return 1;
    }

    // Espera para que um cliente tente acessar o servidor
    printf("\nAguardando cliente...\n");
    if((sock_cliente = accept(sock_server, (struct sockaddr*) &clientAddr, &tamanho)) == -1) {
        printf("\nERRO. Um cliente não pode se conectar.\n");
        return 1;
    }
    printf("\nCliente conectado!\n");

    // Define-se duas threads
    pthread_t threads[2];
    void *status;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], &attr, sendmessage, NULL);
    pthread_create(&threads[1], &attr, listener, NULL);

    while(done){}

    return 0;

}

void *sendmessage() {
    int  enviados;
    char mensagem[256];

    do{
        printf("Server: ");
        fgets(mensagem,256,stdin);
        mensagem[strlen(mensagem)-1] = '\0';
        enviados = send(sock_cliente,mensagem,strlen(mensagem),0);
    } while(strcmp(mensagem,"exit")!=0);
}

void *listener() {
    int recebidos;
    char resposta[256];
    do{
        recebidos = recv(sock_cliente,resposta,256,0);              /* Recebe mensagem do cliente */
        resposta[recebidos] = '\0';
        printf("\n Cliente: %s\n",resposta);
    }while(strcmp(resposta,"exit")!=0);
	pthread_mutex_destroy(&mutexsum);
	pthread_exit(NULL);
	done=0;
}

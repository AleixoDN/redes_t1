#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int    sock_cliente;
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;

void *sendmessage();
void *listener();
int done=1;

int main() {

    // Cria o socket e verifica erros
    if((sock_cliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("Erro ao criar o socket!\n");
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
    addr.sin_port        = htons(1311);                 // Define a porta do servidor
    addr.sin_addr.s_addr = inet_addr("192.168.0.104");  // Define o IP como o ip do computador
    memset(&addr.sin_zero,0,sizeof(addr.sin_zero));     // Set de zeros para uso do sistema

    /*
    sockaddr_in: é uma struct utilizada para guardar informaçoes sobre os sockets.
    memset() garante que informacoes da struct que nao serao utilizadas contenham o valor 0.
    AF_INET indica que o socket contém um endereço IPV4
    a funcao htons() garante que o numero da porta esta no formatado de maneira adequada.
    */

    // Conecta-se ao servidor nos parametros dados e verifica erros
    printf("Tentando se conectar ao servidor...\n");
    if(connect(sock_cliente,(struct sockaddr*)&addr,sizeof(addr)) == -1) {
        printf("ERRO. Não foi possível conectar ao servidor.\n");
        return 1;
    }

    printf("Conectado!\n\n");

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

    do {
        printf("Cliente: ");
        fgets(mensagem,256,stdin);
        mensagem[strlen(mensagem)-1] = '\0';
        enviados = send(sock_cliente,mensagem,strlen(mensagem),0);
    } while(strcmp(mensagem,"exit")!=0);

	pthread_mutex_destroy(&mutexsum);
	pthread_exit(NULL);
	close(sock_cliente);
	done=0;
}

void *listener() {
    int recebidos;
    char resposta[256];

    do {
        recebidos = recv(sock_cliente,resposta,256,0);
        resposta[recebidos] = '\0';
        printf("\n Servidor: %s\n",resposta);
    } while(recebidos != -1);

}

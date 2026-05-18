#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int s;

    char msg[100];

    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(s, (struct sockaddr*)&server, sizeof(server));

    printf("Enter message: ");
    fgets(msg, sizeof(msg), stdin);

    send(s, msg, strlen(msg)+1, 0);

    recv(s, msg, sizeof(msg), 0);

    printf("Message from server: %s\n", msg);

    close(s);

    return 0;
}
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int s, ns;

    char msg[100];

    struct sockaddr_in server, client;

    socklen_t len;

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&server, sizeof(server));

    listen(s, 5);

    len = sizeof(client);

    ns = accept(s, (struct sockaddr*)&client, &len);

    recv(ns, msg, sizeof(msg), 0);

    printf("Message from client: %s\n", msg);

    send(ns, msg, strlen(msg)+1, 0);

    close(ns);
    close(s);

    return 0;
}
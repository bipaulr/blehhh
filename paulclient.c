#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 1024

int sock;

void *receive(void *arg) {
    char buf[BUF_SIZE];
    int n;
    while ((n = recv(sock, buf, BUF_SIZE - 1, 0)) > 0) {
        buf[n] = 0;
        printf("%s", buf);
        fflush(stdout);
    }
    printf("Disconnected from server.\n");
    exit(0);
}

int main() {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT)
    };
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect failed");
        return 1;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, receive, NULL);
    pthread_detach(tid);

    char buf[BUF_SIZE];
    while (fgets(buf, BUF_SIZE, stdin)) {
        send(sock, buf, strlen(buf), 0);
        if (strncmp(buf, "/quit", 5) == 0) break;
    }

    close(sock);
    return 0;
}

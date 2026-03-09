#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX_CLIENTS 10
#define BUF_SIZE 1024

int clients[MAX_CLIENTS];
char usernames[MAX_CLIENTS][64];
int client_count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void broadcast(char *msg, int sender_fd) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++) {
        if (clients[i] != sender_fd)
            send(clients[i], msg, strlen(msg), 0);
    }
    pthread_mutex_unlock(&lock);
}

void remove_client(int fd) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++) {
        if (clients[i] == fd) {
            clients[i] = clients[client_count - 1];
            strcpy(usernames[i], usernames[client_count - 1]);
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&lock);
}

void *handle_client(void *arg) {
    int fd = *(int *)arg;
    free(arg);

    char buf[BUF_SIZE];
    char username[64];

    // Get username
    send(fd, "Enter username: ", 16, 0);
    int n = recv(fd, username, sizeof(username) - 1, 0);
    if (n <= 0) { close(fd); return NULL; }
    username[strcspn(username, "\r\n")] = 0;

    // Register client
    pthread_mutex_lock(&lock);
    clients[client_count] = fd;
    strcpy(usernames[client_count], username);
    client_count++;
    pthread_mutex_unlock(&lock);

    snprintf(buf, BUF_SIZE, "*** %s joined ***\n", username);
    printf("%s", buf);
    broadcast(buf, fd);
    send(fd, "Welcome! Type /quit to exit.\n", 29, 0);

    // Message loop
    while ((n = recv(fd, buf, BUF_SIZE - 1, 0)) > 0) {
        buf[n] = 0;
        buf[strcspn(buf, "\r\n")] = 0;
        if (strcmp(buf, "/quit") == 0) break;

        char msg[BUF_SIZE + 70];
        snprintf(msg, sizeof(msg), "[%s]: %s\n", username, buf);
        printf("%s", msg);
        broadcast(msg, fd);
    }

    snprintf(buf, BUF_SIZE, "*** %s left ***\n", username);
    printf("%s", buf);
    remove_client(fd);
    broadcast(buf, fd);
    close(fd);
    return NULL;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };
    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, MAX_CLIENTS);

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int *fd = malloc(sizeof(int));
        *fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
        printf("New connection: %s\n", inet_ntoa(client_addr.sin_addr));

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, fd);
        pthread_detach(tid);
    }
}

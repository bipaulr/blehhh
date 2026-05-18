#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define LOSS_PROBABILITY 70   // 30% chance to send NACK instead of ACK

int main() {
    int s, ns;
    char msg[100], reply[100];
    struct sockaddr_in server, client;
    socklen_t len;

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&server, sizeof(server));
    listen(s, 5);

    printf("Server waiting...\n");

    len = sizeof(client);
    ns = accept(s, (struct sockaddr*)&client, &len);
    printf("Connection established.\n");

    srand(time(0));
    int expected = 1;

    while (1) {
        memset(msg, 0, sizeof(msg));
        int bytes_received = recv(ns, msg, sizeof(msg), 0);

        if (bytes_received <= 0 || strcmp(msg, "Exit") == 0) {
            printf("Client finished transmission. Exiting...\n");
            break;
        }

        int frame;
        if (sscanf(msg, "Frame %d", &frame) != 1) {
            continue;
        }

        printf("Received: %s\n", msg);

        if (frame == expected) {
            // Simulate "loss" by sending NACK instead of ACK
            if (rand() % 100 >= LOSS_PROBABILITY) {
                sprintf(reply, "ACK %d", frame);
                send(ns, reply, strlen(reply) + 1, 0);
                printf("Sent: %s\n\n", reply);
                expected++;
            } else {
                // Send NACK for the lost frame
                sprintf(reply, "NACK %d", frame);
                send(ns, reply, strlen(reply) + 1, 0);
                printf("Sent: %s (Simulated loss → NACK)\n\n", reply);
            }
        } else {
            // Out-of-order frame – send cumulative ACK of last in-order frame
            printf("Discarded Out-of-Order Frame %d (Still expecting Frame %d)\n", frame, expected);
            int last_ack = expected - 1;
            if (last_ack >= 1) {
                sprintf(reply, "ACK %d", last_ack);
                send(ns, reply, strlen(reply) + 1, 0);
                printf("Sent cumulative: %s\n\n", reply);
            } else {
                printf("No prior safe packets to ACK yet.\n\n");
            }
        }
    }

    close(ns);
    close(s);
    return 0;
}
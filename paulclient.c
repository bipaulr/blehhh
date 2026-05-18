#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s;
    int base = 1, next = 1;
    int window = 3;
    int total = 5;
    char msg[100], reply[100];
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection failed.\n");
        return 1;
    }

    // No timeout configuration – we rely on NACKs from the server

    while (base <= total) {
        // Send burst inside window
        while (next < base + window && next <= total) {
            memset(msg, 0, sizeof(msg));
            sprintf(msg, "Frame %d", next);
            send(s, msg, strlen(msg) + 1, 0);
            printf("Sent: %s\n", msg);
            sleep(1);   // small delay for visualization
            next++;
        }

        // Receive reply (blocks until server responds with ACK or NACK)
        memset(reply, 0, sizeof(reply));
        recv(s, reply, sizeof(reply), 0);

        int ackno;
        if (sscanf(reply, "ACK %d", &ackno) == 1) {
            if (ackno >= base) {
                printf("Received: %s\n", reply);
                base = ackno + 1;   // slide window past ACKed frame
            }
        }
        else if (sscanf(reply, "NACK %d", &ackno) == 1) {
            printf("Received: %s → Retransmitting from frame %d\n", reply, ackno);
            next = ackno;           // Go-Back-N: restart from lost frame
            // Optional: base remains unchanged (we haven't advanced)
        }
    }

    // Signal server to exit cleanly
    memset(msg, 0, sizeof(msg));
    sprintf(msg, "Exit");
    send(s, msg, strlen(msg) + 1, 0);

    printf("All frames transmitted successfully!\n");
    close(s);
    return 0;
}
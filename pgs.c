#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>      // Required for seeding rand()

#define LOSS_PROBABILITY 30 // 30% chance of ACK loss simulation

int main()
{
    int s, ns;
    char msg[100], ack[100];
    struct sockaddr_in server, client;
    socklen_t len;

    s = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&server, sizeof(server));
    listen(s, 5);

    printf("Server waiting...\n");

    len = sizeof(client);
    ns = accept(s, (struct sockaddr*)&client, &len);
    printf("Connection established.\n");

    // Seed the random number generator using system time
    srand(time(0));
    int expected = 1;

    while(1)
    {
        memset(msg, 0, sizeof(msg));
        int bytes_received = recv(ns, msg, sizeof(msg), 0);

        // Check if the client disconnected or sent the exit string
        if (bytes_received <= 0 || strcmp(msg, "Exit") == 0)
        {
            printf("Client finished transmission. Terminating server...\n");
            break;
        }

        int frame;
        if (sscanf(msg, "Frame %d", &frame) != 1) {
            continue;
        }

        printf("Received: %s\n", msg);

        /* 1. IN-ORDER FRAME HANDLING */
        if(frame == expected)
        {
            // Simulate Random Network ACK Drop
            if(rand() % 100 >= LOSS_PROBABILITY)
            {
                memset(ack, 0, sizeof(ack));
                sprintf(ack, "ACK %d", frame);

                send(ns, ack, strlen(ack) + 1, 0);
                printf("Sent: %s\n\n", ack);

                expected++; // Accept frame and advance expectation tracker
            }
            else
            {
                // Frame arrived fine, but we intentionally drop the reply
                printf("ACK for Frame %d lost! (Simulated Loss)\n\n", frame);
                
                // CRITICAL: We DO NOT increment expected here because 
                // from the client's point of view, this frame was never finalized.
            }
        }
        /* 2. OUT-OF-ORDER FRAME HANDLING */
        else
        {
            printf("Discarded Out-of-Order Frame %d (Still expecting Frame %d)\n", frame, expected);
            
            // Send a cumulative ACK for the last successful packet received
            int last_ack = expected - 1;
            if (last_ack >= 1)
            {
                memset(ack, 0, sizeof(ack));
                sprintf(ack, "ACK %d", last_ack);
                send(ns, ack, strlen(ack) + 1, 0);
                printf("Sent cumulative: %s\n\n", ack);
            }
            else
            {
                printf("No prior safe packets to ACK yet.\n\n");
            }
        }
    }

    close(ns);
    close(s);
    return 0;
}
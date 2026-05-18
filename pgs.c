#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>      

#define LOSS_PROBABILITY 30 

int main()
{
    int s, ns;
    char msg[100], ack[100];
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

    while(1)
    {
        memset(msg, 0, sizeof(msg));
        int bytes_received = recv(ns, msg, sizeof(msg), 0);

        if (bytes_received <= 0 || strcmp(msg, "Exit") == 0)
        {
            printf("Client finished transmission. Exiting...\n");
            break;
        }

        int frame;
        if (sscanf(msg, "Frame %d", &frame) != 1) {
            continue;
        }

        printf("Received: %s\n", msg);

        if(frame == expected)
        {
            if(rand() % 100 >= LOSS_PROBABILITY)
            {
                memset(ack, 0, sizeof(ack));
                sprintf(ack, "ACK %d", frame);

                send(ns, ack, strlen(ack) + 1, 0);
                printf("Sent: %s\n\n", ack);

                expected++; 
            }
            else
            {
                printf("ACK for Frame %d lost! (Simulated Loss)\n\n", frame);
            }
        }
        else
        {
            printf("Discarded Out-of-Order Frame %d (Still expecting Frame %d)\n", frame, expected);
            
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
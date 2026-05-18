#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int s;

    char msg[100], ack[100];

    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(s, (struct sockaddr*)&server, sizeof(server));

    for(int i = 0; i < 5; i++)
    {
        recv(s, msg, sizeof(msg), 0);

        printf("Received: %s\n", msg);

        /*
            Simulate Frame 2 loss
        */

        if(i == 2)
        {
            printf("Frame 2 Lost\n");

            sprintf(ack, "NACK 2");

            send(s, ack, strlen(ack)+1, 0);

            printf("Sent: %s\n", ack);

            /*
                Receive retransmitted frame
            */

            recv(s, msg, sizeof(msg), 0);

            printf("Received Retransmitted: %s\n", msg);

            sprintf(ack, "ACK 2");

            send(s, ack, strlen(ack)+1, 0);

            printf("Sent: %s\n", ack);

            continue;
        }

        sprintf(ack, "ACK %d", i);

        send(s, ack, strlen(ack)+1, 0);

        printf("Sent: %s\n", ack);
    }

    close(s);

    return 0;
}
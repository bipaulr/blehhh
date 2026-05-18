#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int s, ns;

    int base = 0;
    int nextSeq = 0;

    int window = 3;
    int total = 5;

    char msg[100], ack[100];

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

    while(base < total)
    {
        /*
            Send frames within window
        */

        while(nextSeq < base + window &&
              nextSeq < total)
        {
            sprintf(msg, "Frame %d", nextSeq);

            send(ns, msg, strlen(msg)+1, 0);

            printf("Sent: %s\n", msg);

            nextSeq++;
        }

        /*
            Receive ACK/NACK
        */

        recv(ns, ack, sizeof(ack), 0);

        printf("Received: %s\n", ack);

        /*
            NACK handling
        */

        if(strncmp(ack, "NACK", 4) == 0)
        {
            int lost;

            sscanf(ack, "NACK %d", &lost);

            printf("Go Back from Frame %d\n", lost);

            nextSeq = lost;

            base = lost;
        }
        else
        {
            base++;
        }
    }

    close(ns);

    close(s);

    return 0;
}
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int s;
    int expected = 0;
    int error = 0;
    char msg[100], ack[100];
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(s, (struct sockaddr*)&server, sizeof(server));

    while(expected < 5)
    {
        int bytes_received = recv(s, msg, sizeof(msg), 0);

        // Check if the server disconnected or crashed
        if (bytes_received <= 0) 
        {
            printf("Server disconnected unexpectedly. Exiting...\n");
            break;
        }

        printf("Received: %s\n", msg);

        int frame;
        // It is also good practice to make sure sscanf actually found a number
        if (sscanf(msg, "Frame %d", &frame) != 1) 
        {
            printf("Received malformed message. Skipping...\n");
            continue;
        }

        /* Simulate error for Frame 2 only once */
        if(frame == 2 && error == 0)
        {
            sprintf(ack, "NACK 2");
            send(s, ack, strlen(ack)+1, 0);
            printf("Sent: %s (Simulated Corruption)\n", ack);
            error = 1;
            continue; 
        }

        /* Accept only expected frame */
        if(frame == expected)
        {
            sprintf(ack, "ACK %d", frame);
            send(s, ack, strlen(ack)+1, 0);
            printf("Sent: %s\n", ack);
            expected++;
        }
        /* Handle out-of-order frames */
        else
        {
            printf("Discarded Out-of-Order Frame %d\n", frame);
            sprintf(ack, "NACK %d", expected);
            send(s, ack, strlen(ack)+1, 0);
            printf("Sent: %s\n", ack);
        }
    }

    close(s);
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h> // Required for struct timeval

int main()
{
    int s;
    int base = 1, next = 1;
    int window = 3;
    int total = 5;
    char msg[100], ack[100];
    struct sockaddr_in server;
    
    // Define a 3-second timeout structure
    struct timeval tv = {3, 0}; 

    s = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection failed.\n");
        return 1;
    }

    // Set the receive timeout option on the client socket
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while(base <= total)
    {
        /* 1. SEND BURST: Transmit frames inside the sliding window */
        while(next < base + window && next <= total)
        {
            memset(msg, 0, sizeof(msg));
            sprintf(msg, "Frame %d", next);

            send(s, msg, strlen(msg) + 1, 0);
            printf("Sent: %s\n", msg);

            sleep(1); // Delay to visualize the pipeline flow easily
            next++;
        }

        /* 2. RECEIVE FEEDBACK: Wait for ACK from the server */
        memset(ack, 0, sizeof(ack));
        int bytes_received = recv(s, ack, sizeof(ack), 0);

        if (bytes_received > 0) 
        {
            int ackno;
            if (sscanf(ack, "ACK %d", &ackno) == 1) 
            {
                if (ackno >= base) 
                {
                    printf("Received: %s\n", ack);
                    base = ackno + 1; // Slide the window past the ACKed frame
                }
            }
        } 
        /* 3. TIMEOUT HANDLING: Triggered if no ACK arrives within 3 seconds */
        else 
        {
            printf("Timeout! Frame %d or its ACK was lost. Rolling back...\n", base);
            next = base; // Reset next sequence pointer back to the unacknowledged base
        }
    }

    // Send an explicit signal telling the server to close down cleanly
    memset(msg, 0, sizeof(msg));
    sprintf(msg, "Exit");
    send(s, msg, strlen(msg) + 1, 0);

    printf("All frames transmitted successfully!\n");
    close(s);
    return 0;
}
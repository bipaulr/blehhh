#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h> // Required for FIONREAD to inspect the socket buffer

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

    // Allow quick port reuse to avoid "Address already in use" errors on restarts
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

    while(base < total)
    {
        /*
            1. Send frames inside the sliding window
        */
        while(nextSeq < base + window && nextSeq < total)
        {
            sprintf(msg, "Frame %d", nextSeq);

            send(ns, msg, strlen(msg)+1, 0);

            printf("Sent: %s\n", msg);

            sleep(1); // Delay to make visualizing the window easier

            nextSeq++;
        }

        /*
            2. Process primary ACK/NACK feedback
        */
        int bytes_received = recv(ns, ack, sizeof(ack), 0);

        // Check if client disconnected or connection dropped
        if (bytes_received <= 0)
        {
            printf("Client disconnected unexpectedly. Terminating server...\n");
            break;
        }

        printf("Received: %s\n", ack);

        /* NACK handling */
        if(strncmp(ack, "NACK", 4) == 0)
        {
            int lost;
            sscanf(ack, "NACK %d", &lost);

            printf("Retransmitting from Frame %d\n", lost);

            base = lost;
            nextSeq = lost;
        }
        /* ACK handling */
        else if(strncmp(ack, "ACK", 3) == 0)
        {
            int ackno;
            sscanf(ack, "ACK %d", &ackno);
            
            if (ackno >= base) 
            {
                base = ackno + 1;
            }
        }

        /*
            3. Drain the buffer: Check and process any additional pending ACKs/NACKs
               sitting right behind the first one in the socket pipeline.
        */
        int bytes_available = 0;
        ioctl(ns, FIONREAD, &bytes_available);

        while(bytes_available > 0)
        {
            bytes_received = recv(ns, ack, sizeof(ack), 0);

            if (bytes_received <= 0)
            {
                break;
            }

            printf("Received (buffered): %s\n", ack);

            if(strncmp(ack, "NACK", 4) == 0)
            {
                int lost;
                sscanf(ack, "NACK %d", &lost);

                printf("Retransmitting from Frame %d\n", lost);

                base = lost;
                nextSeq = lost;
                
                // Instantly break ACK processing loop to go back and retransmit
                break; 
            }
            else if(strncmp(ack, "ACK", 3) == 0)
            {
                int ackno;
                sscanf(ack, "ACK %d", &ackno);
                
                if (ackno >= base) 
                {
                    base = ackno + 1;
                }
            }
            
            // Check if any more bytes are still left in the buffer line
            ioctl(ns, FIONREAD, &bytes_available);
        }
    }

    // Only prints if the window cleanly finishes without crashing/breaking early
    if (base >= total)
    {
        printf("All frames transmitted successfully\n");
    }

    close(ns);
    close(s);

    return 0;
}
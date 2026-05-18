#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
int s, i;
char msg[100], ack[100];
struct sockaddr_in server;
s = socket(AF_INET, SOCK_STREAM, 0);
server.sin_family = AF_INET;
server.sin_port = htons(5000);
server.sin_addr.s_addr = inet_addr("127.0.0.1");
connect(s, (struct sockaddr*)&server, sizeof(server));
for(i = 0; i < 5; )
{
recv(s, msg, sizeof(msg), 0);
printf("Received: %s\n", msg);
if(i == 2)
{
strcpy(ack, "NACK");
send(s, ack, strlen(ack)+1, 0);
printf("Sent: %s\n", ack);
i = -2;
continue;
}
if(i == -2)
{
i = 2;
}
sprintf(ack, "ACK %d", i);
send(s, ack, strlen(ack)+1, 0);
printf("Sent: %s\n", ack);
i++;
}
close(s);
return 0;
}
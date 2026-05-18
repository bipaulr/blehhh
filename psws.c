#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
int s, ns, i;
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
for(i = 0; i < 5; )
{
sprintf(msg, "Frame %d", i);
send(ns, msg, strlen(msg)+1, 0);
printf("Sent: %s\n", msg);
recv(ns, ack, sizeof(ack), 0);
printf("Received: %s\n", ack);
if(strncmp(ack, "NACK", 4) == 0)
{
printf("Retransmitting Frame %d\n", i);
}
else
{
i++;
}
}
close(ns);
close(s);
return 0;
}
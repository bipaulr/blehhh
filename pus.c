#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
int s;
char msg[100];
struct sockaddr_in server, client;
socklen_t len;
s = socket(AF_INET, SOCK_DGRAM, 0);
server.sin_family = AF_INET;
server.sin_port = htons(5000);
server.sin_addr.s_addr = INADDR_ANY;
bind(s, (struct sockaddr*)&server, sizeof(server));
len = sizeof(client);
recvfrom(s, msg, sizeof(msg), 0, (struct sockaddr*)&client, &len);
printf("Client: %s\n", msg);
strcpy(msg, "Hello Client");
sendto(s, msg, strlen(msg)+1, 0, (struct sockaddr*)&client, len);
close(s);
return 0;
}

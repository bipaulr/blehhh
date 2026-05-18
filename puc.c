#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
int s;
char msg[100];
struct sockaddr_in server;
socklen_t len;
s = socket(AF_INET, SOCK_DGRAM, 0);
server.sin_family = AF_INET;
server.sin_port = htons(5000);
server.sin_addr.s_addr = inet_addr("127.0.0.1");
len = sizeof(server);
printf("Enter message: ");
fgets(msg, sizeof(msg), stdin);
msg[strcspn(msg, "\n")] = '\0';
sendto(s, msg, strlen(msg)+1, 0, (struct sockaddr*)&server, len);
recvfrom(s, msg, sizeof(msg), 0, NULL, NULL);
printf("Server: %s\n", msg);
close(s);
return 0;
}
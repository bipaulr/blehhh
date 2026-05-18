#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
void resend_message(int msg_num, int c_sock)
{
char buff[60];
snprintf(buff, sizeof(buff), "Resending message: %d", msg_num);
printf("%s\n", buff);
write(c_sock, buff, sizeof(buff));
usleep(1000);
}
int main()
{
int s_sock, c_sock;
struct sockaddr_in server, client;
socklen_t addr_len = sizeof(client);
s_sock = socket(AF_INET, SOCK_STREAM, 0);
if (s_sock < 0)
{
perror("socket");
return 0;
}
server.sin_family = AF_INET;
server.sin_port = htons(9009);
server.sin_addr.s_addr = INADDR_ANY;
bind(s_sock, (struct sockaddr *)&server, sizeof(server));
listen(s_sock, 10);
printf("Server Up - Selective Repeat ARQ\n");
c_sock = accept(s_sock, (struct sockaddr *)&client, &addr_len);
int total_msgs = 9;
int window_size = 3;
int sent_msgs = 0;
while (sent_msgs < total_msgs)
{
for (int i = sent_msgs; i < sent_msgs + window_size && i < total_msgs; i++)
{
char buff[50];
snprintf(buff, sizeof(buff), "server message: %d", i);
printf("Message sent to client: %s\n", buff);
write(c_sock, buff, sizeof(buff));
usleep(1000);
}
sent_msgs += window_size;
}
close(c_sock);
close(s_sock);
}
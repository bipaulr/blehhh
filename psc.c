#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
int is_faulty()
{
int d = rand() % 4;
return (d > 2);
}
int main()
{
srand(time(0));
int sockfd;
struct sockaddr_in server;
char buffer[100], response[50];
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0)
{
perror("socket failed");
return 0;
}
server.sin_family = AF_INET;
server.sin_port = htons(9009);
server.sin_addr.s_addr = inet_addr("127.0.0.1");
if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
{
perror("connection failed");
return 0;
}
printf("Client with individual acknowledgement (Selective Repeat)\n");
int count = 0;
while (count < 9)
{
bzero(buffer, sizeof(buffer));
int n = read(sockfd, buffer, sizeof(buffer));
if (n <= 0)
continue;
int msg_num = buffer[strlen(buffer) - 1] - '0';
printf("Message received from server: %s\n", buffer);
int faulty = is_faulty();
if (faulty)
{
snprintf(response, sizeof(response), "nack%d", msg_num);
printf("Negative ACK sent for message %d\n", msg_num);
}
else
{
snprintf(response, sizeof(response), "ack%d", msg_num);
printf("ACK sent for message %d\n", msg_num);
count++;
}
write(sockfd, response, sizeof(response));
usleep(1000);
}
close(sockfd);
return 0;
}
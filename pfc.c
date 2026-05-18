#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
int s, n;
char fname[100], data[100];
struct sockaddr_in server;
s = socket(AF_INET, SOCK_STREAM, 0);
server.sin_family = AF_INET;
server.sin_port = htons(5000);
server.sin_addr.s_addr = inet_addr("127.0.0.1");
connect(s, (struct sockaddr*)&server, sizeof(server));
printf("Enter filename to retrieve: ");
fgets(fname, sizeof(fname), stdin);
fname[strcspn(fname, "\n")] = '\0';
send(s, fname, strlen(fname)+1, 0);
printf("\n--- File Contents ---\n");
while ((n = recv(s, data, sizeof(data)-1, 0)) > 0)
{
data[n] = '\0';
printf("%s", data);
}
printf("\n--- End of File ---\n");
close(s);
return 0;
}
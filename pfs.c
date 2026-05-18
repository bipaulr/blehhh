#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
int s, ns;
char fname[100], data[100];
FILE *fp;
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
recv(ns, fname, sizeof(fname), 0);
printf("File requested: %s\n", fname);
fp = fopen(fname, "r");
if (fp == NULL)
send(ns, "ERROR: File not found", 22, 0);
else
{
while (fgets(data, sizeof(data), fp) != NULL)
send(ns, data, strlen(data)+1, 0);
fclose(fp);
}
close(ns);
close(s);
return 0;
}
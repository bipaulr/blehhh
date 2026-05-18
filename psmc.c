#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
int s;
char buf[256];
struct sockaddr_in server;
s = socket(AF_INET, SOCK_STREAM, 0);
server.sin_family = AF_INET;
server.sin_port = htons(2525);
server.sin_addr.s_addr = inet_addr("127.0.0.1");
connect(s, (struct sockaddr*)&server, sizeof(server));
send(s, "HELO localhost", 15, 0);
recv(s, buf, sizeof(buf), 0); printf("Server: %s\n", buf);
send(s, "MAIL FROM: abc@gmail.com", 25, 0);
recv(s, buf, sizeof(buf), 0); printf("Server: %s\n", buf);
send(s, "RCPT TO: xyz@gmail.com", 23, 0);
recv(s, buf, sizeof(buf), 0); printf("Server: %s\n", buf);
send(s, "DATA", 5, 0);
recv(s, buf, sizeof(buf), 0); printf("Server: %s\n", buf);
send(s, "Hello World", 12, 0);
recv(s, buf, sizeof(buf), 0); printf("Server: %s\n", buf);
send(s, "QUIT", 5, 0);
close(s);
return 0;
}
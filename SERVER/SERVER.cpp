#include "stdafx.h"
#include "myheader.h"

#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9500
#define BUFSIZE    512

#define SAVEMODE 1
#define LOADMODE 2


// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void printDir()	//���丮���
{
	printf("\n---------------------- C:\MyServer\ -------------------\n\n");
	system("dir C:\MyServer /b");
	printf("\n---------------------- C:\MyServer\ -------------------\n\n");
}

// ����� ���� ������ ���� �Լ�
int savef(SOCKET client_sock)
{
	char *buf;
	int fsize=0;
	char path[50];
	char fname[20];

	FILE *fp;
	int retval;
	retval = recv(client_sock, fname, sizeof(fname), 0);//�����̸��޾ƿ���
	if(retval == SOCKET_ERROR){
		err_display("recv()");
		return 0;
	}
	printf("���� �̸� ����!! ���� �̸� : %s\n",fname);

	strcpy(path,"C:\\MyServer\\");

	
	strcat(path,fname);//�����̸��� �����ġ��

	printf("������ : %s\n",path);

	if(retval == SOCKET_ERROR){
		err_display("recv()");
		return 0;
	}
	//printf("%s ������.!!\n",path);
	// ������ �ޱ�(���� ����)
	retval = recv(client_sock, (char *)&fsize, sizeof(int), 0);		//���ϻ�����ޱ�
	if(retval == SOCKET_ERROR){
		err_display("recv()");
		return 0;
	}
	else if(retval == 0)
		return 0;
	printf("���� ������ %d ����Ʈ \n",fsize);

	buf = (char*)malloc(fsize);
	memset(buf, 0, fsize);			//�����ʱ�ȭ
		// ������ �ޱ�(���� ����)
	retval = recv(client_sock, buf, fsize, 0);
	if(retval == SOCKET_ERROR){
		err_display("recv()");
		return 0;
	}
	else if(retval == 0)
		return 0;
	if((fp=fopen(path,"w"))==NULL)
	{
		printf("error");
		return 0;
	}
	fwrite(buf, 1, fsize, fp);
	printf("%s ����Ϸ�!!\n",path);
	printDir();
	free(buf);
	fclose(fp);
	return 0;
}



int loadf(SOCKET client_sock)
{
	char *buf;
	int fsize=0;
	char path[50];
	char fname[20];
	int tF=0;
	FILE *fp;
	int retval;

	while(1){
		tF=-1;
		retval = recv(client_sock, fname, sizeof(fname), 0);//�����̸��޾ƿ���

		printf("���� �̸� ����!! ���� �̸� : %s\n",fname);

		strcpy(path,"C:\\MyServer\\");

		strcat(path,fname);//�����̸��� �����ġ��

		if((fp=fopen(path,"r"))!=NULL)
		{
			tF=1;
			send(client_sock,(char*)&tF, sizeof(int), 0);
			printf("%s �о��\n",path);
			break;	
		}
		printf("%s �� �����ϴ�\n",path);
		send(client_sock,(char*)&tF, sizeof(int), 0);
	}

	fseek(fp, 0, SEEK_END);   // ���������Ͱ� ������ ������
	fsize = ftell(fp);        // ����ũ�⸦ fsize�� ����
	fseek(fp, 0, SEEK_SET);   // ���������Ͱ� �ʱ�ȭ

	retval=send(client_sock, (char*)&fsize, sizeof(fsize), 0); // ���ϻ����� ������
	printf("���� ������ %d ����Ʈ \n",fsize);

	buf = (char*)malloc(fsize);			//������ �޸𸮸� ���ϻ������ �����Ҵ�
	memset(buf, 0, fsize);// �ʱ�ȭ

	fread(buf, 1, fsize, fp);			//���ۿ� ���� �б�

	retval=send(client_sock, buf, fsize, 0);	//���ۺ�����
	printf("%s ������ �Ϸ�\n",fname);
	
	printDir();

	free(buf);
	fclose(fp);
	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	int MODE=0;



	if(mkdir("C:\\MyServer\\")==0)
		printf("C:\\MyServer\\ ������ ���� ����!!!");

	printDir();

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		printf("\n���� �����....\n");
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\ Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���

		retval = recv(client_sock, (char *)&MODE, sizeof(int), 0);//������, �ε��� �޾Ƽ�Ȯ��
		if(MODE==SAVEMODE)
		{
			printf(" ���Ӹ�� = ���ε� \n");
			savef(client_sock);
		}
		else if(MODE==LOADMODE)
		{
			printf(" ���Ӹ�� = �ٿ�ε� \n");
			loadf(client_sock);
		}
		// closesocket()
		closesocket(client_sock);
		printf(" Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}
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


// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

void printDir()	//디렉토리출력
{
	printf("\n---------------------- C:\MyServer\ -------------------\n\n");
	system("dir C:\MyServer /b");
	printf("\n---------------------- C:\MyServer\ -------------------\n\n");
}

// 사용자 정의 데이터 수신 함수
int savef(SOCKET client_sock)
{
	char *buf;
	int fsize=0;
	char path[50];
	char fname[20];

	FILE *fp;
	int retval;
	retval = recv(client_sock, fname, sizeof(fname), 0);//파일이름받아오기
	if(retval == SOCKET_ERROR){
		err_display("recv()");
		return 0;
	}
	printf("파일 이름 수신!! 파일 이름 : %s\n",fname);

	strcpy(path,"C:\\MyServer\\");

	
	strcat(path,fname);//파일이름과 경로합치기

	printf("저장경로 : %s\n",path);

	if(retval == SOCKET_ERROR){
		err_display("recv()");
		return 0;
	}
	//printf("%s 저장중.!!\n",path);
	// 데이터 받기(고정 길이)
	retval = recv(client_sock, (char *)&fsize, sizeof(int), 0);		//파일사이즈받기
	if(retval == SOCKET_ERROR){
		err_display("recv()");
		return 0;
	}
	else if(retval == 0)
		return 0;
	printf("파일 사이즈 %d 바이트 \n",fsize);

	buf = (char*)malloc(fsize);
	memset(buf, 0, fsize);			//버퍼초기화
		// 데이터 받기(가변 길이)
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
	printf("%s 저장완료!!\n",path);
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
		retval = recv(client_sock, fname, sizeof(fname), 0);//파일이름받아오기

		printf("파일 이름 수신!! 파일 이름 : %s\n",fname);

		strcpy(path,"C:\\MyServer\\");

		strcat(path,fname);//파일이름과 경로합치기

		if((fp=fopen(path,"r"))!=NULL)
		{
			tF=1;
			send(client_sock,(char*)&tF, sizeof(int), 0);
			printf("%s 읽어옴\n",path);
			break;	
		}
		printf("%s 이 없습니다\n",path);
		send(client_sock,(char*)&tF, sizeof(int), 0);
	}

	fseek(fp, 0, SEEK_END);   // 파일포인터가 파일의 마지막
	fsize = ftell(fp);        // 파일크기를 fsize에 전달
	fseek(fp, 0, SEEK_SET);   // 파일포인터가 초기화

	retval=send(client_sock, (char*)&fsize, sizeof(fsize), 0); // 파일사이즈 보내기
	printf("파일 사이즈 %d 바이트 \n",fsize);

	buf = (char*)malloc(fsize);			//버퍼의 메모리를 파일사이즈로 동적할당
	memset(buf, 0, fsize);// 초기화

	fread(buf, 1, fsize, fp);			//버퍼에 파일 읽기

	retval=send(client_sock, buf, fsize, 0);	//버퍼보내기
	printf("%s 보내기 완료\n",fname);
	
	printDir();

	free(buf);
	fclose(fp);
	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
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

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	int MODE=0;



	if(mkdir("C:\\MyServer\\")==0)
		printf("C:\\MyServer\\ 폴더를 새로 생성!!!");

	printDir();

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		printf("\n접속 대기중....\n");
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		printf("\ 클라이언트 접속: IP 주소=%s, 포트 번호=%d",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신

		retval = recv(client_sock, (char *)&MODE, sizeof(int), 0);//저장모드, 로드모드 받아서확인
		if(MODE==SAVEMODE)
		{
			printf(" 접속모드 = 업로드 \n");
			savef(client_sock);
		}
		else if(MODE==LOADMODE)
		{
			printf(" 접속모드 = 다운로드 \n");
			loadf(client_sock);
		}
		// closesocket()
		closesocket(client_sock);
		printf(" 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
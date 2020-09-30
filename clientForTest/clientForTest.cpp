// clientForTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "myheader.h"


#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9500
#define BUFSIZE    50

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

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("connect()");

	// 데이터 통신에 사용할 변수
	char *buf;
	FILE *fp;
	char fname[20];
	char path[50]="C:\\MyPicture\\";
	int fsize=0;

	if(mkdir("C:\\MyPicture\\")==0)
	printf("C:\\MyPicture\\ 폴더를 새로 생성!!!");


	scanf("%s",fname);

	strcat(path,fname);
	//printf("%s ",path);
	// 서버와 데이터 통신
	if((fp=fopen(path,"r"))==NULL)
	{
		printf("%s 이 없습니다\n",path);
		return 0;
	}
	printf("%s 로드, 서버에 저장할 이름 입력 :",path);

	scanf("%s",fname);
	send(sock,fname,sizeof(fname),0);

	printf("%s로 저장중..\n :",fname);

	// fseek와 ftell을 통해 파일크기 구하기
	fseek(fp, 0, SEEK_END);   // 파일포인터가 파일의 마지막
	fsize = ftell(fp);        // 파일크기를 fsize에 전달
	fseek(fp, 0, SEEK_SET);   // 파일포인터가 초기화

	retval=send(sock, (char*)&fsize, sizeof(fsize), 0); // 파일사이즈 보내기
	printf("%d, %d, ",retval,fsize);

	buf = (char*)malloc(fsize);
	memset(buf, 0, fsize);// 초기화

	fread(buf, 1, fsize, fp);			//파일 저장

	retval=send(sock, buf, fsize, 0);
	printf("%d ,%d ",retval,fsize);


	free(buf);
	fclose(fp);

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
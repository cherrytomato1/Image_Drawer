
#include "stdafx.h"
#include "myheader.h"


#pragma comment(lib, "ws2_32")
#include <winsock2.h>

#include<stdio.h>
#include<Windows.h>

#include<stdlib.h>
#include<conio.h>

#define xMax 120			//그림판 x최대
#define yMax 32				//y최대

#define pxMax xMax-6
#define pyMax yMax-3

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9500
#define BUFSIZE    50

#define SAVEMODE 1
#define LOADMODE 2


#define col GetStdHandle (STD_OUTPUT_HANDLE)	//색상 변경을 위함

#define RED SetConsoleTextAttribute( col,0x000c); // 빨간색
#define BLUE SetConsoleTextAttribute( col,0x0001 | 0x0008); // 파란색
#define YELLOWGREEN SetConsoleTextAttribute( col,0x00a); // 연두색
#define WHITE SetConsoleTextAttribute( col,0x000f); // 흰색
#define SKY SetConsoleTextAttribute( col,0x000b); // 하늘색
#define YELLOW SetConsoleTextAttribute( col,0x000e); // 노란색
#define FLUORESCENT SetConsoleTextAttribute( col,0x000d); // 형광색
#define GRAY SetConsoleTextAttribute( col,0x0008); // 회색

char pat='*';
char path[50];

typedef struct{			//그림의 한 점
	char pPat;			//모양
	int pCor;			//색
}paint;

typedef struct listnode{		//그림의 한 페이지(노드)
	paint dot[pxMax][pyMax];		//그림 좌표
	struct listnode *llink;		//엘링크
	struct listnode *rlink;		//알링크
}listNode;

typedef struct{					//연결리스트 헤드
	int cc;						//현재 페이지 수
	listNode *head;				//헤드링크
}listHead;



listHead* clistHead()		//연결리스트 헤드생성
{
	listHead* L;
	L= (listHead*)malloc(sizeof(listHead));
	L->head = NULL;
	return L;
}

listNode* addNode(listHead *L,listNode *current)		//연결리스트 노드 추가
{
	int i,j;

	listNode* newNode;
	newNode = (listNode *)malloc(sizeof(listNode));		//메모리할당

	for(i=0;i<pxMax;i++)					//그림좌표 초기화
		for(j=0;j<pyMax;j++)
		{
			newNode->dot[i][j].pPat=0;
			newNode->dot[i][j].pCor=0;
		}
		L->cc++;					//현재페이지 ++
		if(L->head ==NULL)		//공백리스트일때
		{
			newNode->llink= NULL;
			newNode->rlink= NULL;
			L->head =newNode;

			return newNode;
		}
		if(current->rlink!=NULL)		//현재 노드가 마지막이 아닐때
			current->rlink->llink=newNode;
		newNode->llink= current;
		newNode->rlink= current->rlink;
		current->rlink= newNode;

		return newNode;

}
void gotoxy(int x, int y)			//고투엑스와이
{
	COORD Pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void printNode2(listNode *previous,listNode *current)
{
	int i,j;
	if(previous==NULL)
	{
		for(i=0;i<pxMax;i++)								//좌표값 검사
			for(j=0;j<pyMax;j++)
			{
				if(current->dot[i][j].pPat!=0&&current->dot[i][j].pPat!=-1)		//*문양이 있을경우
				{
					switch(current->dot[i][j].pCor)		//컬러에 따른 스위치
					{
					case '0' : WHITE;					//각 컬러 함수 실행
						break;
					case '1' : RED;
						break;
					case '2' : BLUE;
						break;
					case '3' : YELLOW;
						break;
					case '4' : YELLOWGREEN;
						break;
					}

					gotoxy(i,j);						//해당좌표이동 후 출력
					printf("%c",current->dot[i][j].pPat);
					WHITE;
				}
			}
	}
	else
	{
		for(i=0;i<pxMax;i++)								//좌표값 검사
			for(j=0;j<pyMax;j++)
				if(previous->dot[i][j].pPat != current->dot[i][j].pPat || previous->dot[i][j].pCor!=current->dot[i][j].pCor)
				{
					if(current->dot[i][j].pPat==0)
					{
						gotoxy(i,j);						//해당좌표이동 후 출력
						printf(" ");
						WHITE;
					}
					switch(current->dot[i][j].pCor)		//컬러에 따른 스위치
					{
					case '0' : WHITE;					//각 컬러 함수 실행
						break;
					case '1' : RED;
						break;
					case '2' : BLUE;
						break;
					case '3' : YELLOW;
						break;
					case '4' : YELLOWGREEN;
						break;
					}
					gotoxy(i,j);						//해당좌표이동 후 출력
					printf("%c",current->dot[i][j].pPat);
					WHITE;
				}
	}
}


listNode* deleteNode(listHead *L,listNode *current)		//노드삭제
{
	listNode *temp;
	if(current->llink==NULL)				//첫번째 페이지일때
	{
		if(current->rlink==NULL)		//마지막 페이지일때
		{
			gotoxy(45,yMax);
			printf("마지막 페이지입니다.");
			return current;			//삭제안하고 리턴
		}
		printNode2(current,current->rlink);
		temp=current;				
		L->head=current->rlink;		//헤드와 오른쪽 노드를 연결
		current->rlink->llink=NULL;	//오른쪽노드의 엘링크를 널로
		current=current->rlink;

		printNode2(current->llink,current);
		free(temp);					//메모리해제
		return current;				//현재 노드 리턴
	}
	else if(current->rlink==NULL)	
		current->llink->rlink=NULL;		//마지막 노드일경우	
	else
	{
		current->rlink->llink=current->llink;		//아닐경우
		current->llink->rlink=current->rlink;
	}
	printNode2(current,current->llink);
	temp=current;			
	current=current->llink;	//현재메모리 이동
	L->cc--;			//현재 페이지값 수정
	free(temp);		//메모리해제
	return current;		//현재 노드 리턴
}

int MouseClick(int *x, int *y)
{ 
	HANDLE   hIn, hOut;
	DWORD   dwNOER;
	INPUT_RECORD rec;

	int tempX,tempY;

	hIn=GetStdHandle(STD_INPUT_HANDLE);
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleMode(hIn, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	while(1)
	{
		ReadConsoleInput(hIn,&rec,1,&dwNOER);
		*x = rec.Event.MouseEvent.dwMousePosition.X;
		*y = rec.Event.MouseEvent.dwMousePosition.Y;
	
		gotoxy(xMax-8,0);
		//if(*x<pxMax&&*y<pyMax)
		printf("%3d,%3d",*x,*y);
		if( tempX = rec.EventType == MOUSE_EVENT )
		{		
			if( tempX = rec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED )
				return 1;
			if( tempY = rec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED )		//우클릭 함수
			{																		//우클릭의 경우 xy값을 음수로 받는다.
				*x = rec.Event.MouseEvent.dwMousePosition.X*-1 -1;					//0,0이 겹치니까 -1을 또 해줌
				*y = rec.Event.MouseEvent.dwMousePosition.Y*-1 -1;
				return 1;
			}
		}
	}
	
}


void printDIf(int cc)                  //printdrawinterface
{										//인터페이스 출력함수
	int i;
	system("cls");		
	WHITE;
	gotoxy(0,yMax-1);
	printf("새노드");
	gotoxy(6,yMax-1);
	printf("  <  ");
	gotoxy(11,yMax-1);
	printf("  >  ");            
	gotoxy(18,yMax-1);
	printf("%d 페이지",cc);

	gotoxy(30,yMax-1);
	printf("삭 제");
	gotoxy(38,yMax-1);
	printf("재 생");
	gotoxy(46,yMax-1);
	printf("뒤 로");
	gotoxy(54,yMax-1);
	printf("부분복사");
	gotoxy(65,yMax-1);
	printf("전체복사");
	gotoxy(76,yMax-1);
	printf("붙여넣기");
	gotoxy(87,yMax-1);
	printf("다지우기");
	gotoxy(98,yMax-1);
	printf("저 장");
	gotoxy(106,yMax-1);
	printf("불러오기");

	gotoxy(xMax-3,2);
	WHITE;
	printf("*");
	gotoxy(xMax-3,4);
	RED;
	printf("*");
	gotoxy(xMax-3,6);
	BLUE;
	printf("*");
	gotoxy(xMax-3,8);
	YELLOW;
	printf("*");
	gotoxy(xMax-3,10);
	YELLOWGREEN;
	printf("*");
	gotoxy(xMax-3,15);
	WHITE;
	printf("*");
	gotoxy(xMax-3,17);
	printf("@");
	gotoxy(xMax-3,19);
	printf("#");
	gotoxy(xMax-3,21);
	printf("^");
	gotoxy(xMax-3,23);
	printf("!");
	GRAY;
	for(i=0;i<xMax;i+=2)				//인터페이스 창을 나누는 선 출력(X축)
	{
		gotoxy(i,yMax-3);
		printf("─");
	}

	for(i=0;i<=yMax;i++)				//Y축 창 출력
	{
		gotoxy(xMax-6,i);
		printf("│");
	}
	WHITE;

}


void playNode(listHead *L)		//노드재생
{
	int s,c=0;      //재생에 필요한 변수 선언
	listNode *temp;
	temp=L->head;      //첫 노드로 초기화
	system("cls");      //화면 초기화
	gotoxy(yMax,12);      //아래의 코드를 지정한 좌표에 출력하는 좌표 지정 함수
	printf("재생 간격(1/1000초) : ");      //재생간격 설정 (노드 넘어가는 지연시간)
	scanf("%d",&s);
	fflush(stdin);
	system("cls");   
	//while(temp != NULL)               //노드가 NULL이 아니면 반복
	while(1)
	{
		printNode2(temp->llink,temp);            //printimage 함수 실행
		gotoxy(0,yMax);                  //아래의 코드를 지정한 좌표에 출력
		printf("%d",++c);               //실행중 화면 번호 출력 (순서)

		temp=temp->rlink;            //노드를 다음 노드로 넘김
		Sleep(s);                     //입력받은 지연 시간 함수
		if(kbhit())
			break;
		if(temp==NULL)
		{
			system("cls");
			temp=L->head;
			c=0;
		}

	}
	fflush(stdin);                     //입력 버퍼 초기화

	gotoxy(46,yMax);                     //아래의 코드를 지정한 좌표에 출력
	printf("press enter");               //실행이 끝나면 아무키나 눌러서 함수를 빠져나감
	getchar();
	system("cls");                     //빠져나감과 동시에 화면을 지워줌
	printDIf(L->cc);
//	return temp;
}

listNode *copy(listNode *current)
{
	int x[2],y[2],i,j;
	listNode *temp;
	gotoxy(xMax/2-3, yMax);
	printf("복사시작");
	Sleep(300);
	MouseClick(&x[0],&y[0]);
	gotoxy(xMax/2-3, yMax);
	printf("복사시작2");
	Sleep(200);
	MouseClick(&x[1],&y[1]);
	temp=(listNode *)malloc(sizeof(listNode));

	for(i=0;i<pxMax;i++)
		for(j=0;j<pyMax;j++)
		{
			if(i<=x[1]-x[0]&&j<=y[1]-y[0])
			{
				temp->dot[i][j].pPat=current->dot[i+x[0]][j+y[0]].pPat;
				temp->dot[i][j].pCor=current->dot[i+x[0]][j+y[0]].pCor;
			}
			else
			{
				temp->dot[i][j].pPat=-1;
				temp->dot[i][j].pCor=-1;
			}
		}
		gotoxy(xMax/2-3, yMax);
		printf("(%d,%d)부터 (%d,%d) 복사",x[0],y[0],x[1],y[1]);
		return temp;

}

listNode* paste(listNode* current,listNode* temp)
{
	int i,j,x,y;
	gotoxy(xMax/2-3, yMax);
	printf("붙여넣기");
	Sleep(500);
	MouseClick(&x,&y);
	gotoxy(xMax/2-3, yMax);
	printf("(%d,%d)에 붙여넣기",x,y);

	for(i=0;i+x<pxMax;i++)
		for(j=0;j+y<pyMax;j++)			//임시저장노드에 잇는 값을 현재 좌표에 그대로 붙여넣기
		{
			if(temp->dot[i][j].pPat==-1||temp->dot[i][j].pCor==-1)
				break;
			current->dot[i+x][j+y].pPat=temp->dot[i][j].pPat;	
			current->dot[i+x][j+y].pCor=temp->dot[i][j].pCor;
		}
		return current;
}

void printDir()
{
	system("cls");
	gotoxy(0,0);
	printf("\n---------------------- C:\MyPicture\ -------------------\n\n");
	system("dir C:\MyPicture /b");
	printf("\n---------------------- C:\MyPicture\ -------------------\n\n");
}

void fileSave(listNode* hLink)
{
	FILE *fp;
	int x,y,i;
	char fname[20];
	listNode *temp;//=(listNode *)malloc(sizeof(listNode));

	temp=hLink;
	system("cls");
	gotoxy(1,yMax);
	printf("-저장하기:");

	strcpy(path,"C:\\MyPicture\\");	

	scanf("%s",fname);

	strcat(path,fname);

	if((fp = fopen(path, "w"))==NULL) 
	{
		printf("error");
		return;
	}
	
	while(temp!=NULL)
	{
		for(y=0;y<pyMax;y++)
		{	
			for(x=0;x<pxMax;x++)
			{
				fwrite(&temp->dot[x][y],sizeof(paint),1,fp);
			}
		}
		temp=temp->rlink;
	}
	printDir();
	gotoxy(1, yMax);
	printf("-%s 저장됨",path);
	getchar();
	getchar();
	fclose(fp);
}


listNode* fileLoad(listNode* hNode)
{
	FILE *fp;
	int x,y;
	char fname[20];
	listNode *temp;
	listNode *temp2=hNode;
	listNode *nheadN;
	int a=0;

	system("cls");
	strcpy(path,"C:\\MyPicture\\");			//path값 주기(경로설정

	//gotoxy(yMax,12);
	printDir();
	gotoxy(1,yMax);
	printf("불러올 파일:");		

	scanf("%s",fname);
	strcat(path,fname);						//path 랑 합치기
	fflush(stdin);

	if((fp=fopen(path,"r"))==NULL)
	{
		gotoxy(1, yMax);
		printf("-%s이(가) 없습니다",path);
		getchar();
		return hNode;
	}

	while(temp2->rlink!=NULL)		//기존에 사용하던 노드 메모리 해제
	{
		temp=temp2;
		temp2=temp2->rlink;		
		free(temp);
	}
	free(temp2);
	temp=(listNode *)malloc(sizeof(listNode));	//새로 사용할 노드의 첫번째 노드 정의
	temp->llink=NULL;
	nheadN=temp;
	while(!feof(fp))
	{
//		temp=(listNode *)malloc(sizeof(listNode));
		//printf("%s 불러옴");
		for(y=0;y<pyMax;y++)	
			for(x=0;x<pxMax;x++)
				fread(&temp->dot[x][y],sizeof(paint),1,fp);

		temp->rlink=(listNode *)malloc(sizeof(listNode));
		temp->rlink->llink=temp;
		temp=temp->rlink;
	}
	temp->llink->llink->rlink=NULL;			//NULL인 페이지 하나 더 불러오는 오류 발생
	free(temp->llink);						//페이지를 하나 더 삭제
	free(temp);	

	printDir();
	gotoxy(1, yMax);


	printf("-%s 불러옴   ",path);

	getchar();


	fclose(fp);

	return nheadN;

}


void drowing()				
{							//그리기함수
	int x,y,i,j;
	listHead *L;			//연결리스트
	listNode *current=NULL;	//현재노드
	listNode *temp=NULL;	//임시저장용노드
	int color=0,click=0;	//컬러(0==흰,1==빨,2==파,3==노)


	L = clistHead();		//연결리스트 생성
	L->cc=0;				//연결리스트 현재페이지 0으로 만듬

	current=addNode(L,current);		//노드 추가
	printDIf(L->cc);				//인터페이스 출력
	printNode2(NULL,current);			//그림 출력
	gotoxy(xMax-3,pyMax-1);
	printf("*");
	while( 1 )
	{	
		click= 0;
		WHITE;
		click=MouseClick(&x, &y);		//마우스 입력을 받음

		if(click)
		{
			if(x<0&&y<0)			//입력받은 값이 음수일 경우(우클릭의 경우)
			{
				x=(x+1)*-1 ;		//-1를 했기때문에 +1을 하고 양수로 다시 바꾸어준다
				y=(y+1)*-1 ;
				gotoxy(x, y);		//해당좌표로 이동 후 공백 출력
				printf(" ");
				current->dot[x][y].pPat=0;		//해당 이중배열에 0대입
			}
			else if( y >= yMax-3 )			//ymax 부터 3칸이하일경우(페이지 편집)
			{
				if( x <= 5 )		//노드추가
				{
					current=addNode(L,current);
					printNode2(current->llink,current);
				}

				if( x >= 6 && x <11&& current->llink!=NULL)   //왼노드로 이동
				{
					current=current->llink;
					L->cc--;
					printNode2(current->rlink,current);
				}
				if( x >= 11 && x <16 && current->rlink!=NULL)   //오른 노드로 이동
				{
					current=current->rlink;
					L->cc++;
					printNode2(current->llink,current);
				}
				if( x >= 30 && x <35)   //삭제
					current=deleteNode(L,current); //노드삭제
				if( x >= 38 && x <43 )	//재생
				{
					playNode(L);
					printDIf(L->cc);
					printNode2(NULL,current);
				}
				if( x >= 46 && x <51)   //뒤로
					return ;
				if( x>= 54 && x <62)	//부분복사
				{
					/*	temp=(listNode *)malloc(sizeof(listNode));		//임시저장노드에 메모리할당
					for(i=0;i<pxMax;i++)					//이중배열의 좌표값을 그대로 임시저장용 노드에 복사
					for(j=0;j<pyMax;j++)
					{
					temp->dot[i][j].pPat=current->dot[i][j].pPat;
					temp->dot[i][j].pCor=current->dot[i][j].pCor;
					}
					*/
					temp=copy(current);
				}
				if( x>= 65 && x <73)	//전체복사
				{
					gotoxy(xMax/2-3, yMax);
					printf("전체 복사됨");
					temp=(listNode *)malloc(sizeof(listNode));		//임시저장노드에 메모리할당
					for(i=0;i<pxMax;i++)					//이중배열의 좌표값을 그대로 임시저장용 노드에 복사
					for(j=0;j<pyMax;j++)
					{
					temp->dot[i][j].pPat=current->dot[i][j].pPat;
					temp->dot[i][j].pCor=current->dot[i][j].pCor;
					}
				}
				if( x>= 76 && x <84 &&temp!=NULL)	//붙여넣기(임시저장이 들어있어야함)
				{
					paste(current,temp);
					printDIf(L->cc);
					//printimage(L,current);
					printNode2(NULL,current);
				}
				if( x>= 87 && x <95)	//다지우기
				{
					for(i=0;i<pxMax;i++)
						for(j=0;j<pyMax;j++)		//0으로 초기화
						{
							current->dot[i][j].pPat=0;
							current->dot[i][j].pCor=0;
						}
						system("cls");
						printDIf(L->cc);

				}
				if( x>= 98 && x <103)	//저장
				{
					fileSave(L->head);
					system("cls");
					printDIf(L->cc);
					printNode2(NULL,current);
				}
				if( x>= 106 && x <114)	//불러오기
				{
					L->head=fileLoad(L->head);
					current=L->head;
					system("cls");
					printDIf(L->cc=1);
					printNode2(NULL,L->head);
				}
				/*if( x>= 104 && x <111)	//1장 불러오기
				{
					fileLoad2(current);
					system("cls");
					printDIf(L->cc);
					printNode2(NULL,current);
				}*/
				//				printDIf(L->cc);		//인터페이스 출력
				//				printNode2(NULL,current);
				Sleep(100);   //연속클릭 방지(
			}

			else if(x>=xMax-6)	//XMAX부터 6칸이내(펜 옵션 설정)
			{
				if(x==xMax-3)	//X가 XMAX-3 번째 좌표일경우
				{
					switch(y)		//Y좌표 값에 따른 스위치
					{
					case 2 :	//스위치에 따라 컬러 변수 값 수정
						color=0;
						break;
					case 4 :
						color=1;
						break;
					case 6 :
						color=2;
						break;
					case 8 :
						color=3;
						break;
					case 10 :
						color=4;
						break;
					case 15 :
						pat='*';
						break;
					case 17 :
						pat='@';
						break;
					case 19 :
						pat='#';
						break;
					case 21 :
						pat='^';
						break;
					case 23 :
						pat='!';
						break;
					}
					gotoxy(xMax-3,pyMax-1);
					switch(color)	//컬러 변수 값에 따라
					{
					case 0 : WHITE;		//색 변경
						break;
					case 1 : RED ;
						break;
					case 2 : BLUE ;
						break;
					case 3 : YELLOW ;
						break;
					case 4 : YELLOWGREEN;
						break;
					}
					printf("%c",pat);
					WHITE;
				}


			}
			else		//특별한 좌표를 누른것이 아닐때(일반 그리기)
			{
				gotoxy(x, y);
				switch(color)	//컬러 변수 값에 따라
				{
				case 0 : WHITE;		//색 변경
					current->dot[x][y].pCor='0';
					break;
				case 1 : RED ;
					current->dot[x][y].pCor='1';
					break;
				case 2 : BLUE ;
					current->dot[x][y].pCor='2';
					break;
				case 3 : YELLOW ;
					current->dot[x][y].pCor='3';
					break;
				case 4 : YELLOWGREEN ;
					current->dot[x][y].pCor='4';
					break;
				}
				printf("%c",pat);		//색변경된 * 출력

				current->dot[x][y].pPat=pat;	//현재노드의 좌표값에 *입력
				WHITE;
			}
		}
		gotoxy(18,yMax-1);
		printf("%d 페이지",L->cc);
	}
}





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


int sendf()
{
	int retval;
	int MODE=SAVEMODE;
	char fname[20];

	system("cls");
	printDir();
	printf("-업로드할 파일:");

	scanf("%s",fname);//짧은 접속시간을 유지하기 위해서 미리 파일이름받기



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

	send(sock,(char *)&MODE,sizeof(int),0); // 저장 모드라고 알려줌

	// 데이터 통신에 사용할 변수
	char *buf;
	FILE *fp;


	int fsize=0;

	while(1){
		strcpy(path,"C:\\MyPicture\\");	


		strcat(path,fname);
		//printf("%s ",path);
		// 서버와 데이터 통신
		
		if((fp=fopen(path,"r"))!=NULL)
			break;	
		
		printf("-업로드할 파일:");
		printf("%s 이 없습니다. 재입력 : ",path);
		scanf("%s",fname);
		fflush(stdin);
//		return 0;

	}

	printf("%s 로드, 서버에 저장할 이름 입력 :",path);
	
	scanf("%s",fname);
	fflush(stdin);

	send(sock,fname,sizeof(fname),0);


	// fseek와 ftell을 통해 파일크기 구하기
	fseek(fp, 0, SEEK_END);   // 파일포인터가 파일의 마지막
	fsize = ftell(fp);        // 파일크기를 fsize에 전달
	fseek(fp, 0, SEEK_SET);   // 파일포인터가 초기화

	retval=send(sock, (char*)&fsize, sizeof(fsize), 0); // 파일사이즈 보내기


	buf = (char*)malloc(fsize);
	memset(buf, 0, fsize);// 초기화

	fread(buf, 1, fsize, fp);			//파일 저장

	retval=send(sock, buf, fsize, 0);
	printf("%s 저장완료! ",fname);

	getchar();

	free(buf);
	fclose(fp);

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;

}

int loadf()
{
	int retval;
	int MODE=LOADMODE;					//로드모드
	int tF=0;							//서버에 존재하는파일인지 확인
	char fname[20];						//파일이름

	system("cls");
	printf("-서버에서 불러올 파일:");
	scanf("%s",fname);//짧은 접속시간을 유지하기 위해서 미리 파일이름받기
	fflush(stdin);


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


	send(sock,(char *)&MODE,sizeof(int),0); // 사용할 모드를 전송(저장)

	while(1)
	{
		send(sock,fname,sizeof(fname),0);		//무엇을 로드하는지 서버에 알려쥼
	
		recv(sock, (char *)&tF, sizeof(int), 0);// 그파일이 서버에 존재하는지 여부 재전송
		if(tF==1)
			break;
		printf("-서버에 %s 파일이 없습니다. 재입력:",fname);
		scanf("%s",fname);
		fflush(stdin);
	}



	// 데이터 통신에 사용할 변수
	char *buf;
	FILE *fp;
	int fsize=0;
	//char path[50]="C:\\MyPicture\\";

	retval = recv(sock, (char *)&fsize, sizeof(int), 0);		//파일사이즈받기

	buf = (char*)malloc(fsize);
	memset(buf, 0, fsize);			//버퍼초기화

	retval = recv(sock, buf, fsize, 0);							//파일받기
	if(retval == SOCKET_ERROR){
		err_display("recv()");
		return 0;
	}
	else if(retval == 0)
		return 0;
	printf("%d, %d \n",fsize,retval);

	printf("%s 로드, 드라이브에 저장할 이름 입력 :",fname);

	scanf("%s",fname);
	fflush(stdin);

	strcpy(path,"C:\\MyPicture\\");	
	strcat(path,fname);

	//printf("%s ",path);
	// 서버와 데이터 통신
	
	if((fp=fopen(path,"w"))==NULL)
	{
		printf("error");
		exit(1);
	}
	//printf("%d",(int)fwrite(buf, 1, fsize, fp));			//
	fwrite(buf, 1, fsize, fp);
	printDir();
	printf("%s 저장완료!!\n",path);
	getchar();

	free(buf);
	fclose(fp);

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;

}

int serverMenu()		//메뉴함수
{
	int x,y;

	while(1)
	{
		system("cls");
		WHITE;
		//gotoxy(xMax/2-8,yMax/2-1);//해당 좌표에 메뉴 출력
		gotoxy(xMax/2-6,yMax/2-3);//해당 좌표에 메뉴 출력
		printf("다 운  로 드 \n");

		gotoxy(xMax/2-5,yMax/2);//해당 좌표에 메뉴 출력
		printf(" 업 로 드 \n");

		gotoxy(xMax/2-4,yMax/2+3);
		printf(" 뒤  로 \n");
		MouseClick(&x, &y);		//마우스 입력을 받음
		if(x>=xMax/2-4||x<=xMax/2+4)		//해당 좌표일 경우에 해당 메뉴 실행
		{
			if(y==yMax/2-3)
				loadf();
			if(y==yMax/2)
				sendf();
			if(y==yMax/2+3)
				return 0;
		}

	}
}

int menu()		//메뉴함수
{
	int x,y;
	//listNode *curr=NULL;
	//커서 안보이게하는 함수
	HANDLE hConsole; // 콘솔 핸들
	CONSOLE_CURSOR_INFO ConsoleCursor; // 콘솔커서 정보 구조체
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 핸들을 구하고
	ConsoleCursor.bVisible = 0; // true 보임 , false 안보임
	ConsoleCursor.dwSize = 1; // 커서 사이즈
	SetConsoleCursorInfo(hConsole , &ConsoleCursor); // 설정

	if(mkdir("C:\\MyPicture\\")==0)
		printf("C:\\MyPicture\\ 폴더를 새로 생성!!!");

	while(1)
	{
		system("cls");
		WHITE;
		//gotoxy(xMax/2-8,yMax/2-1);//해당 좌표에 메뉴 출력
		gotoxy(xMax/2-8,yMax/2-3);//해당 좌표에 메뉴 출력
		printf("그 림   그 리 기\n");

		gotoxy(xMax/2-6,yMax/2);//해당 좌표에 메뉴 출력
		printf("서 버  접 속 \n");

		gotoxy(xMax/2-4,yMax/2+3);
		printf(" 종  료 \n");
		MouseClick(&x, &y);		//마우스 입력을 받음
		if(x>=xMax/2-4||x<=xMax/2+4)		//해당 좌표일 경우에 해당 메뉴 실행
		{
			if(y==yMax/2-3)
				drowing();
			if(y==yMax/2)
				serverMenu();
			if(y==yMax/2+3)
				exit(1);
		}

	}
}


int main(int argc, char *argv[])
{
	system("mode con:cols=120 lines=33");	//콘솔창 크기 변경

	DWORD prevMode = 0;							//빠른편집모드 해제 명령
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(handle, &prevMode);
	SetConsoleMode(handle, prevMode & ~ENABLE_QUICK_EDIT_MODE);

	while(1)
		menu();
}
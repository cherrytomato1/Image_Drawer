
#include "stdafx.h"
#include "myheader.h"


#pragma comment(lib, "ws2_32")
#include <winsock2.h>

#include<stdio.h>
#include<Windows.h>

#include<stdlib.h>
#include<conio.h>

#define xMax 120			//�׸��� x�ִ�
#define yMax 32				//y�ִ�

#define pxMax xMax-6
#define pyMax yMax-3

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9500
#define BUFSIZE    50

#define SAVEMODE 1
#define LOADMODE 2


#define col GetStdHandle (STD_OUTPUT_HANDLE)	//���� ������ ����

#define RED SetConsoleTextAttribute( col,0x000c); // ������
#define BLUE SetConsoleTextAttribute( col,0x0001 | 0x0008); // �Ķ���
#define YELLOWGREEN SetConsoleTextAttribute( col,0x00a); // ���λ�
#define WHITE SetConsoleTextAttribute( col,0x000f); // ���
#define SKY SetConsoleTextAttribute( col,0x000b); // �ϴû�
#define YELLOW SetConsoleTextAttribute( col,0x000e); // �����
#define FLUORESCENT SetConsoleTextAttribute( col,0x000d); // ������
#define GRAY SetConsoleTextAttribute( col,0x0008); // ȸ��

char pat='*';
char path[50];

typedef struct{			//�׸��� �� ��
	char pPat;			//���
	int pCor;			//��
}paint;

typedef struct listnode{		//�׸��� �� ������(���)
	paint dot[pxMax][pyMax];		//�׸� ��ǥ
	struct listnode *llink;		//����ũ
	struct listnode *rlink;		//�˸�ũ
}listNode;

typedef struct{					//���Ḯ��Ʈ ���
	int cc;						//���� ������ ��
	listNode *head;				//��帵ũ
}listHead;



listHead* clistHead()		//���Ḯ��Ʈ ������
{
	listHead* L;
	L= (listHead*)malloc(sizeof(listHead));
	L->head = NULL;
	return L;
}

listNode* addNode(listHead *L,listNode *current)		//���Ḯ��Ʈ ��� �߰�
{
	int i,j;

	listNode* newNode;
	newNode = (listNode *)malloc(sizeof(listNode));		//�޸��Ҵ�

	for(i=0;i<pxMax;i++)					//�׸���ǥ �ʱ�ȭ
		for(j=0;j<pyMax;j++)
		{
			newNode->dot[i][j].pPat=0;
			newNode->dot[i][j].pCor=0;
		}
		L->cc++;					//���������� ++
		if(L->head ==NULL)		//���鸮��Ʈ�϶�
		{
			newNode->llink= NULL;
			newNode->rlink= NULL;
			L->head =newNode;

			return newNode;
		}
		if(current->rlink!=NULL)		//���� ��尡 �������� �ƴҶ�
			current->rlink->llink=newNode;
		newNode->llink= current;
		newNode->rlink= current->rlink;
		current->rlink= newNode;

		return newNode;

}
void gotoxy(int x, int y)			//������������
{
	COORD Pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void printNode2(listNode *previous,listNode *current)
{
	int i,j;
	if(previous==NULL)
	{
		for(i=0;i<pxMax;i++)								//��ǥ�� �˻�
			for(j=0;j<pyMax;j++)
			{
				if(current->dot[i][j].pPat!=0&&current->dot[i][j].pPat!=-1)		//*������ �������
				{
					switch(current->dot[i][j].pCor)		//�÷��� ���� ����ġ
					{
					case '0' : WHITE;					//�� �÷� �Լ� ����
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

					gotoxy(i,j);						//�ش���ǥ�̵� �� ���
					printf("%c",current->dot[i][j].pPat);
					WHITE;
				}
			}
	}
	else
	{
		for(i=0;i<pxMax;i++)								//��ǥ�� �˻�
			for(j=0;j<pyMax;j++)
				if(previous->dot[i][j].pPat != current->dot[i][j].pPat || previous->dot[i][j].pCor!=current->dot[i][j].pCor)
				{
					if(current->dot[i][j].pPat==0)
					{
						gotoxy(i,j);						//�ش���ǥ�̵� �� ���
						printf(" ");
						WHITE;
					}
					switch(current->dot[i][j].pCor)		//�÷��� ���� ����ġ
					{
					case '0' : WHITE;					//�� �÷� �Լ� ����
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
					gotoxy(i,j);						//�ش���ǥ�̵� �� ���
					printf("%c",current->dot[i][j].pPat);
					WHITE;
				}
	}
}


listNode* deleteNode(listHead *L,listNode *current)		//������
{
	listNode *temp;
	if(current->llink==NULL)				//ù��° �������϶�
	{
		if(current->rlink==NULL)		//������ �������϶�
		{
			gotoxy(45,yMax);
			printf("������ �������Դϴ�.");
			return current;			//�������ϰ� ����
		}
		printNode2(current,current->rlink);
		temp=current;				
		L->head=current->rlink;		//���� ������ ��带 ����
		current->rlink->llink=NULL;	//�����ʳ���� ����ũ�� �η�
		current=current->rlink;

		printNode2(current->llink,current);
		free(temp);					//�޸�����
		return current;				//���� ��� ����
	}
	else if(current->rlink==NULL)	
		current->llink->rlink=NULL;		//������ ����ϰ��	
	else
	{
		current->rlink->llink=current->llink;		//�ƴҰ��
		current->llink->rlink=current->rlink;
	}
	printNode2(current,current->llink);
	temp=current;			
	current=current->llink;	//����޸� �̵�
	L->cc--;			//���� �������� ����
	free(temp);		//�޸�����
	return current;		//���� ��� ����
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
			if( tempY = rec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED )		//��Ŭ�� �Լ�
			{																		//��Ŭ���� ��� xy���� ������ �޴´�.
				*x = rec.Event.MouseEvent.dwMousePosition.X*-1 -1;					//0,0�� ��ġ�ϱ� -1�� �� ����
				*y = rec.Event.MouseEvent.dwMousePosition.Y*-1 -1;
				return 1;
			}
		}
	}
	
}


void printDIf(int cc)                  //printdrawinterface
{										//�������̽� ����Լ�
	int i;
	system("cls");		
	WHITE;
	gotoxy(0,yMax-1);
	printf("�����");
	gotoxy(6,yMax-1);
	printf("  <  ");
	gotoxy(11,yMax-1);
	printf("  >  ");            
	gotoxy(18,yMax-1);
	printf("%d ������",cc);

	gotoxy(30,yMax-1);
	printf("�� ��");
	gotoxy(38,yMax-1);
	printf("�� ��");
	gotoxy(46,yMax-1);
	printf("�� ��");
	gotoxy(54,yMax-1);
	printf("�κк���");
	gotoxy(65,yMax-1);
	printf("��ü����");
	gotoxy(76,yMax-1);
	printf("�ٿ��ֱ�");
	gotoxy(87,yMax-1);
	printf("�������");
	gotoxy(98,yMax-1);
	printf("�� ��");
	gotoxy(106,yMax-1);
	printf("�ҷ�����");

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
	for(i=0;i<xMax;i+=2)				//�������̽� â�� ������ �� ���(X��)
	{
		gotoxy(i,yMax-3);
		printf("��");
	}

	for(i=0;i<=yMax;i++)				//Y�� â ���
	{
		gotoxy(xMax-6,i);
		printf("��");
	}
	WHITE;

}


void playNode(listHead *L)		//������
{
	int s,c=0;      //����� �ʿ��� ���� ����
	listNode *temp;
	temp=L->head;      //ù ���� �ʱ�ȭ
	system("cls");      //ȭ�� �ʱ�ȭ
	gotoxy(yMax,12);      //�Ʒ��� �ڵ带 ������ ��ǥ�� ����ϴ� ��ǥ ���� �Լ�
	printf("��� ����(1/1000��) : ");      //������� ���� (��� �Ѿ�� �����ð�)
	scanf("%d",&s);
	fflush(stdin);
	system("cls");   
	//while(temp != NULL)               //��尡 NULL�� �ƴϸ� �ݺ�
	while(1)
	{
		printNode2(temp->llink,temp);            //printimage �Լ� ����
		gotoxy(0,yMax);                  //�Ʒ��� �ڵ带 ������ ��ǥ�� ���
		printf("%d",++c);               //������ ȭ�� ��ȣ ��� (����)

		temp=temp->rlink;            //��带 ���� ���� �ѱ�
		Sleep(s);                     //�Է¹��� ���� �ð� �Լ�
		if(kbhit())
			break;
		if(temp==NULL)
		{
			system("cls");
			temp=L->head;
			c=0;
		}

	}
	fflush(stdin);                     //�Է� ���� �ʱ�ȭ

	gotoxy(46,yMax);                     //�Ʒ��� �ڵ带 ������ ��ǥ�� ���
	printf("press enter");               //������ ������ �ƹ�Ű�� ������ �Լ��� ��������
	getchar();
	system("cls");                     //���������� ���ÿ� ȭ���� ������
	printDIf(L->cc);
//	return temp;
}

listNode *copy(listNode *current)
{
	int x[2],y[2],i,j;
	listNode *temp;
	gotoxy(xMax/2-3, yMax);
	printf("�������");
	Sleep(300);
	MouseClick(&x[0],&y[0]);
	gotoxy(xMax/2-3, yMax);
	printf("�������2");
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
		printf("(%d,%d)���� (%d,%d) ����",x[0],y[0],x[1],y[1]);
		return temp;

}

listNode* paste(listNode* current,listNode* temp)
{
	int i,j,x,y;
	gotoxy(xMax/2-3, yMax);
	printf("�ٿ��ֱ�");
	Sleep(500);
	MouseClick(&x,&y);
	gotoxy(xMax/2-3, yMax);
	printf("(%d,%d)�� �ٿ��ֱ�",x,y);

	for(i=0;i+x<pxMax;i++)
		for(j=0;j+y<pyMax;j++)			//�ӽ������忡 �մ� ���� ���� ��ǥ�� �״�� �ٿ��ֱ�
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
	printf("-�����ϱ�:");

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
	printf("-%s �����",path);
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
	strcpy(path,"C:\\MyPicture\\");			//path�� �ֱ�(��μ���

	//gotoxy(yMax,12);
	printDir();
	gotoxy(1,yMax);
	printf("�ҷ��� ����:");		

	scanf("%s",fname);
	strcat(path,fname);						//path �� ��ġ��
	fflush(stdin);

	if((fp=fopen(path,"r"))==NULL)
	{
		gotoxy(1, yMax);
		printf("-%s��(��) �����ϴ�",path);
		getchar();
		return hNode;
	}

	while(temp2->rlink!=NULL)		//������ ����ϴ� ��� �޸� ����
	{
		temp=temp2;
		temp2=temp2->rlink;		
		free(temp);
	}
	free(temp2);
	temp=(listNode *)malloc(sizeof(listNode));	//���� ����� ����� ù��° ��� ����
	temp->llink=NULL;
	nheadN=temp;
	while(!feof(fp))
	{
//		temp=(listNode *)malloc(sizeof(listNode));
		//printf("%s �ҷ���");
		for(y=0;y<pyMax;y++)	
			for(x=0;x<pxMax;x++)
				fread(&temp->dot[x][y],sizeof(paint),1,fp);

		temp->rlink=(listNode *)malloc(sizeof(listNode));
		temp->rlink->llink=temp;
		temp=temp->rlink;
	}
	temp->llink->llink->rlink=NULL;			//NULL�� ������ �ϳ� �� �ҷ����� ���� �߻�
	free(temp->llink);						//�������� �ϳ� �� ����
	free(temp);	

	printDir();
	gotoxy(1, yMax);


	printf("-%s �ҷ���   ",path);

	getchar();


	fclose(fp);

	return nheadN;

}


void drowing()				
{							//�׸����Լ�
	int x,y,i,j;
	listHead *L;			//���Ḯ��Ʈ
	listNode *current=NULL;	//������
	listNode *temp=NULL;	//�ӽ��������
	int color=0,click=0;	//�÷�(0==��,1==��,2==��,3==��)


	L = clistHead();		//���Ḯ��Ʈ ����
	L->cc=0;				//���Ḯ��Ʈ ���������� 0���� ����

	current=addNode(L,current);		//��� �߰�
	printDIf(L->cc);				//�������̽� ���
	printNode2(NULL,current);			//�׸� ���
	gotoxy(xMax-3,pyMax-1);
	printf("*");
	while( 1 )
	{	
		click= 0;
		WHITE;
		click=MouseClick(&x, &y);		//���콺 �Է��� ����

		if(click)
		{
			if(x<0&&y<0)			//�Է¹��� ���� ������ ���(��Ŭ���� ���)
			{
				x=(x+1)*-1 ;		//-1�� �߱⶧���� +1�� �ϰ� ����� �ٽ� �ٲپ��ش�
				y=(y+1)*-1 ;
				gotoxy(x, y);		//�ش���ǥ�� �̵� �� ���� ���
				printf(" ");
				current->dot[x][y].pPat=0;		//�ش� ���߹迭�� 0����
			}
			else if( y >= yMax-3 )			//ymax ���� 3ĭ�����ϰ��(������ ����)
			{
				if( x <= 5 )		//����߰�
				{
					current=addNode(L,current);
					printNode2(current->llink,current);
				}

				if( x >= 6 && x <11&& current->llink!=NULL)   //�޳��� �̵�
				{
					current=current->llink;
					L->cc--;
					printNode2(current->rlink,current);
				}
				if( x >= 11 && x <16 && current->rlink!=NULL)   //���� ���� �̵�
				{
					current=current->rlink;
					L->cc++;
					printNode2(current->llink,current);
				}
				if( x >= 30 && x <35)   //����
					current=deleteNode(L,current); //������
				if( x >= 38 && x <43 )	//���
				{
					playNode(L);
					printDIf(L->cc);
					printNode2(NULL,current);
				}
				if( x >= 46 && x <51)   //�ڷ�
					return ;
				if( x>= 54 && x <62)	//�κк���
				{
					/*	temp=(listNode *)malloc(sizeof(listNode));		//�ӽ������忡 �޸��Ҵ�
					for(i=0;i<pxMax;i++)					//���߹迭�� ��ǥ���� �״�� �ӽ������ ��忡 ����
					for(j=0;j<pyMax;j++)
					{
					temp->dot[i][j].pPat=current->dot[i][j].pPat;
					temp->dot[i][j].pCor=current->dot[i][j].pCor;
					}
					*/
					temp=copy(current);
				}
				if( x>= 65 && x <73)	//��ü����
				{
					gotoxy(xMax/2-3, yMax);
					printf("��ü �����");
					temp=(listNode *)malloc(sizeof(listNode));		//�ӽ������忡 �޸��Ҵ�
					for(i=0;i<pxMax;i++)					//���߹迭�� ��ǥ���� �״�� �ӽ������ ��忡 ����
					for(j=0;j<pyMax;j++)
					{
					temp->dot[i][j].pPat=current->dot[i][j].pPat;
					temp->dot[i][j].pCor=current->dot[i][j].pCor;
					}
				}
				if( x>= 76 && x <84 &&temp!=NULL)	//�ٿ��ֱ�(�ӽ������� ����־����)
				{
					paste(current,temp);
					printDIf(L->cc);
					//printimage(L,current);
					printNode2(NULL,current);
				}
				if( x>= 87 && x <95)	//�������
				{
					for(i=0;i<pxMax;i++)
						for(j=0;j<pyMax;j++)		//0���� �ʱ�ȭ
						{
							current->dot[i][j].pPat=0;
							current->dot[i][j].pCor=0;
						}
						system("cls");
						printDIf(L->cc);

				}
				if( x>= 98 && x <103)	//����
				{
					fileSave(L->head);
					system("cls");
					printDIf(L->cc);
					printNode2(NULL,current);
				}
				if( x>= 106 && x <114)	//�ҷ�����
				{
					L->head=fileLoad(L->head);
					current=L->head;
					system("cls");
					printDIf(L->cc=1);
					printNode2(NULL,L->head);
				}
				/*if( x>= 104 && x <111)	//1�� �ҷ�����
				{
					fileLoad2(current);
					system("cls");
					printDIf(L->cc);
					printNode2(NULL,current);
				}*/
				//				printDIf(L->cc);		//�������̽� ���
				//				printNode2(NULL,current);
				Sleep(100);   //����Ŭ�� ����(
			}

			else if(x>=xMax-6)	//XMAX���� 6ĭ�̳�(�� �ɼ� ����)
			{
				if(x==xMax-3)	//X�� XMAX-3 ��° ��ǥ�ϰ��
				{
					switch(y)		//Y��ǥ ���� ���� ����ġ
					{
					case 2 :	//����ġ�� ���� �÷� ���� �� ����
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
					switch(color)	//�÷� ���� ���� ����
					{
					case 0 : WHITE;		//�� ����
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
			else		//Ư���� ��ǥ�� �������� �ƴҶ�(�Ϲ� �׸���)
			{
				gotoxy(x, y);
				switch(color)	//�÷� ���� ���� ����
				{
				case 0 : WHITE;		//�� ����
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
				printf("%c",pat);		//������� * ���

				current->dot[x][y].pPat=pat;	//�������� ��ǥ���� *�Է�
				WHITE;
			}
		}
		gotoxy(18,yMax-1);
		printf("%d ������",L->cc);
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


int sendf()
{
	int retval;
	int MODE=SAVEMODE;
	char fname[20];

	system("cls");
	printDir();
	printf("-���ε��� ����:");

	scanf("%s",fname);//ª�� ���ӽð��� �����ϱ� ���ؼ� �̸� �����̸��ޱ�



	// ���� �ʱ�ȭ
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

	send(sock,(char *)&MODE,sizeof(int),0); // ���� ����� �˷���

	// ������ ��ſ� ����� ����
	char *buf;
	FILE *fp;


	int fsize=0;

	while(1){
		strcpy(path,"C:\\MyPicture\\");	


		strcat(path,fname);
		//printf("%s ",path);
		// ������ ������ ���
		
		if((fp=fopen(path,"r"))!=NULL)
			break;	
		
		printf("-���ε��� ����:");
		printf("%s �� �����ϴ�. ���Է� : ",path);
		scanf("%s",fname);
		fflush(stdin);
//		return 0;

	}

	printf("%s �ε�, ������ ������ �̸� �Է� :",path);
	
	scanf("%s",fname);
	fflush(stdin);

	send(sock,fname,sizeof(fname),0);


	// fseek�� ftell�� ���� ����ũ�� ���ϱ�
	fseek(fp, 0, SEEK_END);   // ���������Ͱ� ������ ������
	fsize = ftell(fp);        // ����ũ�⸦ fsize�� ����
	fseek(fp, 0, SEEK_SET);   // ���������Ͱ� �ʱ�ȭ

	retval=send(sock, (char*)&fsize, sizeof(fsize), 0); // ���ϻ����� ������


	buf = (char*)malloc(fsize);
	memset(buf, 0, fsize);// �ʱ�ȭ

	fread(buf, 1, fsize, fp);			//���� ����

	retval=send(sock, buf, fsize, 0);
	printf("%s ����Ϸ�! ",fname);

	getchar();

	free(buf);
	fclose(fp);

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;

}

int loadf()
{
	int retval;
	int MODE=LOADMODE;					//�ε���
	int tF=0;							//������ �����ϴ��������� Ȯ��
	char fname[20];						//�����̸�

	system("cls");
	printf("-�������� �ҷ��� ����:");
	scanf("%s",fname);//ª�� ���ӽð��� �����ϱ� ���ؼ� �̸� �����̸��ޱ�
	fflush(stdin);


	// ���� �ʱ�ȭ
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


	send(sock,(char *)&MODE,sizeof(int),0); // ����� ��带 ����(����)

	while(1)
	{
		send(sock,fname,sizeof(fname),0);		//������ �ε��ϴ��� ������ �˷���
	
		recv(sock, (char *)&tF, sizeof(int), 0);// �������� ������ �����ϴ��� ���� ������
		if(tF==1)
			break;
		printf("-������ %s ������ �����ϴ�. ���Է�:",fname);
		scanf("%s",fname);
		fflush(stdin);
	}



	// ������ ��ſ� ����� ����
	char *buf;
	FILE *fp;
	int fsize=0;
	//char path[50]="C:\\MyPicture\\";

	retval = recv(sock, (char *)&fsize, sizeof(int), 0);		//���ϻ�����ޱ�

	buf = (char*)malloc(fsize);
	memset(buf, 0, fsize);			//�����ʱ�ȭ

	retval = recv(sock, buf, fsize, 0);							//���Ϲޱ�
	if(retval == SOCKET_ERROR){
		err_display("recv()");
		return 0;
	}
	else if(retval == 0)
		return 0;
	printf("%d, %d \n",fsize,retval);

	printf("%s �ε�, ����̺꿡 ������ �̸� �Է� :",fname);

	scanf("%s",fname);
	fflush(stdin);

	strcpy(path,"C:\\MyPicture\\");	
	strcat(path,fname);

	//printf("%s ",path);
	// ������ ������ ���
	
	if((fp=fopen(path,"w"))==NULL)
	{
		printf("error");
		exit(1);
	}
	//printf("%d",(int)fwrite(buf, 1, fsize, fp));			//
	fwrite(buf, 1, fsize, fp);
	printDir();
	printf("%s ����Ϸ�!!\n",path);
	getchar();

	free(buf);
	fclose(fp);

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;

}

int serverMenu()		//�޴��Լ�
{
	int x,y;

	while(1)
	{
		system("cls");
		WHITE;
		//gotoxy(xMax/2-8,yMax/2-1);//�ش� ��ǥ�� �޴� ���
		gotoxy(xMax/2-6,yMax/2-3);//�ش� ��ǥ�� �޴� ���
		printf("�� ��  �� �� \n");

		gotoxy(xMax/2-5,yMax/2);//�ش� ��ǥ�� �޴� ���
		printf(" �� �� �� \n");

		gotoxy(xMax/2-4,yMax/2+3);
		printf(" ��  �� \n");
		MouseClick(&x, &y);		//���콺 �Է��� ����
		if(x>=xMax/2-4||x<=xMax/2+4)		//�ش� ��ǥ�� ��쿡 �ش� �޴� ����
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

int menu()		//�޴��Լ�
{
	int x,y;
	//listNode *curr=NULL;
	//Ŀ�� �Ⱥ��̰��ϴ� �Լ�
	HANDLE hConsole; // �ܼ� �ڵ�
	CONSOLE_CURSOR_INFO ConsoleCursor; // �ܼ�Ŀ�� ���� ����ü
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // �ڵ��� ���ϰ�
	ConsoleCursor.bVisible = 0; // true ���� , false �Ⱥ���
	ConsoleCursor.dwSize = 1; // Ŀ�� ������
	SetConsoleCursorInfo(hConsole , &ConsoleCursor); // ����

	if(mkdir("C:\\MyPicture\\")==0)
		printf("C:\\MyPicture\\ ������ ���� ����!!!");

	while(1)
	{
		system("cls");
		WHITE;
		//gotoxy(xMax/2-8,yMax/2-1);//�ش� ��ǥ�� �޴� ���
		gotoxy(xMax/2-8,yMax/2-3);//�ش� ��ǥ�� �޴� ���
		printf("�� ��   �� �� ��\n");

		gotoxy(xMax/2-6,yMax/2);//�ش� ��ǥ�� �޴� ���
		printf("�� ��  �� �� \n");

		gotoxy(xMax/2-4,yMax/2+3);
		printf(" ��  �� \n");
		MouseClick(&x, &y);		//���콺 �Է��� ����
		if(x>=xMax/2-4||x<=xMax/2+4)		//�ش� ��ǥ�� ��쿡 �ش� �޴� ����
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
	system("mode con:cols=120 lines=33");	//�ܼ�â ũ�� ����

	DWORD prevMode = 0;							//����������� ���� ���
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(handle, &prevMode);
	SetConsoleMode(handle, prevMode & ~ENABLE_QUICK_EDIT_MODE);

	while(1)
		menu();
}
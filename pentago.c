#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define WHITE 'W'
#define BLACK 'B'

#if defined(_WIN32) || defined(_WIN64)
    const char* clear = "cls";
#else
    const char* clear = "clear";
#endif
void genBoard(char arr[4][9])
{
	int i, j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<9;j++)
		{
			arr[i][j]= ' ';
		}
	}
}
void rotateC(char arr[4][9], int sector)
{
	char nboard[4][9];
	genBoard(nboard);
	int i, j, k;
	for(i=0;i<4;i++)
	{
		if (i != sector)
		{
			for(j=0;j<9;j++)
			{
				nboard[i][j]=arr[i][j];
			}
		}
	}
	nboard[sector][2] = arr[sector][0];
	nboard[sector][5] = arr[sector][1];
	nboard[sector][8] = arr[sector][2];
	nboard[sector][1] = arr[sector][3];
	nboard[sector][4] = arr[sector][4];
	nboard[sector][7] = arr[sector][5];
	nboard[sector][0] = arr[sector][6];
	nboard[sector][3] = arr[sector][7];
	nboard[sector][6] = arr[sector][8];
	for(k=0;k<9;k++)
	{
		arr[sector][k]=nboard[sector][k];
	}
}
void rotateCC(char arr[4][9], int sector)
{
	char nboard[4][9];
	genBoard(nboard);
	int i, j, k;
	for(i=0;i<4;i++)
	{
		if (i != sector)
		{
			for(j=0;j<9;j++)
			{
				nboard[i][j]=arr[i][j];
			}
		}
	}
	nboard[sector][0] = arr[sector][2];
	nboard[sector][1] = arr[sector][5];
	nboard[sector][2] = arr[sector][8];
	nboard[sector][3] = arr[sector][1];
	nboard[sector][4] = arr[sector][4];
	nboard[sector][5] = arr[sector][7];
	nboard[sector][6] = arr[sector][0];
	nboard[sector][7] = arr[sector][3];
	nboard[sector][8] = arr[sector][6];
	for(k=0;k<9;k++)
	{
		arr[sector][k]=nboard[sector][k];
	}
}
void rotateSquare(char arr[4][9])
{
	int pass0, pass1, pass2, pass3;
	int square, mark;
	pass3 = 0;
	char rawrot[] = "A'";
	while (pass3 == 0)
	{
		pass0 = 0;
		pass1 = 0;
		pass2 = 0;
		printf("Rotation (ie. A' or C\"): ");
		fgets(rawrot, 3, stdin);
		if (rawrot[strlen(rawrot) -1] != '\n')
		{
			int dropped = 0;
			while (fgetc(stdin) != '\n')
			dropped++;
			if (dropped == 0)
			{
				pass0 = 1;
			}
		}
		if (pass0)
		{
			int i;
			char letters[]=
			{'A', 'B', 'C', 'D'};
			if(rawrot[1]=='\'' || rawrot[1]=='\"')
			{
				pass1 = 1;
			}
			for (i=0;i<4;i++)
			{
				if(rawrot[0]==letters[i])
				{
					pass2 = 1;
				}
			}
		}
		if(pass1 && pass2)
		{
			if (rawrot[0] == 'A')
			{
				square = 0;
			}
			else if (rawrot[0] == 'B')
			{
				square = 1;
			}
			else if (rawrot[0] == 'C')
			{
				square = 2;
			}
			else if (rawrot[0] == 'D')
			{
				square = 3;
			}
			pass3=1;
		}
	}
	if (rawrot[1] == '\'')
	{
		rotateC(arr, square);
	}
	else
	{
		rotateCC(arr, square);
	}
}
void placePiece(char arr[4][9], char color)
{
	int pass0, pass1, pass2, pass3;
	int square, place;
	pass3 = 0;
	while (pass3 == 0)
	{
		pass0 = 0;
		pass1 = 0;
		pass2 = 0;
		char rawloc[] = "A1";
		printf("Location (ie. A4 or C9): ");
		fgets(rawloc, 3, stdin);
		if (rawloc[strlen(rawloc) -1] != '\n')
		{
			int dropped = 0;
			while (fgetc(stdin) != '\n')
                dropped++;
			if (dropped == 0)
			{
				pass0 = 1;
			}
		}
		if (pass0)
		{
			int i;
			char letters[]=
			{'A', 'B', 'C', 'D'};
			for (i=1;i<10;i++)
			{
				if(rawloc[1]==('0'+i))
				{
					pass1 = 1;
				}
			}
			for (i=0;i<4;i++)
			{
				if(rawloc[0]==letters[i])
				{
					pass2 = 1;
				}
			}
		}
		if(pass1 && pass2)
		{
			if (rawloc[0] == 'A')
			{
				square = 0;
			}
			else if (rawloc[0] == 'B')
			{
				square = 1;
			}
			else if (rawloc[0] == 'C')
			{
				square = 2;
			}
			else if (rawloc[0] == 'D')
			{
				square = 3;
			}
			place = rawloc[1] - '0';
			if (arr[square][place-1]==' ')
			{
				pass3 = 1;
			}
		}
	}
	arr[square][place-1] = color;
}
void printBoard(char arr[4][9])
{
	system(clear);
	printf("%c %c %c|%c %c %c\n%c %c %c|%c %c %c\n%c %c %c|%c %c %c\n-----+-----\n%c %c %c|%c %c %c\n%c %c %c|%c %c %c\n%c %c %c|%c %c %c\n\n",arr[0][0], arr[0][1], arr[0][2], arr[1][0], arr[1][1], arr[1][2], arr[0][3], arr[0][4], arr[0][5], arr[1][3], arr[1][4], arr[1][5], arr[0][6], arr[0][7], arr[0][8], arr[1][6], arr[1][7], arr[1][8], arr[2][0], arr[2][1], arr[2][2], arr[3][0], arr[3][1], arr[3][2], arr[2][3], arr[2][4], arr[2][5], arr[3][3], arr[3][4], arr[3][5], arr[2][6], arr[2][7], arr[2][8], arr[3][6], arr[3][7], arr[3][8]);
}
main ()
{
	char gameBoard[4][9];
	genBoard(gameBoard);
	int playing = 1;
	printBoard(gameBoard);
	while (playing)
	{
		printf("White's Turn\n\n");
		placePiece(gameBoard, WHITE);
		printBoard(gameBoard);
		if(playing)
		{
			printf("White's Turn\n\n");
			rotateSquare(gameBoard);
			printBoard(gameBoard);
			if (playing)
			{
				printf("Black's Turn\n\n");
				placePiece(gameBoard, BLACK);
				printBoard(gameBoard);
				if(playing)
				{
					printf("Black's Turn\n\n");
					rotateSquare(gameBoard);
					printBoard(gameBoard);
				}
			}
		}
	}
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HEIGHT 3
#define WIDTH 3


int winCheck(int sum)
{
    if (sum == 3)
    {
        printf("Player X WIN!\n");
        return 1;
    }
    else if (sum == -3)
    {
        printf("Player O WIN!\n");
        return 1;
    }

    return 0; //Nobody WIN
}

int win(int b[][HEIGHT])
{
    int i, j;
    int sum;

    // 가로로 세개씩 체크
    for (i = 0; i < WIDTH; i++)
    {
        sum = 0; //각각 체크해야하므로 매번 초기화
        for (j = 0; j < HEIGHT; j++)
            sum += b[i][j];

        if (winCheck(sum)) return 1;
    }

    // 세로로 세개씩 체크
    for (j = 0; j < HEIGHT; j++)
    {
        sum = 0;
        for (i = 0; i < WIDTH; i++)
            sum += b[i][j];
        if (winCheck(sum)) return 1;
    }
    // 대각선 체크
    sum = 0;
    for (i = 0; i < WIDTH; i++)
    {
        sum += b[i][i];
    }
    if (winCheck(sum)) return 1;

    // 역대각선 체크
    sum = 0;
    for (i = 0; i < WIDTH; i++)
    {
        sum += b[i][WIDTH - 1 - i];
    }
    if (winCheck(sum)) return 1;

    return 0;
}

void gotoxy(int x, int y)
{
    printf("\033[%d;%df", x, y);
    fflush(stdout);
}

void printfColor(int color, const char* str)
{
    printf("\033[%dm%s\033[0m", color, str);
}

int gameBoard(int boardSize) {

    printf("TicTacToe!\n");

    if (boardSize % 2 != 1)
    {
        return-1;
    }
    char buf1[3 * 4] = { 0 };
    char buf2[3 * 4] = { 0 };
    int x = 0, y = 0;
    for (x = 0; x < boardSize * 3; x += 3)
    {
        strcpy(&buf1[x], "---");
        strcpy(&buf2[x], "| |");
    }
    strcpy(&buf1[x], "\n\0");
    strcpy(&buf2[x], "\n\0");
    for (y = 0; y < boardSize; y++)
    {
        printfColor(31, buf1);
        printfColor(31, buf2);
    }
    printfColor(31, buf1);
    return 0;
}

int stonePrint(int b[][HEIGHT]) 
{
	char ch;
	int i, j;

	for (i = 0; i < WIDTH; i++) 
	{
		for (j = 0; j < HEIGHT; j++) 
		{
			if (b[i][j] == 1)
				ch = 'X';
			else if (b[i][j] == -1)
				ch = 'O';
			else
				ch = ' ';

            gotoxy(2 * i + 3, 3 * j + 2);    
			printf(" %c", ch);
		}
		printf("\n");
	}
}

int main()
{
    //char inputBuf[100];
    //int boardSize;
    //scanf("Board Size (odd number) : %d", &boardSize);

    int board[3][3];
    char turn = 'X';  // first turn : Player X

    int r, c;
    int i, j;
    int count;
    int lose;

    for (i = 0; i < WIDTH; i++)
        for (j = 0; j < HEIGHT; j++)
            board[i][j] = 0;

    count = 0;



    system("clear");

    do
    {
        system("clear");
        gotoxy(1, 1);
        //printf("TicTacToe!\n");

        
        gameBoard(3);
        stonePrint(board);

        gotoxy(3 * 3 + 1, 1);
        printf("\nPlayer %c (row, col) : ", turn);
        scanf("%d %d", &r, &c);
        system("clear");
        //printf("\n");
        //scanf("%s",inputBuf);
        

        if (board[r][c]) //이미 채워져있으면 다시 입력 프롬프트 실행
            continue;		//코드 이전부분으로 돌아감

        count++; //심볼이 입력된 횟수 증가

        

        if (turn == 'X') {

            board[r][c] = 1;
            turn = 'O'; //턴을 바꿔줌
            //gotoxy(2 * r + 3, 3 * c + 2);
            //printf("X");
        }
        else {
            board[r][c] = -1;
            turn = 'X';
            //gotoxy(2 * r + 3, 3 * c + 2);
            //printf("O");
        }

        gameBoard(3);
        stonePrint(board);
        gotoxy(3 * 3 + 1, 1);


        /*if(inputBuf[0] == 'q')
        {
            break;
        }*/

    } while (((lose = !win(board)) && count < 9));

    if (lose && count == 9)
        printf("Nobody WON!\n");

    return 0;
}
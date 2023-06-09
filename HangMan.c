#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>

#define WORD_SIZE 125
#define MAX_LIFE 5

void GotoXY(int x, int y)
{
    move(y,x);
}

typedef struct coord
{
    int x_coord;
    int y_coord;
} COORD;

char problem[WORD_SIZE];
int visited[WORD_SIZE];
int life = MAX_LIFE;
int count = 0;

// 파일로 들어가서 글자 읽어오기
void LoadWord()
{
    FILE *fs;
    fs = fopen("words.txt", "r");   // 파일을 열어주세용

    srand(time(NULL));  // 매번 랜덤한 글자 주기
    int idx = rand() % WORD_SIZE;

    char strTmp[WORD_SIZE];
    char *pStr;
    int line_num = 0;
    // feof (파일의 끝을 알려주는 것) 즉, 끝까지 읽지 않았다면 반복해주세요
    while (!feof(fs)){
        pStr = fgets(strTmp, sizeof(strTmp), fs); //아오
        line_num++;
        if (line_num == idx) {
            strcpy(problem, pStr);
            break;
        }
    }
    fclose(fs);
}

void DrawMan()
{
    if (life == MAX_LIFE)
    {
        GotoXY(48, 10);
        printw("◜---◝\n");
        GotoXY(48, 11);
        printw("◟---◞\n");
    }
    else if (life == MAX_LIFE-1)
    {
        GotoXY(50, 12);
        printw("|\n");
        GotoXY(50, 13);
        printw("|\n");
    }
    else if (life == MAX_LIFE-2)
    {
        GotoXY(49, 14);
        printw("/  \\\n");
        GotoXY(48, 15);
        printw("/    \\\n");
    }
    else if (life == MAX_LIFE-3)
    {
        GotoXY(50, 16);
        printw("|\n");
        GotoXY(50, 17);
        printw("|\n");
    }
    else
    {
        GotoXY(49, 18);
        printw("/  \\\n");
        GotoXY(48, 19);
        printw("/    \\\n");
    }
}

void PrintMap()
{
    GotoXY(30, 7);
    for (int i=0; i<40; i++) printw("■");
    for (int i=0; i<3; i++){
        GotoXY(50, 7+i);
        printw("■");
    }
    for (int i=0; i<13; i++){
        GotoXY(30, 7+i);
        printw("■");
    }
}

void InputWord()
{
    char guess;
    int line = 0;
    int probLen = (int)(strlen(problem) - 1);
    while(count < probLen && life > 0)
    {
        GotoXY(43, 23+line);
        int found = 0;
        guess = getch();    //enter 안 쳐도 반영되게 하기
        for (int i=0; i < probLen; i++){
            if (guess == problem[i] && !visited[i])
            {
                printw(" Righttt! \n");
                GotoXY(38, 21);
                count++;
                found = 1;
                visited[i]++;
                for (int j=0; j < probLen; j++)
                {
                    if (i == j) {
                        printw("%c ", problem[j]);
                    }
                    else if (visited[j]) {
                        printw("%c ", problem[j]);
                    }
                    else {
                        printw("_ ");
                    }
                }
                break;
            }
        }
        if (!found)
        {
            addstr(" Wrong answer! \n");
            DrawMan();
            GotoXY(35+life, 6);
            printw(" ");
            life--;
        }

        getchar();
        line++;
    }
}

int main(void)
{
    //노래 넣고 싶어.. 찾아보기 
    setlocale(LC_ALL, "");  //현재 내 컴퓨터의 환결설정에 저장된 언어 정보 사용
    initscr();  //curses 모드 시작

    GotoXY(40, 4);
    printw("HANGMAN Game Start!\n\n");    //curses모드에서는 printf함수 이용 불가능
    refresh();  //refresh를 해주기 전까지는 화면에 출력되지 않는다. 근데 잘되는거 같은데? 찾아보기
    GotoXY(30, 6);
    printw("Life: ♥♥♥♥♥\n");
    refresh();

    LoadWord();
    PrintMap();

    GotoXY(30, 21);
    printw("Answer: ");
    for (int i=0; i<(int)strlen(problem)-1; i++) printw(" _");
    refresh();
    GotoXY(30, 22);
    addstr("Guess the Word! \n");
    refresh();

    GotoXY(30, 23);
    addstr("Your answer: ");
    InputWord();

    GotoXY(30, 37);
    if (count==(int)strlen(problem)-1 && !life) printw("You won!\n");
    else printw("You lose! The answer was: %s \n", problem);

    GotoXY(30, 40);
    addstr("\npress any key to exit...");
    refresh();
    getch();    //어떤 키를 입력하든 해당 키값을 반환하며 즉식 종료한다
                //콘솔 입력함수. scanf와 달리 엔터를 누르지 않아도 반환한다

    endwin();   //curses 모드 종료

    return 0;
}
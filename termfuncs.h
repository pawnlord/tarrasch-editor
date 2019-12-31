#include <termios.h>
#include <stdio.h>
#define clear() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))
#define godownx(x) printf("\033[%dB", (x))
#define goupx(x) printf("\033[%dA", (x))
#define goleftx(x) printf("\033[%dD", (x))
#define gorightx(x) printf("\033[%dC", (x))
#define delete(x) printf("\033[%dD \033[%dD", 1, 1)
#define display() printf("\n")


static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); //grab old terminal i/o settings
  new = old; //make new settings same as old settings
  new.c_lflag &= ~ICANON; //disable buffered i/o
  new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
  tcsetattr(0, TCSANOW, &new); //apply terminal io settings
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
int getch_(int echo) 
{
  int ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* 
Read 1 character without echo 
getch() function definition.
*/
int getch(void) 
{
  return getch_(0);
}

/* 
Read 1 character with echo 
getche() function definition.
*/
int getche(void) 
{
  return getch_(1);
}

void message(int width, int height, char* message)
{
	int length = 0;
	while(message[length+1] !=0)
		length++;
	gotoxy(width/2-(length/2), height-1);
	printf("%s", message);
}
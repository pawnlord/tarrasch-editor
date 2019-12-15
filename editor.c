#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "filereader.h"
#include "termfuncs.h"
int main(void)
{
	char text[100000];
	FILE* source = fopen("test", "r");
	readfile(source, text);
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int width = w.ws_col;
	int last_width = 0;
	clear();
	printf("HELLO ");
	int i;
	unsigned char c = ' '; 
	while(1)
	{
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	 	width = w.ws_col;
	 	
		if(last_width != width)
		{
		
			clear();
			//printf("-%d, %d", width, last_width);
			
			gotoxy(1,2);
			for(i = 0; i < width; ++i)
			{
				printf("-");
			}
			display();
			printf("%s",text);
		}
		c = getch();
		printf("%c", c);
		if(c == 127)
			delete();
		if(c == 3)
			break;
		last_width = width;
		//scanf(" %s", n);
	}
	
	return 0;
}

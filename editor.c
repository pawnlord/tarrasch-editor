#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stropts.h>

#include "filereader.h"
#include "termfuncs.h"

int main(void)
{
	char text[100000];
	FILE* source = fopen("test", "r");
	int cursor = readfile(source, text);
	fclose(source);
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int width = w.ws_col;
	int last_width = 0;
	clear();
	int i;
	int j;
	int length;
	int tab_spaces = 4;
	unsigned int c = ' '; 
	while(1)
	{
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	 	width = w.ws_col;
	 	
		if(last_width != width)
		{
		
			clear();
			printf("ESC TWICE-EXIT WITH SAVING   ^C-EXIT WITHOUT SAVING");
			
			gotoxy(1,2);
			for(i = 0; i < width; ++i)
			{
				printf("-");
			}
			display();
			printf("%s",text);
		}
		c = getch();
			
		if(c == 127 && cursor >= 0)
		{
			delete();
			
			cursor--;
			if(text[cursor] == '\n')
			{
				j = cursor-1;
				length = 0;
				while(text[j] != '\n' && j >= 0){
					j--;
					length++;
				}
				//printf("%d", length);
				goupx(1);
				gorightx(length);
			}
			text[cursor] = 0;
			
		}
		else if(c == '\t')
		{
			for(j = 0; j < tab_spaces; j++)
			{
				printf(" ");
				text[cursor] = ' ';
				cursor++;
			}
			
		}
		else if(c == 27)
		{
			c = getch();
			if(c == 27)
			{
				printf("\n");
				break;
			}
			else
			{
				c = getch();
				switch(c)
				{
					case 'D':
					//left
						goleftx(1);
						cursor--;
						if(text[cursor] == '\n')
						{
							j = cursor-1;
							length = 0;
							while(text[j] != '\n' && j >= 0){
								j--;
								length++;
							}
							//printf("%d", length);
							goupx(1);
							gorightx(length);
						}
					break;
					case 'C':
					//right
						if(text[cursor+1] != 0){
							gorightx(1);
							cursor++;
							if(text[cursor] == '\n')
							{
								godownx(1);
								goleftx(width);
							}
						}
					break;
					default:
						printf("%c", c);
					break;
				}
			}
		}
		else
		{
			printf("%c", c);
			text[cursor] = c;
			cursor++;
		}
		last_width = width;
		//scanf(" %s", n);
	}
	source = fopen("test", "w");
	fputs(text, source);
	
	return 0;
}

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stropts.h>
#include <string.h>
#include "filereader.h"
#include "termfuncs.h"
int insert(char* source, char* new, int index)
{
	int i = index;
	char insertion[100000] = {0};
	gotoxy(20, 20);
	while(source[i]!=0)
	{
		printf("H");
		insertion[i-(index)] = source[i];
		++i;
	}
	
	int j = 0;
	while(new[j]!=0)
	{
		source[index+j] = new[j];
		++j;
	}
	i = 0;
	while(insertion[i]!=0)
	{
		source[index+j+i] = insertion[i];
		++i;
	}
	
}
int insert_deletion(char* source,  int index)
{
	int i = index;
	char insertion[100000] = {0};
	while(source[i]!=0)
	{
		insertion[i-index] = source[i];
		++i;
	}
	i = index;
	while(insertion[i-(index)]!=0)
	{
		source[i] = insertion[i-(index-1)];
		++i;
	}
	source[i] = 0;
}
int main(void)
{
	char text[100000] = {0};
	FILE* source = fopen("test", "r");
	int cursor = readfile(source, text);
	fclose(source);
	struct winsize w;
	//Get the width for formating
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int width = w.ws_col;
	int height = w.ws_row;
	int last_width = 0;
	clear();
	int i;
	int j;
	int length;
	int tab_spaces = 4;
	unsigned int c = ' ';
	int x = 0; 
	int y = 0; 
	char msg[200];
	while(1)
	{
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	 	width = w.ws_col;
	 	height = w.ws_row;
	 	
		clear();
		printf("ESC TWICE-EXIT WITH SAVING   ^C-EXIT WITHOUT SAVING--Tarrasch");
		
		gotoxy(1,2);
		for(i = 0; i < width; ++i)
		{
			printf("-");
		}
		display();
		printf("%s",text);
		x=0;
		y=3;
		i = 0;
		
		sprintf(msg, "Cursor: %d", cursor);
		message(width, height, msg);
		int nl_check = 0;
		while(text[i]!=0){
			if(text[i] == '\n' || x > width)
			{
				x=0;
				y++;
				nl_check = 1;
			}
			++i;
			x++;
			if(i == cursor)
			{
				break;
			}
			
			x%=width;
		}
		sprintf(msg, "Coords: %d, %d", x, y);
		message(width, height-1, msg);
		gotoxy(x+1-nl_check,y);
				
		c = getch();
			
		if(c == 127 && cursor > 0)
		{
			--cursor;
			insert_deletion(text, cursor);
			
		}
		else if(c == '\t')
		{
			for(j = 0; j < tab_spaces; j++)
			{
				printf(" ");
				insert(text, " ", cursor);
				cursor++;
			}
			
		}
		else if(c == 27)
		{
			message(width, height, "--Really Exit?--");
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
						if(cursor > 0)
							cursor--;
					break;
					case 'A':
					//up
						if(y > 3)
						{
							j = cursor;
							i = cursor;
							while(text[i]!='\n' && i >= 0)
							{
								--i;
							}
							cursor = i;
						 	--i;
							length = 0;
							while(text[i]!='\n' && i >= 0)
							{
								length++;
								--i;
							}
							if(x < length )
							{
								cursor = (j-cursor)+i;
							}
							else
							{
								j--;
								while(text[j]!='\n' && j >= 0)
								{
									--j;
								}
								cursor = j;
							}
						}
					break;
					case 'B':
					//down
						i = cursor;
						if(text[cursor] == '\n')
							cursor--;
						length = 1;
						int new_length = 0;
						while(text[++cursor] != '\n' && text[cursor-1] != 0)
						{
						}
						j = cursor;
						while(text[++j] != '\n' && text[j-1] != 0)
						{
							new_length++;
						}
						while(text[--i] != '\n' && i > 0)
						{
							length++;
						}
						if(length < new_length)
						{
							cursor += length;
						}
						else
						{
							cursor += new_length;
						}
					break;
					case 'C':
					//right
						if(text[cursor]!=0)
							cursor++;
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
			insert(text, (char*)&c, cursor);
			cursor++;
		}
		last_width = width;
		//scanf(" %s", n);
	}
	source = fopen("test", "w");
	fputs(text, source);
	gotoxy(0,height);
	return 0;
}

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
int main(int argc, char *argv[])
{
	char text[100000] = {0};
	char filename[100] = {0};
	if(argc < 2)
		strcpy(filename, "untitled");
	else
		strcpy(filename, argv[1]);
	FILE* source;
    if (!(source = fopen(filename, "r")))
    {
        source = fopen(filename, "w");
        fclose(source);
        source = fopen(filename, "r");
    }
	int cursor = readfile(source, text);
	fclose(source);
	struct winsize w;
	//Get the width for formating
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int width = w.ws_col;
	int height = w.ws_row;
	int lines_start = 0;
	int lines_end = height-4;
	int last_width = 0;
	clear();
	int i;
	int j;
	int length;
	int tab_spaces = 4;
	unsigned int c = ' ';
	int x = 0; 
	int y = 0; 
	int last_x = 0; 
	int last_y = 0; 
	char msg[200];
	while(1)
	{
		//get screen dimensions
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	 	width = w.ws_col;
	 	height = w.ws_row;
	 	
		clear();
		//Start figuring out display_text
		last_x = x;
		last_y = y;
		x=0;
		y=3;
		i = 0;
		lines_start = 0;
		lines_end = height-4;
		int line_counter = 0;
		char display_text[100000] = {0};
		if(cursor < 0)
			cursor = 0;
		sprintf(msg, "Cursor: %d", cursor);
		message(width, height, msg);
		int nl_check = 0;
		while(text[i]!=0){
			if(text[i] == '\n' || x > width)
			{
				x=0;
				line_counter++;
				y++;
				if(y > height-4)
				{
					y--;
					lines_start++;
					lines_end++;
				}
			}
			++i;
			x++;
			if(i == cursor)
			{
				break;
			}
			
			x%=width;
		}
		/*
		if(y<last_y && y == 4 && lines_start != 0)
		{
			lines_start--;
			lines_end--;
		}
		*/
		sprintf(msg, "X, Y: %d, %d", x, y);
		message(width, height-2, msg);
		sprintf(msg, "LINES_START, LINES_END: %d, %d", lines_start, lines_end);
		message(width, height-1, msg);
		line_counter = 0;
		char cpy_check = 0;
		int cpy_start = 0;
		i = 0;
		while(text[i]!=0){
			if(cpy_check == 1)
			{
				display_text[i-cpy_start] = text[i];
			}
			else
			{
				++cpy_start;
			}
			
			if(text[i] == '\n' || x > width)
			{
				line_counter++;
			}
			if(line_counter == lines_start)
			{
				cpy_check = 1;
			}
			if(line_counter == lines_end)
			{
				cpy_check = 0;
			}
			++i;
		}
		//display text
		gotoxy(1,3);
		printf("%s",display_text);
		//display overhead
		gotoxy(1,1);
		for(i = 0; i < width; ++i)
		{
			printf(" ");
		}
		gotoxy(1,1);
		printf("ESC TWICE-EXIT WITH SAVING   ^C-EXIT WITHOUT SAVING -- %s -- Tarrasch", filename);
		
		gotoxy(1,2);
		for(i = 0; i < width; ++i)
		{
			printf("-");
		}
		gotoxy(x-nl_check,y);
			
		//*****************
		// Start of editor
		//*****************
		
		c = getch();
			
		if(c == 127 && cursor >= 0)
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
						if(text[j] == '\n')
							new_length++;
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
	if(strcmp(filename, "untitled")==0)
	{
		gotoxy(0,height);
		strcpy(filename, "");
		printf("NAME: "); 
		scanf("%s", filename);
	}
	//printf("\nFILENAME: %s\n", filename);
	source = fopen(filename, "w");
	fputs(text, source);
	fclose(source);
	return 0;
}

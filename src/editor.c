#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stropts.h>
#include <string.h>
#include "../headers/keyhandlers.h"
#include "../headers/filereader.h"
#include "../headers/termfuncs.h"
int insert(char* source, char* new, int index) {
	int i = index;
	char insertion[100000] = {0};
	gotoxy(20, 20);
	while(source[i]!=0) {
		insertion[i-(index)] = source[i];
		++i;
	}
	
	int j = 0;
	while(new[j]!=0) {
		source[index+j] = new[j];
		++j;
	}
	
	i = 0;
	while(insertion[i]!=0) {
		source[index+j+i] = insertion[i];
		++i;
	}
	
}
int insert_deletion(char* source,  int index) {
	int i = index;
	char insertion[100000] = {0};
	while(source[i]!=0) {
		insertion[i-index] = source[i];
		++i;
	}
	i = index;
	while(insertion[i-(index)]!=0) {
		source[i] = insertion[i-(index-1)];
		++i;
	}
	source[i] = 0;
}

int readsyntaxcfg(char raw_syntax[], char syntax[][1000]) {
	char word[255] = {0};
	int current_letter = 0;
	int current_word = 0;
	printf("Reading syntax...\n");
	for(int i = 0; raw_syntax[i] != 0; i++) {
		if(raw_syntax[i] == '\n'  || raw_syntax[i] == 0) {
			strcpy(syntax[current_word], word);
			for(int j = 0; word[j] != 0; j++) {
				word[j] = 0;
			}
			current_letter = 0;
			current_word++;
			i++;
		}
		word[current_letter] = raw_syntax[i];
		current_letter++;
	}
	if(strcmp(syntax[current_word], "")) {
		strcpy(syntax[current_word], word);
	}
	printf("Read syntax, lines: %d\n", current_word);
	return 0;
}

int getsyntax(char syntax[][1000], char keyword[], char val[]) {
	char current_keyword[50] = {0};
	char name_end = 0;
	char current_highlight[50] = {0};
	int finding_name = 1;
	for(int i = 0; strcmp(syntax[i], ""); i++) {
		/* Reset variables */
		finding_name = 1;
		name_end = 0;
		for(int j = 0; current_keyword[j] != 0; j++){
			current_keyword[j] = 0;
		}
		/* read for name */
		for(int currc = 0; syntax[i][currc] != 0; currc++) {
			/* if we are at the end of a word, check it */
			if(syntax[i][currc] == ' ') {
				/* if it is the word we are looking for, look for val */
				if(strcmp(current_keyword, keyword) == 0){
					finding_name = 0;
					name_end = currc+1;		
					for(int j = 0; current_highlight[j] != 0; j++){
						current_highlight[j] = 0;
					}
				}else {
					current_keyword[currc] = syntax[i][currc];
				}
				
			}
			else if(finding_name) {
				current_keyword[currc] = syntax[i][currc];
			}
			else {
				current_highlight[currc-name_end] = syntax[i][currc];
			}
		}
	}
	if(strcmp(current_highlight, "") == 0) { 
		return 0;
	}		
	strcpy(val, current_highlight);
	return 1;
}

int main(int argc, char *argv[]) {
	start_handler(INThandler, SIGINT);
	startbuff();
	printf("\033[0m");
	char text[100000] = {0};
	char filename[100] = {0};
	if(argc < 2)
		strcpy(filename, "untitled");
	else
		strcpy(filename, argv[1]);
	FILE* source;
	if(strcmp(filename, "untitled")) {
		if (!(source = fopen(filename, "r"))) {
		    source = fopen(filename, "w");
		    fclose(source);
		    source = fopen(filename, "r");
		}
		readfile(source, text);
		fclose(source);
    }
	int cursor = 0;
	/* Config file for syntax highlighting */
	FILE* syntax_cfg;
	char raw_syntax[100000] = {0};
	char syntax[1000][1000] = {""};
	char highlight[1000] = {0};
	char highlight_code[1000] = {0};
	char syntax_filename[100];
	/* get home directory */
	strcat(strcpy(syntax_filename, getenv("HOME")), "/.tarrasch/syntax.cfg");
    if (!(syntax_cfg = fopen(syntax_filename, "r"))) {
        syntax_cfg = fopen(syntax_filename, "w");
        fclose(syntax_cfg);
        syntax_cfg = fopen(syntax_filename, "r");
    }
    readfile(syntax_cfg, raw_syntax);
	fclose(syntax_cfg);
	readsyntaxcfg(raw_syntax, syntax);
	struct winsize w;
	/* Get the width for formating */
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int width = w.ws_col;
	int height = w.ws_row;
	int lines_start = 0;
	int lines_end = height-4;
	int last_width = 0;
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
	sprintf(msg, "New Buffer");
	message(width, height, msg);
	while(running) {
		/* get screen dimensions */
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	 	width = w.ws_col;
	 	height = w.ws_row;
	 	
		/* Start figuring out display_text */
		last_x = x;
		last_y = y;
		x=1;
		y=3;
		i = 0;
		lines_start = 0;
		lines_end = height-1;
		int line_counter = 0;
		char display_text[100000] = {0};
		if(cursor < 0)
			cursor = 0;
		if(cursor!= 0)
			while(text[cursor-1] == 0)
				cursor--;
		int nl_check = 0;
		while(text[i]!=0){
			if(text[i] == '\n' || x > width) {
				x=0;
				line_counter++;
				y++;
				if(y > height-1) {
					y-=((height-1)/2);
					lines_start+=((height-1)/2);
					lines_end+=((height-1)/2);
				}
			}
			if(text[i] == '\t') {
				x+=7;
			}
			++i;
			x++;
			if(i == cursor) {
				break;
			}
			
		}
		line_counter = 0;
		char cpy_check = 0;
		int cpy_start = 0;
		i = 0;
		/* Syntax trackers */
		char current_word[1000] = {};
		int word_counter = 0;
		int word_start = 0;
		/* Copy all text we want to display, from line lines_start to lines_end */
		int offset = 0;
		while(text[i]!=0) {
		
			/* Start copying if we are at lines start */
			if(line_counter == lines_start) {
				cpy_check = 1;
				word_start = 0;
			}
			/* If we are copying, copy */
			if(cpy_check == 1) {
				display_text[i-cpy_start+offset] = text[i];
				/* TODO: MAKE FUNCTION */ 
				if(text[i] == ' ' || text[i] == '\n' || text[i] == '#' ||
				 text[i] == '+' || text[i] == '[' || text[i] == ']' ||
				 text[i] == '=' || text[i] == '-' || text[i] == '(' ||
				 text[i] == ')' || text[i] == '{' || text[i] == '}' ||
				 text[i] == '<' || text[i] == '>' || text[i] == '/' ||
				 text[i] == '*' || text[i] == '.' || text[i] == ',' ||
				 text[i] == '!' || text[i] == '@' || text[i] == '$' ||
				 text[i] == '%' || text[i] == '^' || text[i] == '&' ||
				 text[i] == '|' || text[i] == '\\' || text[i] == '?' ||
				 text[i] == ':' || text[i] == ';' || text[i] == '~' ||
				 text[i] == '\t'  ) {
					if(getsyntax(syntax, current_word, highlight)){
					    sprintf(highlight_code, "\033[%sm", highlight);
						insert(display_text, highlight_code, word_start+offset);
						offset+=5;
						insert(display_text, "\033[0m", i-cpy_start+offset);
						offset+=4;
					}
					word_start = i+1-cpy_start;
					word_counter = 0;
					for(int k = 0; current_word[k] != 0; k++) {
						current_word[k] = 0;
					}
				} else {
					current_word[word_counter] = text[i];
					word_counter++;
				}
			}
			/* otherwise, move the counter so we have accurate start of copying */
			else {
				++cpy_start;
			}
			/* if we are going to a new line, add to line coutner */
			if(text[i] == '\n' || x > width) {
				line_counter++;
			}
			/* stop if we are at the end */
			if(line_counter == lines_end) {
				cpy_check = 0;
			}
			++i;
		}
		/* display text */
		gotoxy(1,3);
		printf("%s",display_text);
		/* display overhead */
		gotoxy(1,1);
		for(i = 0; i < width; ++i) {
			printf(" ");
		}
		gotoxy(1,1);
		printf("ESC TWICE-EXIT WITH SAVING   ^C-EXIT WITHOUT SAVING -- %s -- Tarrasch", filename);
		
		gotoxy(1,2);
		for(i = 0; i < width; ++i) {
			printf("-");
		}
		gotoxy(x,y);
			
		/****************************************************/
		/* Start of editor                                  */
		/****************************************************/
		
		c = getch();
			
		if(c == 127 && cursor >= 0) {
			--cursor;
			insert_deletion(text, cursor);
			
		}
		else if(c == 27) {
			message(width, height, "--Really Exit?--");
			c = getch();
			if(c == 27) {
				printf("\n");
				break;
			} else {
				c = getch();
				switch(c) {
					case 'D':
					//left
						if(cursor > 0)
							cursor--;
					break;
					case 'A':
					//up
						if(y > 3) {
							j = cursor;
							i = cursor;
							while(text[i]!='\n' && i >= 0) {
								--i;
							}
							cursor = i;
						 	--i;
							length = 0;
							while(text[i]!='\n' && i >= 0) {
								length++;
								--i;
							}
							if(x < length ) {
								cursor = (j-cursor)+i;
							}
							else {
								j--;
								while(text[j]!='\n' && j >= 0) {
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
						while(text[++cursor] != '\n' && text[cursor-1] != 0){}
						j = cursor;
						while(text[++j] != '\n' && text[j-1] != 0) {
							new_length++;
						}
						if(text[j] == '\n')
							new_length++;
						while(text[--i] != '\n' && i > 0) {
							length++;
						}
						if(length < new_length) {
							cursor += length;
						}
						else {
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
		else {
			printf("%c", c);
			insert(text, (char*)&c, cursor);
			cursor++;
		}
		last_width = width;
		clear();
	}
	if(running) {
		if(strcmp(filename, "untitled")==0) {
			gotoxy(0,height);
			strcpy(filename, "");
			printf("NAME: "); 
			scanf("%s", filename);
		}
		source = fopen(filename, "w");
		fputs(text, source);
		fclose(source);
	}
	endbuff();
	return 0;
}

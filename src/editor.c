#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stropts.h>
#include <string.h>
#include "../headers/keyhandlers.h"
#include "../headers/filereader.h"
#include "../headers/termfuncs.h"
#include "../headers/config_reader.h"
int in_string(char ch, char* str){
	for(int i = 0; str[i] != 0; i++) {
		if(ch == str[i]) {
			return 1;
		}
	}
	return 0;
}

int len(char str[]){
	int i;	
	for(i = 0; str[i] != 0; i++){}
	return i;
}
char symbols[] = "!@#$%^&*(){}[]\\;:'\"<>,./?~ \n\t";

char alphanumeric[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890 \n\t";

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
	config syntax_cfg;
	auto_cfg_setup(&syntax_cfg);
	char*  syntax_filename = malloc(50);
	char*  highlight_code = malloc(50); 
	char** highlight_val = malloc(10);
	for(int i = 0; i < 50; i++){
		syntax_filename[i] = 0;
		highlight_code[i] = 0;
	}
	for(int i = 0; i < 10; i++){
		highlight_val[i] = malloc(50);
		for(int j = 0; j < 50; j++){
			highlight_val[i][j] = 0;
		}
	}
	strcat(strcpy(syntax_filename, getenv("HOME")), "/.tarrasch/syntax.cfg");
		
	if (!(config_reader(syntax_filename, &syntax_cfg))) {
        FILE* fp = fopen(syntax_filename, "w");
        fclose(fp);
        config_reader(syntax_filename, &syntax_cfg);
    }
    /* fields of syntax */
    char** syntaxfield= malloc(255*sizeof(*syntaxfield));
    char** symbolsfield = malloc(255*sizeof(*symbolsfield));
    char** blockfield = malloc(255*sizeof(*blockfield));
    for(int i = 0; i < 255; i++){
    	syntaxfield[i] = malloc(255);
    	symbolsfield[i] = malloc(255);
    	blockfield[i] = malloc(255);
    	for(int j = 0; j < 255; j++){
    		syntaxfield[i][j] = 0;
    		symbolsfield[i][j] = 0;
    		blockfield[i][j] = 0;
    	}
    }
    get_field(syntax_cfg, "SYNTAX", syntaxfield);
    get_field(syntax_cfg, "SYMBOLS", symbolsfield);
    get_field(syntax_cfg, "BLOCKSYNTAX", blockfield);
    printf("noerr\n");
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
	int x_of_lastline = 0;
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
		lines_end = height-3;
		int line_counter = 0;
		char display_text[100000] = {0};
		if(cursor < 0)
			cursor = 0;
		if(cursor!= 0)
			while(text[cursor-1] == 0)
				cursor--;
		x_of_lastline = 0;
		while(text[i]!=0 && i != cursor){
			if(text[i] == '\n' || x >= width) {
				x_of_lastline = x;
				x=0;
				line_counter++;
				y++;
				if(y > height-1 ) {
					y-=((height-4)/2);
					lines_start+=((height-4)/2);
					lines_end+=((height-4)/2);
				}
			}
			if(text[i] == '\t') {
				x+=7;
			}
			i++;
			x++;
		}
		line_counter = 0;
		char cpy_check = 0;
		int cpy_start = 0;
		i = 0;
		int found_line = 0;
		int curr_x = 0;
		/* Syntax trackers */
		char current_word[1000] = {};
		char current_symbols[1000] = {};
		int word_counter = 0;
		int word_start = 0;
		int symbol_counter = 0;
		int symbol_start = 0;
		int counter = 0;
		/* Copy all text we want to display, from line lines_start to lines_end */
		int offset = 0;
		int in_block = 0;
		char end[10];
		while(text[i]!=0) {
		
			/* Start copying if we are at lines start */
			if(line_counter == lines_start) {
				cpy_check = 1;
				if(!found_line) {
					word_start = 0;
				}
				found_line = 1;
			}
			/* If we are copying, copy */
			if(cpy_check == 1) {
				display_text[i-cpy_start+offset] = text[i];
				if(in_string(text[i], symbols) && !in_block) {
					if(get_first_attr(syntax_cfg, syntaxfield, current_word, highlight_val) && !in_block){
					    sprintf(highlight_code, "\033[%sm", highlight_val[0]);
						insert(display_text, highlight_code, word_start+offset);
						offset+=len(highlight_code);
						insert(display_text, "\033[0m", i-cpy_start+offset);
						offset+=4;
						counter++;
					}
					word_start = i+1-cpy_start;
					word_counter = 0;
					for(int k = 0; current_word[k] != 0; k++) {
						current_word[k] = 0;
					}
				} else {
					current_word[word_counter] = text[i];
					if(text[i+1] == 0) {
						if(get_first_attr(syntax_cfg, syntaxfield, current_word, highlight_val) && !in_block){
							sprintf(highlight_code, "\033[%sm", highlight_val[0]);
							insert(display_text, highlight_code, word_start+offset);
							offset+=len(highlight_code);
							insert(display_text, "\033[0m", i+1-cpy_start+offset);
							offset+=4;
							counter++;
						}
						word_start = i+1-cpy_start;
						word_counter = 0;
						for(int k = 0; current_word[k] != 0; k++) {
							current_word[k] = 0;
						}
					}
					word_counter++;
				}
				if(in_string(text[i], alphanumeric)) {
					char cstr[2] = {text[i], 0};
					if((strcmp(end, current_symbols) == 0 || strcmp(end, cstr) == 0) && in_block) {
						insert(display_text, "\033[0m", i-cpy_start+offset);
						in_block = 0;
						offset+=4;
					} else if(get_first_attr(syntax_cfg, blockfield, current_symbols, highlight_val) && strcmp(current_symbols, "") && !in_block) {
						sprintf(highlight_code, "\033[%sm", highlight_val[0]);
						insert(display_text, highlight_code, symbol_start+offset);
						offset+=len(highlight_code);
						in_block = 1;
						if(strcmp(highlight_val[1], "") == 0){
							strcpy(end, "\n");
						} else{
							strcpy(end, highlight_val[1]);
						}
					} else if(get_first_attr(syntax_cfg, symbolsfield, "SYMBOLS", highlight_val) && strcmp(current_symbols, "") && !in_block) {
						sprintf(highlight_code, "\033[%sm", highlight_val[0]);
						
						insert(display_text, highlight_code, symbol_start+offset);
						offset+=len(highlight_code);
						insert(display_text, "\033[0m", i-cpy_start+offset);
						offset+=4;
					}
					
					symbol_start = i+1-cpy_start;
					symbol_counter = 0;
					for(int k = 0; current_symbols[k] != 0; k++) {
						current_symbols[k] = 0;
					}
				} else {
					current_symbols[symbol_counter] = text[i];
				
					if(text[i+1] == 0) {
						sprintf(highlight_code, "\033[%sm", highlight_val[0]);
						insert(display_text, highlight_code, symbol_start+offset);
						offset+=len(highlight_code);
						insert(display_text, "\033[0m", i+1-cpy_start+offset);
						offset+=4;
					}
					symbol_counter++;
				}
				
			}
			
			/* otherwise, move the counter so we have accurate start of copying */
			else {
				++cpy_start;
			}
			/* if we are going to a new line, add to line coutner */
			if(text[i] == '\n' || curr_x >= width) {
				line_counter++;
				curr_x = 0;
			}
			/* stop if we are at the end */
			if(line_counter == lines_end) {
				cpy_check = 0;
			}
			if(text[i] == '\t') {
				curr_x+=7;
			}
			++i;
			curr_x++;
		}
		insert(display_text, "\033[0m", i-cpy_start+offset);
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
		for(i =  0; i < width; ++i) {
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
							int original = cursor;
							i = cursor;
							int cx = x;
							int taboff = 0;
							int itaboff = 0;
							/* find previous line */
							while(cx > 0 && i >= 0) {

								--i;
								if(text[i]=='\t'){
									cx-=7;
									taboff+=1;
								}
								cx--;
							}
							/* store previous line */
							cursor = i;
						 	--i;
							cx = x_of_lastline;
							/* find length of previous line */
							while((text[i]!='\n' && !(cx <= 1)) && i >= 0) {
								--i;
								if(text[i]=='\t'){
									cx-=7;
									itaboff+=1;
								}
								cx--;
							}
							/* if x is less than length, goto the spot we found */
							/* else, goto the last line */
							if(x < x_of_lastline) {
								cursor = i+(original-cursor)+(taboff*7)-(itaboff*7); /* start_of_last_line+(original_position - start_of_this_line) */
							} 
						}
					break;
					case 'B':
					//down
						i = cursor;
						int cx = x;
						if(text[cursor] == '\n')
							cursor--;
						if(cx == width)
							cx--;
						length = 1;
						int new_length = 0;
						int taboff = 0;
						/* go to next EOL or EOF */
						while(text[++cursor] != '\n' && ++cx < width && text[cursor-1] != 0){
							if(text[cursor] == '\t') {
								x+=7;
							}
						}
						/* save that EOL */
						j = cursor;
						cx = 0;
						/* find the length of this new line */
						while(text[++j] != '\n' && ++cx < width && text[j-1] != 0) {
							new_length++;
							if(text[j] == '\t') {
								new_length+=7;
								cx+=7;
								taboff++;
							}
						}
						if(text[j] == '\n')
							new_length++;
						/* x is offset */
						/* goto offset */
						/* if the length of the new line is les than the offset, */
						/* just go to the end of the line */
						if(x <= new_length) {
							/* figure out offset with tabs */
							for(int i = 0; i < x; i++){	
								cursor++;
								if(text[cursor] == '\t') {
									i+=7;
								}
							}
						}
						else {
							cursor += new_length-(taboff*7);
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

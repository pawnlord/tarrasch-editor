#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/config_reader.h"

int init_str(char** str, int dim){
	*str = malloc(sizeof(char)*dim);
	if(*str == NULL){
		perror("config_reader: init_str");
		return 0;
	}
	for(int i = 0; i < dim; i++){
		(*str)[i] = 0;
	}
	return 1;
}
void free_str(char** str){
	free(*str);
}
int init_strptr(char*** strptr, int dim1, int dim2) {
	*strptr = malloc(sizeof(char*)*dim1);
	if(*strptr == NULL){
		perror("config_reader: init_strptr");
		return 0;
	}
	for(int i = 0; i < dim1; i++){
		(*strptr)[i] = malloc(sizeof(char)*dim2);
		if((*strptr)[i] == NULL){
			perror("config_reader: init_strptr");
			return 0;
		}
		for(int j = 0; j < dim2; j++){
			(*strptr)[i][j] = 0;
		}
	}
	return 1;
}
void free_strptr(char*** strptr, int dim1){
	for(int i = 0; i < dim1; i++){
		free((*strptr)[i]);
	}
	free((*strptr));
}

int init_strptrptr(char**** strptrptr, int dim1, int dim2, int dim3){
	*strptrptr = malloc(sizeof(char**)*dim1);
	if(*strptrptr == NULL){
		perror("config_reader: init_strptrptr");
		return 0;
	}
	for(int i = 0; i < dim1; i++){
		(*strptrptr)[i] = malloc(sizeof(char*)*dim2);
		if((*strptrptr)[i] == NULL){
			perror("config_reader: init_strptrptr");
			return 0;
		}
		for(int j = 0; j < dim2; j++){
			(*strptrptr)[i][j] = malloc(sizeof(char)*dim3);
			if((*strptrptr)[i][j] == NULL){
				perror("config_reader: init_strptrptr");
				return 0;
			}
			for(int k = 0; k < dim3; k++){
					
				(*strptrptr)[i][j][k] = 0;
			}
		}
	}
	return 1;
}

void free_strptrptr(char**** strptrptr, int dim1, int dim2){
	for(int i = 0; i < dim1; i++){
		for(int j = 0; j < dim2; j++){
			free((*strptrptr)[i][j]);
		}
		free((*strptrptr)[i]);
	}
	free((*strptrptr));
}

void insert_string_array(char** originalp, char** newp, int index){
	char** original = originalp;
	char** new = newp;
	char** copy;
	init_strptr(&copy, MAX_SIZE, MAX_SIZE);
	int original_index = index;
	int copy_index = 0;
	for(; strcmp(original[index], ""); index++){
		strcpy(copy[copy_index], original[index]);
		copy_index++;
	}
	index = original_index;
	for(int i = 0; strcmp(new[i], ""); i++){
		strcpy(original[index], new[i]);
		index++;
	}
	for(int i = 0; i < copy_index; i++){
		strcpy(original[index], copy[i]);
		index++;	
	}
	free_strptr(&copy, MAX_SIZE);
}

int is_field(char* str, char begin, char end) {
	int last = 0;
	if(str == NULL){return 0;}
	if(str[0] != begin) {
		return 0;
	}
	for(; str[last] != 0; last++){}
	if(last == 1) {
		return 0;
	}
	if(str[last-1] != end) {
		return 0;
	}
	return 1;
}

int file_reader(char* filename, char* buffer) {
	int ret;
	FILE* fp = fopen(filename, "r");
	if(fp == NULL){
		printf("config_reader: file_reader: error getting file: %s\n", filename);
		perror("config_reader: file_reader");
		return 0;
	}
	char c = 0;
	int i = 0;
	c = fgetc((FILE*)fp);
	// repeat until c is EOF
	while(c != EOF )
	{
		buffer[i] = c;
		i++;
		c = fgetc((FILE*)fp);
	}
	buffer[i] = 0;
	fclose(fp);
	return 1;
}

int config_reader(char* filename, config* cfg) {
	char* buffer;
	if(!init_str(&buffer, 1000)) {return 0;}
	if(!file_reader(filename, buffer)) {return 0;}
	if(!init_strptr(&cfg->words, MAX_SIZE, MAX_SIZE)) {return 0;}
	if(!init_str(&cfg->filename, MAX_SIZE) && cfg->verbose) {
		printf("config_reader: config_reader: failed to save filename, continueing anyway\n");
	} else {
		strcpy(cfg->filename, filename);
	}
	int word_counter = 0;
	int current_word = 0;
	char* eol_str;
	init_str(&eol_str, 2);
	eol_str[0] = cfg->eol;
	int i = 0;
	for(i = 0; buffer[i] != 0; i++) {
		if(buffer[i] == ' ') {
			/* add array for next word */
			if(strcmp(cfg->words[current_word], "")) {
				word_counter = 0;
				current_word++;
			}
		} else if( buffer[i] == cfg->eol){
			/* add eol as word */
			if(strcmp(cfg->words[current_word], "")) {
				word_counter = 0;
				current_word++;
			}
			strcpy(cfg->words[current_word], eol_str);
			/* add array for next word */
			word_counter = 0;
			current_word++;
		} else {
			cfg->words[current_word][word_counter] = buffer[i];
			word_counter++;
		}
	}
	word_counter = 0;
	current_word++;
	free_str(&buffer);
	free_str(&eol_str);
	return 1;
}

int get_field(config cfg, char* field_name, char** field) {
	char* full_field_name;
	
	
	if(!init_str(&full_field_name, 50)) {return 0;}
	
	char* field_char;
	if(!init_str(&field_char, 2)) {return 0;}
	field_char[0] = cfg.begin_field;
	strcat(full_field_name, field_char);
	strcat(full_field_name, field_name);
	field_char[0] = cfg.end_field;
	strcat(full_field_name, field_char);
	
	int i = 0;
	int found = 0;
	
	for(i = 0; strcmp(cfg.words[i], ""); i++) {
		if(strcmp(cfg.words[i], full_field_name) == 0) {
			i+=2; /* two to go down a line */
			found = 1;
			break;
		}
	}
	
	if(found == 0) {
		printf("config_reader: get_field: failed to find field %s\n", full_field_name);
		return 0;
	}
	int index = 0;
	for(; strcmp(cfg.words[i], "") && !is_field(cfg.words[i], cfg.begin_field, cfg.end_field); i++) {
		strcpy(field[index], cfg.words[i]);
		index++;
	}
	
	free_str(&full_field_name);
	return 1;
}

int get_attr(config cfg, char** field, char* attr, char*** val) {
	char* new_line;
	if(!init_str(&new_line, 2)) {return 0;}
	
	new_line[0] = cfg.eol;
	int current_val  = 0;
	int current_val_number = 0;
	int found = 0;
	int after_nl = 0;
	
	for(int i = 0; strcmp(field[i], ""); i++) {
		if(strcmp(field[i], new_line) == 0) {
			after_nl = 1;
		} 
		if(strcmp(field[i], attr) == 0 && after_nl) {
			found = 1;
			i+=1;
			int curr_i = i;
			for(; strcmp(field[i], "") && strcmp(field[i], new_line); i++) {
				strcpy(val[current_val_number][current_val], field[i]);
				current_val++;
			}
			i = curr_i;
			current_val = 0;
			current_val_number++;
		}
		if(strcmp(field[i], new_line)) {
			after_nl = 0;
		} 
	}
	
	free_str(&new_line);
	
	if(found == 0 && cfg.verbose) {printf("config_reader: get_attr: failed to find %s\n", attr);}
	
	return found;
}
int get_first_attr(config cfg, char** field, char* attr, char** val) {
	char* new_line;
	if(!init_str(&new_line, 2)) {return 0;}
	
	new_line[0] = cfg.eol;
	int current_val  = 0;
	int found = 0;
	int after_nl = 0;
	
	for(int i = 0; strcmp(field[i], ""); i++) {
		if(strcmp(field[i], new_line) == 0) {
			after_nl = 1;
		} 
		if(strcmp(field[i], attr) == 0 && after_nl) {
			found = 1;
			i+=1;
			int curr_i = i;
			for(; strcmp(field[i], "") && strcmp(field[i], new_line); i++) {
				strcpy(val[current_val], field[i]);
				current_val++;
			}
			break;
		}
		if(strcmp(field[i], new_line)) {
			after_nl = 0;
		} 
	}
	
	free_str(&new_line);
	
	if(found == 0 && cfg.verbose) {printf("config_reader: get_attr: failed to find %s\n", attr);}
	return found;
}
int get_last_attr(config cfg, char** field, char* attr, char** val) {
	char* new_line;
	if(!init_str(&new_line, 2)) {return 0;}
	
	new_line[0] = cfg.eol;
	int current_val  = 0;
	int found = 0;
	int after_nl = 0;
	
	for(int i = 0; strcmp(field[i], ""); i++) {
		if(strcmp(field[i], new_line) == 0) {
			after_nl = 1;
		} 
		if(strcmp(field[i], attr) == 0 && after_nl) {
			found = 1;
			i+=1;
			int curr_i = i;
			for(int j = 0; strcmp(val[j], ""); j++){
				strcpy(val[j], "");
			}
			for(; strcmp(field[i], "") && strcmp(field[i], new_line); i++) {
				strcpy(val[current_val], field[i]);
				current_val++;
			}
			i = curr_i;
			current_val = 0;
		}
		if(strcmp(field[i], new_line)) {
			after_nl = 0;
		} 
	}
	
	free_str(&new_line);
	
	if(found == 0 && cfg.verbose) {printf("config_reader: get_attr: failed to find %s\n", attr);}
	return found;
}
int dir_get_attr(config cfg, char* fieldname, char* attr, char*** val) {
	char** field;
	if(!init_strptr(&field, MAX_SIZE, MAX_SIZE)) {return 0;}
	
	if(!get_field(cfg, fieldname, field)) {return 0;}
	if(!get_attr(cfg, field, attr, val)) {return 0;}
	
	free_strptr(&field, MAX_SIZE);
	return 1;
}

int dir_get_first_attr(config cfg, char* fieldname, char* attr, char** val) {
	char** field;
	if(!init_strptr(&field, MAX_SIZE, MAX_SIZE)) {return 0;}
	
	if(!get_field(cfg, fieldname, field)) {return 0;}
	if(!get_first_attr(cfg, field, attr, val)) {return 0;}
	
	free_strptr(&field, MAX_SIZE);
	return 1;
}
int dir_get_last_attr(config cfg, char* fieldname, char* attr, char** val) {
	char** field;
	if(!init_strptr(&field, MAX_SIZE, MAX_SIZE)) {return 0;}
	
	if(!get_field(cfg, fieldname, field)) {return 0;}
	if(!get_last_attr(cfg, field, attr, val)) {return 0;}
	
	free_strptr(&field, MAX_SIZE);
	return 1;
}

void set_cfg_field(config* cfg, char begin, char end) {
	cfg->begin_field = begin;
	cfg->end_field = end;
}; 

void set_cfg_eol(config* cfg, char eol) {
	cfg->eol= eol;
}; 

void auto_cfg_setup(config* cfg) {
	set_cfg_field(cfg, '[', ']');
	set_cfg_eol(cfg, '\n');
	cfg->verbose = 0;
}

void cfg_setup(config* cfg, char eol, char begin, char end, char verbosity) {
	set_cfg_field(cfg, begin, end);
	set_cfg_eol(cfg, eol);
	cfg->verbose = verbosity;
}


int save_config(config* cfg, char* filename){
	FILE* fp = fopen(filename, "w");
	char* eol_str;
	if(!init_str(&eol_str, 2)) {return 0;}
	
	eol_str[0] = cfg->eol;
	char* copy_word = malloc(MAX_SIZE*sizeof(char));
	
	for(int i = 0; strcmp(cfg->words[i], ""); i++){
		if(strcmp(cfg->words[i], eol_str) != 0) {
			strcat(strcpy(copy_word, cfg->words[i]), " ");
		} else {
			strcpy(copy_word, cfg->words[i]);
		}
		
		if(fputs(copy_word, fp) == EOF){
			printf("config_reader: save_config: failed to save to file %s", filename);
			return 0;
		}
	}
	
	free_str(&eol_str);
	fclose(fp);
	return 1;
}

int set_field_attr(config* cfg, char* field_name, char* attr, char** new_val) {
	char* full_field_name = "";
	if(!init_str(&full_field_name, 50)) {return 0;}
	
	char* field_char;
	if(!init_str(&field_char, 2)) {return 0;}
	
	field_char[0] = cfg->begin_field;
	strcat(full_field_name, field_char);
	strcat(full_field_name, field_name);
	field_char[0] = cfg->end_field;
	strcat(full_field_name, field_char);
	
	int i = 0;
	int found = 0;
	
	for(i = 0; strcmp(cfg->words[i], ""); i++) {
		if(strcmp(cfg->words[i], full_field_name) == 0) {
			i+=2; /* two to go down a line */
			found = 1;
			break;
		}
	}
	if(found == 0) {
		printf("config_reader: set_field_attr: failed to find %s\n", full_field_name);
		return 0;
	}
	
	int findex = i; /* field index */
	char* new_line;
	if(!init_str(&new_line, 2)) {return 0;}
	new_line[0] = cfg->eol;
	new_line[1] = 0;
	int current_val  = 0;
	int current_val_number = 0;
	found = 0;
	int after_nl = 0;
	
	for(i = 0; strcmp(cfg->words[i+findex], "") && !is_field(cfg->words[i+findex], cfg->begin_field, cfg->end_field); i++) {
		if(strcmp(cfg->words[i+findex], new_line) == 0) {
			after_nl = 1;
		} 
		if(strcmp(cfg->words[i+findex], attr) == 0) {
			found = 1;
			i+=1;
			int curr_i = i;
			int curr_new_val = 0;
			int copying = 1;
			for(; strcmp(cfg->words[i+findex], "") != 0 && strcmp(cfg->words[i+findex], new_line) != 0 && 
							!is_field(cfg->words[i+findex], cfg->begin_field, cfg->end_field); i++) {
				if(copying){
					strcpy(cfg->words[i+findex], new_val[curr_new_val]);
				} else{
					strcpy(cfg->words[i+findex], " ");
				}
				curr_new_val++;
				if(strcmp(new_val[curr_new_val], "") == 0){
					copying = 0;
				}
			}
			if(strcmp(new_val[curr_new_val], "")){ /* we need to insert words! */
				insert_string_array(cfg->words, new_val+curr_new_val, findex+i);
			}
			current_val = 0;
			current_val_number++;
		}
		if(strcmp(cfg->words[i+findex], new_line)) {
			after_nl = 0;
		} 
	}
	
	if(found == 0) {
		printf("config_reader: set_field_attr: failed to find %s in field %s\n", attr, full_field_name);
		return 0;
	}
	
	free_str(&full_field_name);
	free_str(&new_line);
	return 1;
}

int close_config(config* cfg, int save){
	if(save){
		if(!save_config(cfg, cfg->filename)) {return 0;}
	}
	
	free_strptr(&cfg->words, MAX_SIZE);
	free_str(&cfg->filename);
	free(cfg);
	return 1;
}

int set_field(config* cfg, char* field_name, char** new_field){
	char* full_field_name = "";
	if(!init_str(&full_field_name, 50)) {return 0;}
	
	char* field_char;
	if(!init_str(&field_char, 2)) {return 0;}
	
	field_char[0] = cfg->begin_field;
	strcat(full_field_name, field_char);
	strcat(full_field_name, field_name);
	field_char[0] = cfg->end_field;
	strcat(full_field_name, field_char);
	
	int i = 0;
	int found = 0;
	
	for(i = 0; strcmp(cfg->words[i], ""); i++) {
		if(strcmp(cfg->words[i], full_field_name) == 0) {
			i+=2; /* two to go down a line */
			found = 1;
			break;
		}
	}
	
	if(found == 0) {
		printf("config_reader: set_field_attr: failed to find %s\n", full_field_name);
		return 0;
	}
	
	int findex = i; /* field index */
	
	char* new_line;
	init_str(&new_line, 2);
	new_line[0] = cfg->eol;
	new_line[1] = 0;
	found = 0;
	
	for(i = 0; strcmp(cfg->words[i+findex], "") && !is_field(cfg->words[i+findex], cfg->begin_field, cfg->end_field)
			&& strcmp(new_field[i], ""); i++){
		cfg->words[i+findex] = new_field[i];
	}
	
	if(strcmp(new_field[i], "")) {
		insert_string_array(cfg->words, new_field+i, i+findex);
	}
	
	free_str(&full_field_name);
	free_str(&new_line);
	return 1;

}


/* TEST CODE 
int main(){
	config cfg;
	cfg_setup(&cfg, '\n', '[', ']');
	printf("%d\n", config_reader("..\\example.cfg", &cfg));
	char** field;
	init_strptr(&field, MAX_SIZE, MAX_SIZE);
	char*** val;
	init_strptrptr(&val, 20, 10, 50);
	char** fval;
	init_strptr(&fval, 10, 50);
	char** lval;
	init_strptr(&lval, 10, 50);
	get_field(cfg, "FIELD1", field);
	printf("FIELD1: \n");
	for(int i = 0; strcmp(field[i], ""); i++) {
		printf("%s ", field[i]);
	}
	printf("attr MIN: \n");
	dir_get_last_attr(cfg, "FIELD1", "TEST", lval);
	for(int i = 0; strcmp(lval[i], ""); i++) {
		printf("%d: %s ", i, lval[i]);
	}
	field[0] = "test";
	field[1] = "\n";
	field[2] = "for";
	field[3] = "\n";
	field[4] = "field";
	field[5] = "change";
	
	printf("FIELD1: \n");
	for(int i = 0; strcmp(field[i], ""); i++) {
		printf("%s ", field[i]);
	}
	set_field(&cfg, "FIELD2", field);
	printf("\n\nPRINTING WORDS\n\n");
	for(int i = 0; strcmp(cfg.words[i], ""); i++) {
		printf("%s ", cfg.words[i]);
	}
	save_config(&cfg, "new.cfg");
	close_config(&cfg, 0);
	return 0;
} */

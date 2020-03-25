#define MAX_SIZE 255
#ifndef CONFIG_READER_HEADER
#define CONFIG_READER_HEADER
typedef struct {
	/* EOL is also a word, for easier reading */
	char words[MAX_SIZE][MAX_SIZE]; 
	char begin_field;
	char end_field;
	char eol;
} config;

int is_field(char* str);

/* reads the file into buffer (ONLY USED IN CONFIG READER)*/
int file_reader(char* filename, char* buffer);

/* reads file data (buffer) into cfg */
int config_reader(char* buffer, config* cfg);

/* gets field field_name, and puts that data into field */
int get_field(config cfg, char* field_name, char field[][255]);

/* gets attribute from field */
int get_attr(config cfg, char field[][255], char attr[], char val[][50][50]);

/* gets first attribute of name attr */
int get_first_attr(config cfg, char field[][255], char attr[], char val[][50]);

/* gets last attribute of name attr */
int get_last_attr(config cfg, char field[][255], char attr[], char val[][50]);

/* direct versions without the field inbetween */
int dir_get_attr(config cfg, char field[], char attr[], char val[][50][50]);

int dir_get_first_attr(config cfg, char fieldname[], char attr[], char val[][50]);

int dir_get_last_attr(config cfg, char fieldname[], char attr[], char val[][50]);

void set_cfg_field(config* cfg, char begin, char end); 

void set_cfg_eol(config* cfg, char eol);
#endif
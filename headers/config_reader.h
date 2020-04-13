#define MAX_SIZE 512
#ifndef CONFIG_READER_HEADER
#define CONFIG_READER_HEADER
typedef struct {
	/* EOL is also a word, for easier reading */
	char** words; 
	char*  filename;
	char   begin_field;
	char   end_field;
	char   eol;
	char   verbose;
} config;

int is_field(char* str, char begin, char end);

/* reads the file into buffer (ONLY USED IN CONFIG READER)*/
int file_reader(char* filename, char* buffer);

/* reads file data (buffer) into cfg */
int config_reader(char* buffer, config* cfg); //good

/* gets field field_name, and puts that data into field */
int get_field(config cfg, char* field_name, char** field); 

/* gets attribute from field */
int get_attr(config cfg, char** field, char* attr, char*** val); 

/* gets first attribute of name attr */
int get_first_attr(config cfg, char** field, char* attr, char** val); 

/* gets last attribute of name attr */
int get_last_attr(config cfg, char** field, char* attr, char** val);

/* direct versions without the field inbetween */
int dir_get_attr(config cfg, char* field, char* attr, char*** val);

int dir_get_first_attr(config cfg, char* fieldname, char* attr, char** val);

int dir_get_last_attr(config cfg, char* fieldname, char* attr, char** val);

/* setup functions */
void set_cfg_field(config* cfg, char begin, char end); 

void set_cfg_eol(config* cfg, char eol);

void cfg_setup(config* cfg, char eol, char begin, char end, char verbosity);

/* setup with default values */
void auto_cfg_setup(config* cfg);

/* edit a config, all are direct */
/* in field, change all values named valname to new_val */
int set_field_attr(config* cfg, char* field_name, char* attr, char** new_val);

int set_field(config* cfg, char* field_name, char** new_field);

int save_config(config* cfg, char* filename);

/* destroy the config */
int close_config(config* cfg, int save);


#endif
#include <stdlib.h>
#include <string.h>

int readfile(FILE* fp, char buff[])
{
	char c = 0;
	int i = 0;
	int l_num = 1;
	// repeat until c is EOF
	while(c != EOF )
	{
		c = fgetc((FILE*)fp);
		//printf("%c\n", c);
		buff[i] = c;
		if(c == '\n')
		{
			l_num+=1;
		}
		i++;
	}
	buff[--i] = '\n';
	return l_num; 
}

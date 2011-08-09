#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXBUFSIZE 1024

int checkAndSetConfig(void);
char password[30] = {};
char username[30] = {};

int main ( int argc, char * argv[] )
{

	if(checkAndSetConfig() == -1)
		printf("no conf file\n");

	printf("%s\t%s\n", username, password);
	return 0;
}  

//only read password & username form file mystar.conf
int checkAndSetConfig(void)
{
    FILE  *fp;
    char   buf[1024];
	char  *buf_b;
    char  *p;
    int   i;

    if( (fp=fopen("mystar.conf","r"))==NULL ){
 		if( (fp=fopen("/etc/mystar.conf","r"))==NULL )
			return -1;//no config file
    }
	printf("file found\n");

	while(fgets(buf, sizeof(buf)-1, fp) != NULL)
    {
		buf_b = buf;
     	if( (buf[0]=='#') || (buf[0]=='\n') ) continue;

     	if(((p=strchr(buf,'=')) == 0) || (p==buf) )  continue;

		buf[strlen(buf) - 1] = '\0';
     	for(buf_b = buf; *buf_b == ' ' && buf_b < p; buf_b++);//delete front space
		for(p+=1 ; *p != NULL && *p == ' '; p++);
printf("%s, %d\n", buf_b, strlen(buf_b));		
printf("%s, %d\n", p, strlen(p));
	
		if(strncmp(buf_b, "username", strlen("username")) == 0){ //is username?
			for(i = 0; p[i] != '\0' && p[i] != ' '; i++){
				username[i] = p[i];
				username[i+1] = '\0';
				printf("%c\n", p[i]);
			}		
		}
		
		if(strncmp(buf_b, "password", strlen("password")) == 0){ //is username?
			for(i = 0; p[i] != '\0' && p[i] != ' '; i++){
				password[i] = p[i];
				password[i+1] = '\0';
				printf("%c\n", p[i]);
			}		
		}
	}

	printf("%s\t%s\n", username, password);

    fclose(fp);
	if((strlen(password) == 0) || (strlen(username) == 0))
		return -1;
	
	
	return 0;//read username & passwork done
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "btx.h"


/*This function reads the input of the page viewer
 * Initiator and terminator get translated to * and #
 * Text is filtered to simple digits and characters
 */
int read_keyboard(int fd)
{
	char c=0;
	int res=read(fd, &c, sizeof(c));
	if (res!=1) return -1;
	if (c==INI) return '*';
	if (c==TER) return '#';
	if (c<'0') return -1;
	if (c<='9') return c;
	if (c<'A') return -1;
	if (c<='Z') return c;
	if (c<'a') return -1;
	if (c<='z') return c;
	return -1;
}



int display_page(const int input, const int output, const char *dir, const char *page)
{
	char pagefilename[PATHLEN+PAGENAMELEN+2];
	memset(pagefilename, 0, sizeof(pagefilename));
	snprintf(pagefilename, sizeof(pagefilename)-1, "%s/%s", dir, page);
	int pf=open(pagefilename, O_RDONLY);
	if (pf==-1) return -2;
	char c=0;
	while (read(pf, &c, 1)==1) {
		write(output, &c, 1);
		int res=read_keyboard(input);
		if (res>=0) {
			close(pf);
			return res;
		}
	}
	fsync(output);

	close(pf);
	return 0;
}


int handle_page(const int input, const int output, const char *dir, const char *page, char *next_page)
{
	next_page[0]=0;
	//set input to non blocking
	int flags = fcntl(input, F_GETFL, 0);
	fcntl(input, F_SETFL, flags | O_NONBLOCK);
	//Display page
	int res=display_page(input, output, dir, page);
	if (res<0) return res;

	//set input to blocking again
	fcntl(input, F_SETFL, flags);

	//Make the terminal talk
	char c=0x1A;
	write(output, &c, 1);
	c=0x11; //Turn on cursor
	write(output, &c, 1);

	char buffer[32];
	memset(buffer, 0, sizeof(buffer));
	gotoxy(output, 1, 24);
	int p=-1;
	if (res>0) {
		p=p+1;
		buffer[p]=res;
		c=buffer[p];
		write(output, &c, 1);
	}
	while (0==0) {
		if (buffer[p]=='#') return 0;
		res=read_keyboard(input);
		if (res<0) continue;
		p=p+1;
		buffer[p]=res;
		c=buffer[p];
		write(output, &c, 1);	
	}
}


/* return -1 for backwards
 * */
int browser(int input, int output, const char *dir, const char *page) 
{
	char next_page[PAGENAMELEN];
	while (0==0) {
		memset(next_page, 0, sizeof(next_page));
		int res=handle_page(input, output, dir, page, next_page);
		if (res<0) return -1; //Invalid page, go back
		if (strcmp(next_page, "*#")==0) return -1;
		int len=strlen(next_page);
		if (next_page[0]=='*') { //Full page number
			while (next_page[len-1]=='#') {
				next_page[len-1]=0;
				len=strlen(next_page);
			}
		}
	}
}




int main(int argc, char **argv)
{
	if (argc!=2) {
		printf("Usage %s <data directory>\nCall from inetd\n", argv[0]);
		return 1;
	}
	int fd=0; //stdin
	//Set stdin to non blocking
	return 0;
}

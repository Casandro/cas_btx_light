#include "btx.h"

#include <unistd.h>
#include <fcntl.h>


void set_fd_nonblocking(int fd)
{
	//set input to non blocking
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void set_fd_blocking(int fd)
{
	//set input to non blocking
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags & ((~0) ^ O_NONBLOCK));
}


int gotoxy(const int output, const int x, const int y)
{
	char buf[3];
	buf[0]=0x1f; //US
	buf[1]=x+0x40;
	buf[2]=y+0x40;
	write(output, buf, 3);
}

void cursor_on(const int output)
{
	char buf[2];
	buf[0]=0x11; //Cursor on
	buf[1]=0x1a; //Make terminal talk
	write(output, &buf, 2);
}

void cursor_off(const int output)
{
	char buf[2];
	buf[0]=0x14; //Cursor on
	write(output, &buf, 1);
}

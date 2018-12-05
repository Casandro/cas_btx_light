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

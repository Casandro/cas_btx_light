#pragma once


//control characters from the terminal
#define INI (0x13)
#define TER (0x1c)
#define APB (0x08)
#define APF (0x09)
#define APD (0x0A)
#define APU (0x0B)
#define APR (0x0D)


#define PAGENAMELEN (32)
#define HISTOLEN (16)
#define PATHLEN (128)

void set_fd_nonblocking(int fd);
void set_fd_blocking(int fd);
int gotoxy(const int output, const int x, const int y);

void cursor_on(const int output);
void cursor_off(const int output);

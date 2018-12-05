#include "form.h"
#include <stdio.h>
#include <stdlib.h>
#include "btx.h"

field_t *add_field(field_t *p, const int x, const int y, const int len, const char *pre)
{
	field_t *f=malloc(sizeof(field_t));
	if (f==NULL) return p;
	memset(f,0,sizeof(field_t));
	f->prev=p;
	if (p!=NULL) {
		f->next=p->next;
		if (p->next!=NULL) p->next->prev=f;
		p->next=f;
	}
	f->x=x;
	f->y=y;
	f->len=len;
	f->value=malloc(len+1); //Store length +1
	if (f->value==NULL) {
		f->len=0;
		return f;
	}
	memset(f->value,0,len+1);
	if (pre!=NULL) strncpy(f->value, pre, f->len);
	return f;
}


void *delete_field(field_t *f)
{
	if (f==NULL) return;
	if (f->next!=NULL) f->next->prev=f->prev;
	if (f->prev!=NULL) f->prev->next=f->next;
	free(f->value);
	free(f);
}

field_t *find_first_field(field *p)
{
	if (p==NULL) return NULL;
	if (p->prev==NULL) return p;
	return find_first_field(p->prev);
}

void delete_all_fields(field_t *p)
{
	field_t *f=find_first_field(p);
	if (f==NULL) return;
	while (f!=NULL) {
		field_t *next=f->next;
		delete_field(f);
		f=next;
	}
}


char read_char(const int fd)
{
	char b=0;
	int res=read(fd, &b, 1);
	if (res!=1) return -1;
	return b;
}

char read_digit_or_command(const int fd)
{
	char b=0;
	while ( (b=read_char(fd))>0) {
		if (b==INI) return '*';
		if (b==TER) return '#';
		if (b==APB) return b;
		if (b==APF) return b;
		if (b==APU) return b;
		if (b==APD) return b;
		if (b==APR) return b;
		if ( (b>='0') && (b<='9') ) return b;
	}
	return b;
}

char parse_digit_or_command(const int fd)
{
	char b=read_digit_or_command(fd);
	if (b=='*') { //Handle sequences starting with *
		b=read_digit_or_command(fd);
		if (b=='*') return APB; //**=>backward
		return b;
	}
	return b;
}


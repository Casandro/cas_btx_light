#include "form.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
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


void delete_field(field_t *f)
{
	if (f==NULL) return;
	if (f->next!=NULL) f->next->prev=f->prev;
	if (f->prev!=NULL) f->prev->next=f->next;
	free(f->value);
	free(f);
	return;
}

field_t *find_first_field(field_t *p)
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




void print_field(field_t *f, const int out)
{
	if (f==NULL) return;
	gotoxy(out, f->x, f->y);
	int n;
	for (n=0; n<f->len; n++) {
		char b=f->value[n];
		if (b<32) b='.';
		if (b>127) b='.';
		write(out, &b, 1);
	}
}

field_t *edit_field(field_t *f, const int inp, const int out)
{
	if (f==NULL) return NULL;
	print_field(f, out);
	int cursor=0;
	while (0==0) {
		gotoxy(out, f->x+cursor, f->y);
		cursor_on(out);
		char b=parse_digit_or_command(inp);
		if (b==APB) {//backward key
			cursor=cursor-1;
		} else
		if (b=='#') { //enter
			int n;
			for (n=cursor; n<f->len; n++) f->value[n]=0;
			print_field(f, out);
			return f->next;
		} else
		if (b==APF) {//right arrow
			cursor=cursor+1;
		} else 
		if (b==APU) return f->prev; else
		if (b==APD) return f->next; else
		if ( (b>='0') && (b<='9') ) {
			f->value[cursor]=b;
			write(out, &b, 1);
			cursor=cursor+1;
		}
		if (cursor<0) return f->prev;
		if (cursor>=f->len) return f->next;
	}
	return NULL; //Should never come here
}

void print_form(field_t *f, const int out)
{
	field_t *s=find_first_field(f);
	while (s!=NULL) {
		print_field(s, out);
		s=s->next;
	}
	return;
}

void edit_form(field_t *f, const int inp, const int out)
{
	print_form(f, out);
	field_t *fld=f;
	while (fld!=NULL) {
		fld=edit_field(fld, inp, out);
	}
	return;
}

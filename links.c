#include "links.h"
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "form.h"

char *copystr(const char *s)
{
	size_t len=strlen(s);
	char *c=malloc(len+1);
	if (c==NULL) return NULL;
	memset(c, 0, len+1);
	strncpy(c, s, len);
	return c;
}


link_t *add_link(link_t *next, const char *num, const char *page)
{
	link_t *l=malloc(sizeof(link_t));
	if (l==NULL) return next;
	memset(l, 0, sizeof(link_t));
	size_t len=strlen(num);
	l->num=copystr(num);
	l->page=copystr(page);
	l->next=next;
	return l;
}

void delete_links(link_t *l)
{
	if (l==NULL) return;
	free(l->num);
	free(l->page);
	link_t *n=l->next;
	free(l);
	return delete_links(n);
}

link_t *read_links(const char *fn)
{
	link_t *next=NULL;
	FILE *f=fopen(fn, "r");
	if (f==NULL) return NULL;
	char line[256];
	while (fgets(line, sizeof(line), f)!=NULL) {
		char *page=strchr(line, '=');
		if (page==NULL) continue;
		*page=0;
		page=page+1;
		next=add_link(next, line, page);	
	}
	fclose(f);
	return next;
}

void write_links(link_t *start, const char *fn)
{
	if (start==NULL) return;
	FILE *f=fopen(fn,"w");
	if (f==NULL) return;
	link_t *link=start;
	while (link!=NULL) {
		fprintf(f, "%s=%d\n", link->num, link->page);
		link=link->next;
	}
	fclose(f);

}




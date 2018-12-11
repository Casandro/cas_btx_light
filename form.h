#pragma once

typedef struct field_s {
	struct field_s *next;
	struct field_s *prev;
	int x;
	int y;
	int len;
	int type;
	char *value;
} field_t;


void edit_form(field_t *f, const int inp, const int out);
field_t *add_field(field_t *p, const int x, const int y, const int len, const char *pre);
void delete_all_fields(field_t *p);

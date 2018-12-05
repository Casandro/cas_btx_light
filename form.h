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



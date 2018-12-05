#pragma once

typedef struct link_s {
	struct link_s *next;
	char *num; //Short number eg 10
	char *page; //long number eg 123432
} link_t;

#ifndef SED_OPERATIONS_H
#define SED_OPERATIONS_H
#include <regex.h>

void replace_text(char *filename, const char *old_text, const char *new_text);

void delete_lines(char *filename, const char *pattern);

void insert_text(char *filename, const char *text, int at_beginning);

#endif // SED_OPERATIOND_H

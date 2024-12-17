#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "sed_operations.h"

void replace_text(char *filename, const char *old_text, const char *new_text)
{
	FILE *file = fopen(filename, "r+");

	if (!file)
	{
		perror("Ошибка открытия файла.");
		return;
	}

	char *buf;
	size_t size = 0;
	long file_size;

	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	buf = malloc(file_size + 1);
	fread(buf, 1, file_size, file);
	buf[file_size] = '\0';

	char *pos = buf;
	while ((pos == strstr(pos, old_text)) != NULL)
	{
		char *new_buf = malloc(file_size + strlen(new_text) - strlen(old_text) + 1);
		strncpy(new_buf, buf, pos - buf);
		strcpy(new_buf + (pos - buf), new_text);
		strcpy(new_buf + (pos - buf) + strlen(new_text), pos + strlen(old_text));

		free(buf);
		buf = new_buf;
		file_size = strlen(buf);
		pos += strlen(new_text);
	}

	fseek(file, 0, SEEK_SET);
	ftruncate(fileno(file), 0);
	fwrite(buf, 1, file_size, file);
	free(buf);
	fclose(file);
}

void delete_lines(char *filename, const char *pattern)
{
	FILE *file = fopen(filename, "r+");

	if (!file)
	{
		perror("Ошибка открфтия файла");
		return;
	}

	regex_t regex;

	if (regcomp(&regex, pattern, 0) != 0)
	{
		printf("Ошибка компиляции регулярного выражения.\n");
		fclose(file);
		return;
	}

	FILE *temp_file = tmpfile();

	if (!temp_file)
	{
		perror("Ошибка открфтия файла");
		regfree(&regex);
		fclose(file);
		return;
	}

	char line[1024];
	while (fgets(line, sizeof(line), file))
	{
		if (regexec(&regex, line, 0, NULL, 0) != 0)
		{
			fputs(line, temp_file);
		}
	}

	rewind(temp_file);
	fseek(file, 0, SEEK_SET);
	ftruncate(fileno(file), 0);

	while (fgets(line, sizeof(line), temp_file))
	{
		fputs(line, file);
	}

	fclose(temp_file);
	regfree(&regex);
	fclose(file);
}

void insert_text(char *filename, const char *text, int at_beginning)
{
	FILE *file = fopen(filename, "r+");
	
	if (!file)
	{
		perror("Ошибка открфтия файла");
		return;
	}

	FILE *temp_file = tmpfile();

	if (!temp_file)
	{
		perror("Ошибка создания временного файла");
		fclose(file);
		return;
	}

	char line[1024];

	while (fgets(line, sizeof(line), file))
	{
		if (at_beginning)
		{
			fprintf(temp_file, "%s%s", text, line);
		}

		else
		{
			fprintf(temp_file, "%s%s", line, text);
		}
	}

	rewind(temp_file);
	fseek(file, 0, SEEK_SET);
	ftruncate(fileno(file), 0);

	while (fgets(line, sizeof(line), temp_file))
	{
		fputs(line, file);
	}

	fclose(temp_file);
	fclose(file);
}


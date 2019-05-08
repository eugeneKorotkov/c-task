#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

int is_symbol(char c) {
	return !(c == ' ' || c == '\n' || c == '\t');
}

int main() {
	char ch, file_name[50];
	FILE* fp;

	printf("file name: ");
	gets(file_name);

	fp = fopen(file_name, "r"); // открываем файл на чтение

	if (fp == NULL)
	{
		perror("error while opening file\n");
		exit(EXIT_FAILURE);
	}

	fseek(fp, 0L, SEEK_END); // прыгаем в конец файла
	long bytes_cnt = ftell(fp); // сколько байт считали
	fseek(fp, 0L, SEEK_SET); // прыгаем в начало

	char prev = ' '; // предыдуий считанный символ
	int lines_cnt = 1; // строк = 1 + количество переводов строк
	int words_cnt = 0;
	while ((ch = fgetc(fp)) != EOF) {
		if (!is_symbol(prev) && is_symbol(ch)) { // начало слова = сначала шел пробел, таб или перевод, а потом идет символ
			words_cnt++;
		}
		if (ch == '\n') {
			lines_cnt++;
		}
		prev = ch;
	}
	printf("%d lines, %d words, %d bytes", lines_cnt, words_cnt, bytes_cnt);

	fclose(fp);
	return 0;
}
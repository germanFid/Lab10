#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

#define MENU_CHOICES_NUM 5
#define PATH_DEFAULT_SIZE 250
#define GETSTRING_ERR_RE -1
#define GETSTRING_ERR_OVERFLOW -2

void clearExtraInput()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}

int getIntChoice(int n, int* choices)
{
	char buf[3];

	scanf_s("%3s", buf, 3);
	clearExtraInput();

	char* end;
	int choice = (int)strtol(buf, &end, 10);

	if (choice == 0)
		return -1;

	for (size_t i = 0; i < n; i++)
	{
		if (choices[i] == choice)
			return choice;
	}

	return -1;
}

void waitForKey()
{
	system("pause");
}

int getString(char* str, const int size)
{
	char* buf = (char*) malloc(size + 1);
	if (gets_s(buf, size) == NULL)
	{
		return GETSTRING_ERR_RE;
	}

	if (strlen(buf) == size)
	{
		return GETSTRING_ERR_OVERFLOW;
	}

	strcpy_s(str, size, buf);
	free(buf);
	
	return 0;
}

FILE* openFileHandler(char* filename)
{
	int result;

	FILE* ptr;

	system("cls");
	printf("Enter path to file (less than 250 symbols!)\n");
	
	do
	{
		printf("\n>");
		result = getString(filename, PATH_DEFAULT_SIZE);

		if (result == GETSTRING_ERR_OVERFLOW)
		{
			printf("You've entered too many symbols!\n");
		}

		if (result == GETSTRING_ERR_RE)
		{
			printf("Reading Error!\n");
		}

	} while (result != 0);

	ptr = openFile(filename);

	waitForKey();
	return ptr;
}

void displayFileHandler(FILE* ptr, char* filename)
{
	int fileOpenedFlag = 0;
	if (ptr == NULL)
	{
		ptr = openFileHandler(filename);
		fileOpenedFlag = 1;
	}

	system("cls");
	printf("%s\n"
		"------------\n", filename);
	int result = printFile(ptr);

	if (result)
	{
		if (result == EACCES)
		{
			printf("Access failure!\n");
		}

		else
		{
			printf("Some failure happened!\n");
		}
	}

	if (fileOpenedFlag)
	{
		fclose(ptr);
		ptr = NULL;
	}
	waitForKey();
}

void writeFileHandler(FILE* ptr, char* filename)
{
	int fileOpenedFlag = 0;
	if (ptr == NULL)
	{
		ptr = openFileHandler(filename);
		fileOpenedFlag = 1;

		if (ptr == NULL)
		{
			return;
		}
	}

	char* str = (char*)malloc(PATH_DEFAULT_SIZE);
	int result;

	system("cls");
	printf("Enter string to write (less than 250 symbols!)\n");

	do
	{
		printf("\n>");
		result = getString(str, PATH_DEFAULT_SIZE);

		if (result == GETSTRING_ERR_OVERFLOW)
		{
			printf("You've entered too many symbols!\n");
		}

		if (result == GETSTRING_ERR_RE)
		{
			printf("Reading Error!\n");
		}

	} while (result != 0);

	fclose(ptr);
	fopen_s(&ptr, filename, "w");

	result = writeFile(ptr, str);
	if (result)
	{
		if (result == 13)
			printf("Access Error!");
		else
			printf("Writing error!");
	}

	fclose(ptr);
	if (fileOpenedFlag)
	{
		ptr = NULL;
	}

	else
	{
		fopen_s(&ptr, filename, "r+");
	}
}

void showMenu(FILE* fp, char* currentFileName)
{
	if (fp != NULL)
	{
		printf("------------\nCurrent File: %s\n------------\n", currentFileName);
	}

	else
	{
		printf("------------\nNo file opened!\n------------\n");
	}

	printf(
		"1. Create/Open file\n"
		"2. Read file\n"
		"3. Write string to file\n"
		"4. Change security attributes of the file\n"
		"99. Exit\n\n>");
}
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "file.h"

#define MENU_CHOICES_NUM 6
#define MOD_CHOICES_NUM 9

#define UNLEN 256

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

		if (ptr == NULL)
		{
			printf("\033[31mOpertaion Failed!\033[0m\n");
			waitForKey();
			return;
		}
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

void getUserNameHandler(char* username)
{
	DWORD ln = UNLEN + 1;
	GetUserName(username, &ln);
}

void modifySecurityAttributesHandler(FILE* ptr, char* filename, char* currentUserName)
{
	int fileOpenedFlag = 0;
	if (ptr == NULL)
	{
		ptr = openFileHandler(filename);
		fileOpenedFlag = 1;

		if (ptr == NULL)
		{
			printf("File can't be opened, trying to modify parameters without precheck!\n");
			waitForKey();
		}
	}

	if (ptr != NULL)
	{
		fclose(ptr); // close file for proper rights modification
		ptr = NULL;
	}

	system("cls");
	
	printf("------------\nCurrent File: %s\n------------\n", filename);
	printf("Select operation type:\n"
	"1. Grant all rights to the current user\n"
	"2. Revoke all rights\n"
	"3. Grant read permission\n"
	"4. Revoke read permission\n"
	"5. Grant write permission\n"
	"6. Revoke wrie permission\n"
	"7. Grant delete permission\n"
	"8. Revoke delete permission\n"
	"99. Exit to main menu\n\n>");

	int choices[] = { 1, 2, 3, 4, 5, 6, 7, 8, 99 };
	int choice;

	DWORD GENERIC_TYPE;
	DWORD ACCESS_TYPE;

	do
	{
		choice = getIntChoice(MOD_CHOICES_NUM, choices);

		switch (choice)
		{
		case 1:
			GENERIC_TYPE = GENERIC_ALL;
			ACCESS_TYPE = SET_ACCESS;
			break;

		case 2:
			GENERIC_TYPE = GENERIC_ALL;
			ACCESS_TYPE = DENY_ACCESS;
			break;

		case 3:
			GENERIC_TYPE = GENERIC_READ;
			ACCESS_TYPE = SET_ACCESS;
			break;

		case 4:
			GENERIC_TYPE = GENERIC_READ;
			ACCESS_TYPE = DENY_ACCESS;
			break;
		
		case 5:
			GENERIC_TYPE = GENERIC_WRITE;
			ACCESS_TYPE = SET_ACCESS;
			break;

		case 6:
			GENERIC_TYPE = GENERIC_WRITE;
			ACCESS_TYPE = DENY_ACCESS;
			break;

		case 7:
			GENERIC_TYPE = DELETE;
			ACCESS_TYPE = SET_ACCESS;
			break;

		case 8:
			GENERIC_TYPE = DELETE;
			ACCESS_TYPE = DENY_ACCESS;
			break;

		case 99:
			waitForKey();
			return;
			break;

		default:
			printf("incorrect input!\n\n>");
			break;
		}
		
	} while (choice == -1);

	DWORD result = AddAceToObjectsSecurityDescriptor(TEXT(filename), SE_FILE_OBJECT, TEXT(currentUserName), TRUSTEE_IS_NAME, GENERIC_TYPE, ACCESS_TYPE, NO_INHERITANCE);

	if (result == 0L)
	{
		printf("\033[32mSUCCESS!\033[0m\n");
	}

	else printf("\033[31mOpertaion Failed!\033[0m\n");
	waitForKey();
}

void showMenu(FILE* fp, char* currentFileName, char* currentUserName)
{
	printf("Welcome, \033[35m%s\033[0m!\n", currentUserName);
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
		"4. Change security attributes of the file (current file will be closed!)\n"
		"5. Close current file\n"
		"99. Exit\n\n>");
}

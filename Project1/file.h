#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "ui.h"

#define FILE_OP_SUCCESS 0
#define FILE_OP_FAIL -1

#define STR_BUFFER_LEN 250

FILE* openFile(char* filename)
{
	FILE* fp = NULL;
	int result;

	result = fopen_s(&fp, filename, "r+");
	if (result != FILE_OP_SUCCESS)
	{
		if (result == EACCES)
		{
			printf("Cannot open this file with r+ attributes due to access failure!\n");
			return NULL;
		}

		printf("File Is not found, creating new...\n");
		result = fopen_s(&fp, filename, "w+");

		if (result != FILE_OP_SUCCESS)
		{
			if (result == EACCES)
			{
				printf("Cannot open this file with r+ attributes due to access failure!\n");
			}

			if (result == EFAULT)
			{
				printf("Provided adress is incorrect!\n");
			}

			printf("Cannot create file!\n");
			return NULL;
		}

		if (fp == NULL)
		{
			printf("File creation failure!\n");
			return NULL;
		}
	}
	
	printf("SUCCESS!\n");
	return fp;
}

int printFile(FILE* ptr) // printing file line by line
{
	char* buf = malloc(STR_BUFFER_LEN);

	fseek(ptr, 0, SEEK_SET);
	while (fgets(buf, STR_BUFFER_LEN, ptr)) {
		printf("%s\n", buf);
	}

	int err;
	if (err = ferror(ptr))
	{
		return err;
	}

	return 0;
}

int writeFile(FILE* ptr, char* str)
{
	if (ptr == NULL)
	{
		return FILE_OP_FAIL;
	}

	int result = fseek(ptr, 0, SEEK_SET);

	if (result)
	{
		return result;
	}

	fprintf_s(ptr, "%s", str);
	return 0;
}

#include <stdio.h>
#include <accctrl.h>

#include "acl.h"
#include "ui.h"
#include "file.h"

int main(int argc, char* argv[])
{
	int choice;
	int availableChoices[] = {1, 2, 3, 4, 99};

	FILE* currentFile = NULL;
	char* currentFileName = malloc(PATH_DEFAULT_SIZE);
	ZeroMemory(currentFileName, PATH_DEFAULT_SIZE);

	while (1)
	{
		showMenu(currentFile, currentFileName);

		choice = getIntChoice(MENU_CHOICES_NUM, availableChoices);
		if (choice == -1)
		{
			printf("Invalid Input!\n\n");
			continue;
		}

		switch (choice)
		{
		case 1:
			if (currentFile != NULL)
			{
				fclose(currentFile);
			}

			currentFile = openFileHandler(currentFileName);
			break;

		case 2:
			displayFileHandler(currentFile, currentFileName);
			break;

		case 3:
			break;

		case 99:
			free(currentFileName);
			
			if(currentFile != 0)
				fclose(currentFile);

			exit(EXIT_SUCCESS);
			break;

		default:
			break;
		}

		system("cls");
	}
	return 0;
}

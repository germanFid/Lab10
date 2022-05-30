#include <stdio.h>
#include <accctrl.h>

#include "acl.h"
#include "ui.h"
#include "file.h"

int main(int argc, char* argv[])
{
	int choice;
	int availableChoices[] = {1, 2, 3, 4, 5, 99};

	FILE* currentFile = NULL;
	char* currentFileName = malloc(PATH_DEFAULT_SIZE);
	char* currentUserName = malloc(sizeof(wchar_t) * (UNLEN + 1));

	ZeroMemory(currentFileName, PATH_DEFAULT_SIZE);
	getUserNameHandler(currentUserName);

	while (1)
	{
		showMenu(currentFile, currentFileName, currentUserName);

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
			writeFileHandler(currentFile, currentFileName);
			break;

		case 4:
			modifySecurityAttributesHandler(currentFile, currentFileName, currentUserName);
			break;

		case 5:
			if (currentFile != NULL)
			{
				fclose(currentFile);
				currentFile = NULL;
				printf("File closed!\n");
			}

			waitForKey();
			break;

		case 99:
			free(currentFileName);
			free(currentUserName);
			
			if(currentFile != NULL)
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

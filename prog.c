/*********************************
* Class: MAGSHIMIM C2			 *
* Week:                			 *
* Name:                          *
* Credits:                       *
**********************************/

#include <dirent.h> 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>


int twentycheck = 0;

void menu(char** argv);

void logMenu(char** argv, FILE* log, int choose);

#define CLEAR 100

int main(int argc, char** argv)
{
	int count = 0;

	FILE* f;
	FILE* v;
	FILE* log;

	int i = 0;

	struct dirent *de;  // Pointer for directory entry
	char dirPath[BUFSIZ] = "";

	char virusInside[BUFSIZ];

	size_t n;

	int sizeF = 0;
	int sizeV = 0;

	int virus = 0;
	int choose = 0;
	
	
	char logPath[BUFSIZ] = "";
	char logtxt[] = "Log.txt";
	
	char copyDirPath[BUFSIZ] = "";
	char filePaths[BUFSIZ] = "";
	
	int endCheck = 0;

	int len = 0;
	
	// opendir() returns a pointer of DIR type.
	DIR *dr = opendir(argv[1]);

	if (dr == NULL)  // opendir returns NULL if couldn't open directory 
	{

		printf("Could not open current directory");
		getchar();
		return 0;
	}

	//Reads the dir
	//dirPath[1] = argv[0][2];

	for (i = 0; i < strlen(argv[1]); i++)
	{
		dirPath[i] = argv[1][i];
	}
	

	menu(argv);

	scanf("%d", &choose);
	getchar();
	

	//adds a '\' to the end of the dir path
	dirPath[strlen(argv[1])] = argv[0][2];
	
	//keeps the dir path for later
	strcpy(copyDirPath, dirPath);
	

	len = strlen(dirPath);

	//try to check if the Log.txt already exist if not then he creates one if it does then its rewrite everything
	for (i = 0; i < strlen(logtxt); i++)
	{
		dirPath[len + i] = logtxt[i];
	}
	
	log = fopen(dirPath, "w");
	if (!log)
	{
		log = fopen(dirPath, "a+");
	}
	
	//prints the menu to the Log.txt
	logMenu(argv, log, choose);

	fclose(log);
	

	printf("Scanning began...\n");
	printf("This process may or may not take several minutes... \n\n");
	
	printf("Scanning: \n");

	//to give filepaths the dir header
	strcpy(filePaths, copyDirPath);

	// for readdir()
	while ((de = readdir(dr)) != NULL)
	{
		twentycheck = 0;
		//printf("\n%s\n", de->d_name);
		count++;

		//there are dots at the start this is just to skip them
		if (count >= 3)
		{
			//strcpy(copyDirPath2, copyDirPath);

			//to clear the array
			for (i = 0; i < CLEAR; i++)
			{
				filePaths[len + i] = ' ';
				
			}
			
			//normal reset
			endCheck = 0;
			
			//gets the file name into the dir header array
			for (i = 0; i < strlen(de->d_name); i++)
			{

				filePaths[len + i] = de->d_name[i];
				
			}
			
			//opening files
			f = fopen(filePaths, "rb");
			v = fopen(argv[2], "rb");
			log = fopen(dirPath, "a");

			//checks if the name of the file is = to the name of the Log file if it does that means that we are at the end of the file so it exit the loop
			for (i = 0; i < strlen(logtxt); i++)
			{
				if (de->d_name[i] == logtxt[i])
				{
					endCheck++;
				}
			}
			
			if (endCheck == 7)
			{
				printf("Scan Completed.\n");
				printf("See log path for results: %s%s", copyDirPath, de->d_name);
				getchar();
				return 0;
			}
			
			//simple check
			if(!f)
			{
				printf("File cannot be open!");
				getchar();
				return 0;
			}
			if (!v)
			{
				printf("Virus File cannot be open!");
				getchar();
				return 0;
			}

			//size of File	
			fseek(f, 0, SEEK_END);
			sizeF = ftell(f);
			rewind(f);
			//printf("------%d--------\n", size);

			//size of Virus
			fseek(v, 0, SEEK_END);
			sizeV = ftell(v);
			rewind(v);
			//printf("-------%d-------\n", size2);

			//THIS IS ALL A PART OF THE MEMMEM THING
			n = fread(virusInside, 1, sizeV, v);

			char *fileData = malloc(sizeF);

			// Read data of file.
			size_t nread = fread(fileData, 1, sizeF, f);

			char *ret = memmem(fileData, nread, virusInside, n);
			if (ret != NULL)
			{
				virus = 1;
			}
			else
			{
				virus = 0;
			}

			//checks what mode are you on and if its infected or not
			if (virus && choose != 0)
			{
				printf("%s%s- %s ", copyDirPath, de->d_name, "Infected!");
				fprintf(log, "%s%s	%s ", copyDirPath, de->d_name, "Infected!");

				if (twentycheck == 1)
				{
					printf("<First 20%c> \n", '%');
					fprintf(log, "<First 20%c> \n", '%');
				}
				else if(twentycheck == 2)
				{
					printf("<Last 20%c> \n", '%');
					fprintf(log, "<Last 20%c> \n", '%');
				}
				
			}
			else if(virus && choose == 0)
			{
				printf("%s%s- %s \n", copyDirPath, de->d_name, "Infected!");
				fprintf(log, "%s%s	%s \n", copyDirPath, de->d_name, "Infected!");
			}
			else
			{
				printf("%s%s- %s \n", copyDirPath, de->d_name, "Clean");
				fprintf(log, "%s%s	%s \n", copyDirPath, de->d_name, "Clean");
			}
			fclose(f);
			fclose(v);
			fclose(log);
		}
		else
		{
			
		}

	}

	closedir(dr);
	

	getchar();
	return 0;
}
//GOOGLE!!!!!!!!!!!!!!!!!!!!!!!!!!! HELPED ME HERE
/*
 * The memmem() function finds the start of the first occurrence of the
 * substring 'needle' of length 'nlen' in the memory area 'haystack' of
 * length 'hlen'.
 *
 * The return value is a pointer to the beginning of the sub-string, or
 * NULL if the substring is not found.
 */
int memmem(char *haystack, size_t hlen, char *needle, size_t nlen)
{
	int needle_first;
	char *p = haystack;
	size_t plen = hlen;
	int firstwenty = 0;
	int lastwenty = 0;
	int a = 0;

	firstwenty = 20 * (hlen/100);
	lastwenty = hlen - (firstwenty * 4);

	if (!nlen)
		return NULL;

	needle_first = *(unsigned char *)needle;
	//printf("  %p   ", needle_first);
	while (plen >= nlen && (p = memchr(p, needle_first, plen - nlen + 1)))
	{
		//a = memcmp(p, needle, nlen);
		//printf("   %d   ", a);
		//printf("%p ", p);
		if (!memcmp(p, needle, nlen))
		{
			//a = memcmp(p, needle, nlen);
			//printf("   %d   ", a);
			//printf("ZZZZZZZZ ");
			if (plen <= firstwenty)
			{
				twentycheck = 1;
			}
			else if(plen >= lastwenty)
			{
				twentycheck = 2;
			}
			return (void *)p;
		}
			

		p++;
		plen = hlen - (p - haystack);
		//printf("    %d    ", plen);
	}

	return NULL;
}
/*
	This Func prints the menu!

	input: argv
	output: none
*/
void menu(char** argv)
{


	printf("Welcome to my Virus Scan!\n\n");

	printf("====================================\n");
	printf("Folder to scan: %s\n", argv[1]);

	printf("Virus signature: %s\n", argv[2]);

	printf("====================================\n\n");

	printf("Press 0 for Normal Scan or any key for a Quick Scan: ");

}
/*
	This Func prints the menu inside the log

	input: argv, the log, what you chose
	output: none
*/
void logMenu(char** argv, FILE* log, int choose)
{

	fprintf(log, "%s\n\n", "Anti-virus began! Welcome!");

	fprintf(log, "%s\n%s\n", "Folder to scan:", argv[1]);

	fprintf(log, "%s\n%s\n\n", "Virus signature:", argv[2]);

	fprintf(log, "%s\n", "Scanning option:");

	if (choose != 0)
	{
		fprintf(log, "%s\n\n", "Quick Scan");
	}
	else if (choose == 0)
	{
		fprintf(log, "%s\n\n", "Normal Scan");
	}

	fprintf(log, "%s\n", "Results:");
}


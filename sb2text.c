#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char* replaceExtension(char *path, char *newExt)
{
	int nameLength = strlen(path);
	int oldExtLength = 0;
	int newExtLength = strlen(newExt);

	while (nameLength > 0)
	{
		nameLength--;
		oldExtLength++;

		if (path[nameLength] == '.')
			break;
	}

	int newPathLength = nameLength + newExtLength;
	char *newPath = malloc(newPathLength + 1);

	memcpy(newPath, path, nameLength);
	memcpy(newPath + nameLength, newExt, newExtLength);

	newPath[newPathLength] = '\0';

	return newPath;
}

int main(int argc, char **argv)
{
	puts("sb2text - .sb2 to .txt converter for SBSP: Employee of the Month");
	puts("By Seil\n");

	if (argc != 2 && argc != 3)
	{
		puts("Usage: sb2make <input sb2 file>");
		puts("       sb2make <input sb2 file> <output text file>\n");
		puts("Or simply drag a .sb2 file onto this executable.");
		return 1;
	}

	FILE *input, *output;
	char *inputPath, *outputPath;
	short lineCount;

	// Open input and output files
	inputPath = argv[1];
	input = fopen(inputPath, "rb");

	if (input == NULL)
		goto error;

	outputPath = (argc == 3) ? argv[2] : replaceExtension(inputPath, ".txt");
	output = fopen(outputPath, "w");

	if (output == NULL)
		goto error;

	// Read line count
	fread(&lineCount, sizeof(short), 1, input);

	for (short i = 0; i < lineCount; i++)
	{
		short lineLength;
		char line[512];

		// Read line data
		fread(&lineLength, sizeof(short), 1, input);
		fread(&line, lineLength, 1, input);

		// De-obfuscate data
		for (short j = 0; j < lineLength; j++)
			line[j] ^= 0x30;

		// Write line
		fwrite(line, lineLength, 1, output);
	}

	fclose(input);

	printf("Converted successfully to %s\n", outputPath);
	return 0;

error:
	perror("Error");
	return errno;
}
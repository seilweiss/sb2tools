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
	puts("sb2make - .txt to .sb2 converter for SBSP: Employee of the Month");
	puts("By Seil\n");

	if (argc != 2 && argc != 3)
	{
		puts("Usage: sb2make <input text file>");
		puts("       sb2make <input text file> <output sb2 file>\n");
		puts("Or simply drag a text file onto this executable.");
		return 1;
	}

	FILE *input, *output;
	char *inputPath, *outputPath;
	size_t size, bytesRead;
	char *data;
	short lineCount = 0;

	// Read input file
	inputPath = argv[1];
	input = fopen(inputPath, "r");

	if (input == NULL)
		goto error;

	fseek(input, 0, SEEK_END);
	size = ftell(input);
	fseek(input, 0, SEEK_SET);

	data = malloc(size);
	memset(data, 0, size);

	bytesRead = fread(data, 1, size, input);

	if (bytesRead != size && ferror(input) != 0)
		goto error;

	// bytesRead != size is not necessarily an error, which is why
	// we check for an explicit file read error.
	// bytesRead may be less than size if the file contains CRLF pairs,
	// which Windows automatically converts to single LF characters
	// during fread (thus shortening the length of the data in memory).

	size = bytesRead;
	fclose(input);

	// Count lines
	for (size_t i = 0; i < size; i++)
	{
		if (data[i] == '\n' || i == size - 1)
			lineCount++;
	}

	// Write output file
	outputPath = (argc == 3) ? argv[2] : replaceExtension(inputPath, ".sb2");
	output = fopen(outputPath, "wb");

	if (output == NULL)
		goto error;

	// Write line count
	fwrite(&lineCount, sizeof(short), 1, output);

	size_t lineStart = 0;

	for (size_t i = 0; i < size; i++)
	{
		char ch = data[i];

		// "Obfuscate" data
		// They'll never figure this out!
		data[i] ^= 0x30;

		if (ch == '\n' || i == size - 1)
		{
			// Write line length
			short lineLength = (short)i + 1 - (short)lineStart;
			fwrite(&lineLength, sizeof(short), 1, output);

			// Write line data (including newline)
			fwrite(data + lineStart, lineLength, 1, output);

			lineStart = i + 1;
		}
	}

	fclose(output);

	printf("Converted successfully to %s\n", outputPath);
	return 0;

error:
	perror("Error");
	return errno;
}
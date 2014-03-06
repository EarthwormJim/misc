#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define TEE_BUFF_SIZE 4096
#define IS_BINARY 10


void cat_file(char *filename) {
	unsigned char buff[TEE_BUFF_SIZE];	// Buffer for file I/O.
	size_t numread,numwrote = 0;		// Keeps track of total bytes to write.
	int count = 0, i;			// Used for determining if the file is binary.
	FILE *input = fopen(filename,"r");	// File pointer pointing to the file to print.

	printf("%s:\n\n",filename);	// Print filename before outputting contents.

	// Check to see if file is binary by reading the first 4096 bytes
	// and determining if 10 or more fall out of the 0 - 127 ASCII range.
	// If it is, return to caller.
	numread = fread(buff, sizeof(unsigned char),  TEE_BUFF_SIZE, input);
	for (i = 0; i < numread; i++){
		if (!isascii((int)buff[i])) count = count + 1;
	}

	printf("============================================================\n");
	printf("********************** %s ****************************\n", filename);
	printf("============================================================\n\n");
	
	if (count >= IS_BINARY){// Skip file if IS_BINARY or more bytes are not "ASCII".
		printf("%s is a binary file and will not be printed.\n", filename);
		return;
	} else {		// Otherwise, the bytes stored in buff[] are printed to stdout.
		numwrote = fwrite(buff, sizeof(unsigned char), numread, stdout);
	}
	while (numread > 0) { 	// While there are bytes left to read, run loop.
		numread = fread(buff, sizeof(unsigned char),  TEE_BUFF_SIZE, input);
		numwrote = fwrite(buff, sizeof(unsigned char), numread, stdout);
	}

	fclose(input);		// Close file stream when finished.

}

void lsd(char *path) {
	DIR *dirp = NULL;		// Pointer to the directory.
	struct dirent *dent = NULL;	// Used for storing directory information.
	struct stat file;		// Used for storing file information.

	dirp = opendir(path);		// This is where the directory at argv[1] is opened.
	while(1) {			// Run loop until all directory entries have been read. 
		dent = readdir(dirp);	// Assign directory entry to dent.
		if(dent == NULL)
			break;		// Break if no entries left.
		if(dent->d_name[0] != '.') {
			stat(dent->d_name, &file);
			if (S_ISREG(file.st_mode)) cat_file(dent->d_name);
		}			// If directory entry is a regular file, send it to cat_file().
	}

	closedir(dirp);
}

int main(int argc, char **argv) {
	struct stat st;
		
	if (argc == 2) {		// Any call to dircat without precisely two
		stat(argv[1], &st);	// arguments is automatically invalid.
		if (S_ISDIR(st.st_mode))// If the mode is a directory, call lsd();
			lsd(argv[1]);
		else			// Otherwise, print "help" message.
			printf("\tArgument is not a directory, or does not exist. \n\tCorrect usage: dircat <directory_name>\n");
	} else {			// Print "help" message if fewer/more than 2 arguments.
		printf("\tIncorrect number of arguments.\n\tCorrect usage: dircat <directory_name>\n");
	}

  return 0;
}


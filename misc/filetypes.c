#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

/**
 * TODO: what happens if you `fstat` a symlink?
 */
int main( int argc, char *argv[] )
{
	int rtn = 0;
	struct stat rStatInfo;
	// Could define a buffer with PATH_MAX size and strncpy to copy ovre
	char *pFilePath = NULL;
	const char *pFileType;
	for(int i = 1; i < argc; i++) {
		pFilePath = argv[i];
		if(lstat(pFilePath, &rStatInfo) < 0) {
			fprintf(stderr, "%s (error %d) during lstat on %s\n", strerror(errno), errno, pFilePath);
			rtn = 1;
			break;
		}
		if(S_ISREG(rStatInfo.st_mode)) { pFileType = "regular"; }
		if(S_ISDIR(rStatInfo.st_mode)) { pFileType = "directory"; }
		if(S_ISCHR(rStatInfo.st_mode)) { pFileType = "character special"; }
		if(S_ISBLK(rStatInfo.st_mode)) { pFileType = "block"; }
		if(S_ISFIFO(rStatInfo.st_mode)) { pFileType = "FIFO/pipe"; }
		if(S_ISLNK(rStatInfo.st_mode)) { pFileType = "symlink"; }
		if(S_ISSOCK(rStatInfo.st_mode)) { pFileType = "socket"; }
		else { pFileType = "UNK"; }
		
		printf("%s: %s file, mode 0%o, last modified: %s\n", pFilePath, pFileType, rStatInfo.st_mode, ctime(&rStatInfo.st_ctime));
	}
	return rtn;
}

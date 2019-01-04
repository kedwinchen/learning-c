#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

void usage(const char *prg)
{
	fprintf(stderr, "Usage: %s <-s size> <-f filename>\n", prg);
}

int main(int argc, char *argv[])
{
	// Size of the buffer to copy
	const int nBUFSIZE = 1024;
	// Maximum size of the file
	int nMAXSIZE;
	struct stat rfstat;
	int rtn = 0;
	int nfd = -1;
	// off_t instead of int
	off_t nwroff = -1;
	off_t nrdoff = -1;
	int nbytesread = -1;
	char *pfpath = NULL;
	char *pafdata[nBUFSIZE];
	int opt;
	// get cli args for size, files; iterate through files
	while( (opt = getopt(argc, argv, "f:s:")) != -1) {
		switch(opt) {
		case 'f':
			pfpath = optarg;
			printf("selected file is: %s\n", pfpath);
			break;
		case 's':
			nMAXSIZE = strtol(optarg, '\0', 10);
			//			nMAXSIZE = atoi(optarg); // atoi does not do error c
			printf("selected file size is: %d\n", nMAXSIZE);
			break;
		default:
			usage(argv[0]);
			exit(1);
		}
	}
	// check to ensure cli args are set
	if( pfpath == NULL ) {
		usage(argv[0]);
		exit(1);
	}
	if(nMAXSIZE < 0) {
		fprintf(stderr, "Argument to -s must be an integer larger than 0!\n");
		usage(argv[0]);
		exit(1);
	}
	
	// ck fsize, if need to do anything
	if(stat(pfpath, &rfstat) < 0) {
		fprintf(stderr, "cannot stat file: %s\n", pfpath);
		//		break;
		rtn = 1;
	}
	else {
		if(rfstat.st_size <= nMAXSIZE) {
			// no need to continue		
			//		break;
			printf("file is of size %ld, max size is %d; ", rfstat.st_size, nMAXSIZE);
			printf("nothing to do!\n");
			rtn = 0;
		}
		else {
			// if yes, open file
			if ( (nfd = open(pfpath, O_RDWR)) < 0 ) {
				fprintf(stderr, "cannot open file %s for read/write\n", pfpath);
				//		break;
				rtn = 1;
			}
			// set initial offsets for reading, writing
			// lseek to size bytes before end
			nwroff = lseek(nfd, 0, SEEK_SET); // probably could just set `nwroff` to `(int) 0`
			nrdoff = lseek(nfd, -nMAXSIZE, SEEK_END);
			if(nrdoff < 0) {
				fprintf(stderr, "failed during lseek while reading %s\n", pfpath);
			}

			// loop the following...
			do {
				// read bufsize bytes into buf, save new lseek loc offset for read
				nbytesread = read(nfd, &pafdata, nBUFSIZE);
				nrdoff += nbytesread;
				// write bufsize bytes to beginning of file, save lseek offset for write
				// set lseek to location to start writing from
				if (lseek(nfd, nwroff, SEEK_SET) < 0) {
					fprintf(stderr, "error during lseek to write location in file %s\n", pfpath);
					break;
				}
				// write the data
				if( nbytesread != write(nfd, &pafdata, nbytesread) ) {
					fprintf(stderr, "could not write all of the data read in file %s\n", pfpath);
					break;
				}
				// set the new write offset for lseek
				nwroff += nbytesread;
				// lseek back to the location to read from
				if( lseek(nfd, nrdoff, SEEK_SET) < 0 ) {
					fprintf(stderr, "failed during lseek while preparing to read %s\n", pfpath);
					break;
				}
				// ... until read bytes is less than buffsize
			} while (!(nbytesread < nBUFSIZE));

			// then, truncate the file to max size
			if( ftruncate(nfd, nMAXSIZE) < 0 ) {
				fprintf(stderr, "failed to truncate file to size %d\n", nMAXSIZE);
				rtn = 1;
			}
			// then close the file
			if( close(nfd) < 0 ) {
				fprintf(stderr, "error %u (%s) while closing file %s\n", errno, strerror(errno), pfpath);
				rtn = 1;
			}
		}
	}
	// return
	return rtn;
}

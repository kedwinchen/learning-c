#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	struct stat ssb;

	if ( stat(argv[1], &ssb) < 0 )
    {
        printf("Error %u", errno);
        perror("in the `struct` function call");
        return 1;
    }
    else
    {
        printf("sizeof(struct stat)=%d", sizeof(ssb));
    }
    return 0;
}

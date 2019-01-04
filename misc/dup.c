#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int output(int fdes, const char *ss)
{
    int n = write( fdes, ss, sizeof(ss));
    if( n == -1 )
    {
        printf("write error");
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    const char *buf = "thing0\n";
    int fd = open( argv[1], O_RDWR | O_CREAT | O_TRUNC );

    output(STDOUT_FILENO, buf);
    output(STDERR_FILENO, buf);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    printf("%s", buf);
    printf("some more text");
}

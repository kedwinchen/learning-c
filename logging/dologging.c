#include <stdio.h>
#include <string.h>
#include "logger.h"


void f1()
{
    fprintf(stdout, "f1: stdout\n");
}

void f2()
{
    fprintf(stderr, "f2: stderr\n");
}

int main(void)
{
    printf("start execution...\n");
    printf("start time: %s\n", now());
    int fd = logInfo("/tmp/test", true, true);
    dprintf(fd, "Start logging...\n");
    f1();
    f2();
    printf("%s\n", (logItem("last log entry\n")) ? "success" : "failure");
    printf("end time: %s\n", now());
    return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	pid_t p = fork();

	if(p > 0) { // parent
		sleep(2);
	}
	/*
	 * -1: error
	 * > 0 : parent
	 * = 0 : is child
	 */
	printf("PID: %6d, getpid() = %6d, getppid() = %6d\n", p, getpid(), getppid());
	
	return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void daemon_skel(void)
{
	pid_t pid;

	pid = fork();

	if(pid < 0) { // failure state
		exit(-1);
	}
	else if(pid > 0) { // the (first) parent, the one that is being run from the shell or wherever
		exit(0); // exit, so the parent's parent can handle the process
	}

    /******* CHILD SECTION *******/ 

	if(setsid() < 0) {
		exit(-1);
	}

	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	umask(0);
	chdir("/tmp");

	for(int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
		close(x);
	}

}


int main(void)
{
	daemon_skel();
	
	FILE* fp = fopen("daemon_skel.log", "a+");
	if(fp == NULL) { return -1; }
	for( int i = 0; i < 10; i++) {
		fprintf(fp, "logging, %d...\n", i);
		sleep(5);
	}
	fprintf(fp, "%d DONE!!!", getpid());
	fclose(fp);
	return 0;
}

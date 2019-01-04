#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
// #include "logger.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

char* now(void)
{

    time_t rawtime;
    time(&rawtime);
    static char ps_time[256];
    struct tm *pr_timeinfo = localtime(&rawtime);
    strftime(ps_time, sizeof(ps_time), "(%F %T [%Z]) ", pr_timeinfo);

    /*
    char *rtn_str = (char *)malloc(sizeof(ps_time));
    strncpy(rtn_str, ps_time, sizeof(rtn_str));
    return rtn_str;
    */

    return (char *)&ps_time;
}

struct logFileInfo
{
    bool inclStdOut;
    bool inclStdErr;
    int fd;
    char path[256];
    //} logFile;
};

/**
 *  @return (on success): `fd`, the file descriptor
 *          (on failure): -1
 *
 * algorithm;
 *  - If no fd set, means not initialized. So create the file
 *      - Copy appropriate values
 *          - Ensure fpath fits in the field in struct
 *      - Open file
 *      - Set fd
 *      - Make redirections as needed
 *  - If fd set, return it
 */
int logInfo( const char *fpath, bool inclStdOut, bool inclStdErr )
{
    // static int fd = -1;
    // if (fpath == NULL && logFile.fd != NULL) { return logFile.fd; }
    // if (fd == -1)
    static struct logFileInfo logFile;
    if( &logFile == NULL || fpath != NULL )
        {
            if ( strlen(fpath) > sizeof(logFile.path) )
                {
                    fprintf(stderr, "redir.c:logfd: Provided path name (%s) of length (%lu) exceeds the maximum allowed length of %lu\n",
                            fpath, sizeof(fpath) - 1, sizeof(logFile.path) - 1);
                    return -1;
                }
            strncpy( logFile.path, fpath, sizeof(logFile.path) );
            logFile.inclStdOut = inclStdOut;
            logFile.inclStdErr = inclStdErr;
            int fd = open( fpath, O_CREAT | O_WRONLY | O_APPEND , 0664);
            if (fd < 0)
                {
                    fprintf(stderr,"redir.c:logfd: Error %d when opening the log file for writing...\n", errno);
                    perror("redir.c:logfd");
                    return -1;
                }
            logFile.fd = fd;
            if (inclStdOut) { dup2(fd, STDOUT_FILENO); }
            if (inclStdErr) { dup2(fd, STDERR_FILENO); }
        }
    return logFile.fd;
}

/**
 *  @description: Get the file descriptor for the log file
 *
 *  @return (on success): `(int) fd`, the file descriptor for the log file at `(char *) logFile.path`
 *          (on failure): -1
 */
static int logfd( const char *fpath, bool inclStdOut, bool inclStdErr )
{
    int fd = logInfo(fpath, inclStdOut, inclStdErr);
    if (fd < 0) { return -1; }
    return fd;
}

/**
 *  @description:
 *
 *  @return (on success):   true
 *          (on failure):   false
 */
bool logItem( const char *toLog )
{
    int fd = logfd(NULL, false, false);
    int len = sizeof(toLog);
    if ( toLog == NULL )
        {
            int n = close(fd);
            if (n != 0) { return false; }
            return true;
        }
    else
        {
            char *pc_time = now();
            int n_lenTime = strlen(pc_time) + 1; // include null terminator
            int n_lenLine = strlen(toLog) + 1;
            char logLine[( n_lenTime + n_lenLine)];

            n_lenLine = strlen(strncat(strncat(logLine, pc_time, n_lenTime), toLog, n_lenLine)) + 1;
            //          if (write(fd, strncat(logLine, toLog, sizeof(toLog)), len) != len) { return false; }
            if(write(fd, &logLine, n_lenLine) != n_lenLine ) { return false; }
        }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/**
 *  @description:
 *
 *  @return (on success):
 *          (on failure):
 */

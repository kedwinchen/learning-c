#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string.h>
#include <stdbool.h>

extern bool logItem(const char *);
extern int logInfo(const char *, bool, bool);
extern char * now(void);

#endif /* __LOGGER_H__ */

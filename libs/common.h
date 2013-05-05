/*
 *
 *
 *
 *
 */

#ifndef __COMMON_H__
#define __COMMON_H__


#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 512

void Write(int iFD, char* buf, ssize_t size);
void ReadLine(int iFD, void* line_buf, size_t size);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>


#include "Filer.h"

#define AF_INET PF_INET

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


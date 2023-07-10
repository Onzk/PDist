#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <netdb.h>
#include <pthread.h>
#define AF_INET PF_INET

#include "Filer.h"

struct Item *thermometres[MAX_THERMOMETRE], *chauffages[MAX_CHAUFFAGE];

int opt = TRUE, portno, n;

// Thermometre objects
char t_buffer[BUFFER_SIZE];
int t_max_sd;
int t_sockfd, t_newsockfd;
int t_master_socket, t_addrlen, t_new_socket, t_activity, t_valread;
struct sockaddr_in t_serv_addr, t_cli_addr;
struct sockaddr_in t_address;
socklen_t t_clilen;
fd_set t_readfds;

// Chauffage objects
char c_buffer[BUFFER_SIZE];
int c_max_sd;
int c_sockfd, c_newsockfd;
int c_master_socket, c_addrlen, c_new_socket, c_activity, c_valread;
struct sockaddr_in c_serv_addr, c_cli_addr;
struct sockaddr_in c_address;
socklen_t c_clilen;
fd_set c_readfds;

// Gestion console objects
int gc_socket;

int cmd_c_sockfd;
struct sockaddr_in cmd_c_servaddr, cmd_c_cliaddr;

int ct_socket;
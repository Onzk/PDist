#include "../../pack/c/Console.h"

int sockfd, portno, n;
char buffer[BUFFER_SIZE];
struct sockaddr_in serv_addr;
struct hostent *server;
int main()
{
    portno = CONTROL_CONSOLE_PORT;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("[CONTROLE_CONSOLE - SOCKET_ERROR] : Impossible de se connecter au module [GESTION_CONSOLE]\n");
    server = gethostbyname("localhost");
    if (server == NULL)
    {
        fprintf(stderr, "[CONTROLE_CONSOLE - SERVER_ERROR] : Hote non trouve...\n");
        exit(1);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("[CONTROLE_CONSOLE - CONNECTION_ERROR] : Impossible de se connecter au module [GESTION_CONSOLE]\n");
    printf("[CONTROLE_CONSOLE - INFO] : Connection etablie avec le module [GESTION_CONSOLE]\n");
    while (1)
    {
        bzero(buffer, BUFFER_SIZE);
        n = read(sockfd, buffer, BUFFER_SIZE);
        if (n < 0)
            error("[CONTROLE_CONSOLE - READING_ERROR] : Impossible de lire les informations du module [GESTION_CONSOLE]\n");
        if (strlen(buffer) > 0)
        {
            printf("-> %s\n", buffer);
        }
        else
        {
            break;
        }
    }
    close(sockfd);
    return 0;
}
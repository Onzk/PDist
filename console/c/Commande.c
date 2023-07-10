#include "../../pack/c/Console.h"

int main()
{
    int sockfd, portno, n;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    portno = COMMAND_CONSOLE_PORT;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("[COMMANDE_CONSOLE - SOCKET_ERROR] : Impossible de se connecter au module [GESTION_CONSOLE]\n");
    server = gethostbyname("localhost");
    if (server == NULL)
    {
        fprintf(stderr, "[COMMANDE_CONSOLE - SERVER_ERROR] : Hote non trouve...\n");
        exit(1);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("[COMMANDE_CONSOLE - CONNECTION_ERROR] : Impossible de se connecter au module [GESTION_CONSOLE]\n\n");
    printf("[COMMANDE_CONSOLE - INFO] : Connection etablie avec le module [GESTION_CONSOLE]\n");
    printf("[COMMANDE_CONSOLE - INFO] : Pour modifier la temperature dans une piece, tapez dans l'ordre ces valeurs separees par un espace : \n");
    printf("[COMMANDE_CONSOLE - INFO] : (1) => L'adresse du groupe multicast de la piece (Ex : localhost) \n");
    printf("[COMMANDE_CONSOLE - INFO] : (2) => Mode de chauffage (A pour automatique / M pour manuel)\n");
    printf("[COMMANDE_CONSOLE - INFO] : (3) => Niveau de chauffage (De 0 a 5)\n");
    struct Information *info = malloc(sizeof(struct Information *));
    int level;
    int oneAttempt = FALSE;
    char mode;
    char mutlicast[MAX_MESSAGE_LENGTH];
    int one, two, three, four;
    while (1)
    {
        do
        {
            printf("> ");
            scanf("%d.%d.%d.%d %c %d", &one, &two, &three, &four, &mode, &level);
            snprintf(mutlicast, sizeof(char[MAX_MESSAGE_LENGTH]), "%d.%d.%d.%d", one, two, three, four);
        } while (level < 0 || level > 5);
        info->multicast = mutlicast;
        info->valeur = level;
        info->type = mode;
        char *message = p_info_parse(info);
        n = write(sockfd, message, strlen(message));
        if (n < 0)
            error("[COMMANDE_CONSOLE - WRITING_ERROR] : Erreur lors de l'envoie de la requete au module [GESTION_CONSOLE]\n");
        printf("[COMMANDE_CONSOLE - INFO] : Commande envoyee.\n");
    }
    free(info);
    close(sockfd);
    return 0;
}
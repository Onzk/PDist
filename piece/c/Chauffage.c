#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define AF_INET PF_INET
#include <json-c/json.h>
#include "../../pack/c/Parser.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    //--- Tentative de connection à la piece ---//
    printf("[CHAUFFAGE - INFO] : Connection au multicast de la piece en cours...\n");
    int pieceSockfd, piecePortno, pieceN;
    char pieceBuffer[BUFFER_SIZE];
    struct sockaddr_in pieceServ_addr;
    struct hostent *pieceSocket;

    if ((pieceSockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&pieceServ_addr, 0, sizeof(pieceServ_addr));

    pieceServ_addr.sin_family = AF_INET;
    pieceServ_addr.sin_port = htons(MULTICAST_PORT);
    pieceServ_addr.sin_addr.s_addr = INADDR_ANY;
    int n;
    socklen_t len;

    printf("[CHAUFFAGE - INFO] : Connection au multicast de la piece reussie...\n");

    printf("[CHAUFFAGE - INFO] : Connection au systeme de gestion de temperature cours...\n");
    int systemeSockfd, systemePortno, systemeN;
    struct sockaddr_in systemeServ_addr;
    struct hostent *systemeSocket;

    systemePortno = CHAUFFAGE_TO_SYSTEM_PORT;
    systemeSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (systemeSockfd < 0)
        error("[CHAUFFAGE - ERROR] : Impossible de se connecter au systeme de gestion de temperature.\n");
    systemeSocket = gethostbyname("localhost");
    if (systemeSocket == NULL)
    {
        fprintf(stderr, "[CHAUFFAGE - ERROR] : L'adresse ip du systeme de gestion de temperature n'est pas valide.\n");
        exit(1);
    }

    bzero((char *)&systemeServ_addr, sizeof(systemeServ_addr));
    systemeServ_addr.sin_family = AF_INET;
    bcopy((char *)systemeSocket->h_addr, (char *)&systemeServ_addr.sin_addr.s_addr, systemeSocket->h_length);
    systemeServ_addr.sin_port = htons(systemePortno);

    if (connect(systemeSockfd, (struct sockaddr *)&systemeServ_addr, sizeof(systemeServ_addr)) < 0)
        error("[CHAUFFAGE - CONNECT_ERROR] : Impossible de se connecter au systeme de gestion de temperature.\n");

    printf("[CHAUFFAGE - INFO] : Connection au systeme de gestion de temperature de la piece reussie...\n");

    while (systemeSockfd >= 0)
    {
        char systemeBuffer[BUFFER_SIZE];
        bzero(systemeBuffer, BUFFER_SIZE);
        systemeN = read(systemeSockfd, systemeBuffer, BUFFER_SIZE);
        if (systemeN < 0)
            error("[CHAUFFAGE - ERROR] : Erreur lors de la lecture du message du module [COMMUNICATION_TEMPERATURE].\n");
        if (strlen(systemeBuffer) > 0)
        {
            pieceN = sendto(pieceSockfd, (const char *)systemeBuffer, BUFFER_SIZE,
                            MSG_CONFIRM, (const struct sockaddr *)&pieceServ_addr,
                            sizeof(pieceServ_addr));
            if (pieceN < 0)
                error("[CHAUFFAGE - ERROR] : Erreur lors de l'envoie du message au module [PIECE].\n");
        }
        else
        {
            break;
        }
    }
    close(pieceSockfd);
    close(systemeSockfd);
    return 0;
}

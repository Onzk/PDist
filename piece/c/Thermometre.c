#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <arpa/inet.h>
#define AF_INET PF_INET

#include "../../pack/c/Constants.h"

int pieceSockfd, piecePortno, pieceN;
char pieceBuffer[BUFFER_SIZE];
struct sockaddr_in pieceServ_addr;
struct hostent *pieceSocket;

int systemeSockfd, systemePortno, systemeN;
char systemeBuffer[BUFFER_SIZE];
struct sockaddr_in systemeServ_addr;
struct hostent *systemeSocket;

socklen_t len;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *sendToPiece()
{
    while (1)
    {
        sleep(5);
        sendto(pieceSockfd, (const char *)systemeBuffer, strlen(systemeBuffer),
               MSG_CONFIRM, (const struct sockaddr *)&pieceServ_addr,
               sizeof(pieceServ_addr));
    }
    return NULL;
}

void *handleMessageFromRoom()
{
    pthread_t stp_id;
    pthread_create(&stp_id, NULL, sendToPiece, NULL);
    while (1)
    {
        pieceN = recvfrom(pieceSockfd, (char *)systemeBuffer, BUFFER_SIZE,
                          MSG_WAITALL, (struct sockaddr *)&pieceServ_addr,
                          &len);
        systemeBuffer[pieceN] = '\0';

        if (pieceN < 0)
            error("[THERMOMETRE - ERROR] : Erreur lors de la reception du message de la piece.");

        systemeN = write(systemeSockfd, systemeBuffer, BUFFER_SIZE);
        if (systemeN < 0)
            error("[THERMOMETRE - ERROR] : Erreur lors de l'envoie du message au systeme.");
    }
    pthread_cancel(stp_id);
    return NULL;
}

int main()
{

    printf("[THERMOMETRE - INFO] : Connection au multicast de la piece en cours...\n");

    if ((pieceSockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        error("[THERMOMETRE - ERROR] : Impossible de se connecter au multicast de la piece.\n");
    }

    memset(&pieceServ_addr, 0, sizeof(pieceServ_addr));

    pieceServ_addr.sin_family = AF_INET;
    pieceServ_addr.sin_port = htons(MULTICAST_PORT);
    pieceServ_addr.sin_addr.s_addr = inet_addr(MULTICAST_HOST);

    printf("[THERMOMETRE - INFO] : Connection au multicast de la piece reussie...\n");

    printf("[THERMOMETRE - INFO] : Connection au systeme de gestion de temperature cours...\n");

    systemePortno = THERMOMETRE_TO_SYSTEM_PORT;
    systemeSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (systemeSockfd < 0)
        error("[THERMOMETRE - ERROR] : Impossible de se connecter au systeme de gestion de temperature.\n");
    systemeSocket = gethostbyname("localhost");
    if (systemeSocket == NULL)
    {

        fprintf(stderr, "[THERMOMETRE - ERROR] : L'adresse ip du systeme de gestion de temperature n'est pas valide.\n");
        exit(1);
    }
    bzero((char *)&systemeServ_addr, sizeof(systemeServ_addr));
    systemeServ_addr.sin_family = AF_INET;
    bcopy((char *)systemeSocket->h_addr, (char *)&systemeServ_addr.sin_addr.s_addr, systemeSocket->h_length);
    systemeServ_addr.sin_port = htons(systemePortno);

    if (connect(systemeSockfd, (struct sockaddr *)&systemeServ_addr, sizeof(systemeServ_addr)) < 0)
        error("[THERMOMETRE - ERROR] : Impossible de se connecter au systeme de gestion de temperature.\n");

    printf("[THERMOMETRE - INFO] : Connection au systeme de gestion de temperature de la piece reussie...\n");

    pthread_t pieceId;
    pthread_create(&pieceId, NULL, handleMessageFromRoom, NULL);
    pthread_join(pieceId, NULL);
    close(pieceSockfd);
    close(systemeSockfd);
    return 0;
}

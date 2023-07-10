#include <pthread.h>
#include "Sys_objects.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *handle_t_send_message(struct Information *info)
{
    for (int i = 0; i < MAX_THERMOMETRE; i++)
    {
        if (thermometres[i] != NULL && thermometres[i]->socket)
        {
            // if (info->multicast == thermometres[i]->multicast)
            // {
            info->multicast = thermometres[i]->multicast;
            char *message = p_info_parse(info);
            int n = send(thermometres[i]->socket, message, sizeof(message), 0);
            if (n < 0)
            {
                printf("[COMMUNICATION_TEMPERATURE - SEND_ERROR] : Impossible de communiquer avec le [THERMOMETRE] de la [PIECE] : %s - %s.\n",
                       thermometres[i]->multicast, thermometres[i]->piece);
            }
            break;
            // }
        }
    }
    return NULL;
}

void *handle_t_connection()
{

    char *message = "GESTION DE TEMPERATURE v1.0 \r\n";
    int i, sd;
    for (i = 0; i < MAX_THERMOMETRE; i++)
    {
        thermometres[i] = NULL;
    }

    if ((t_master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("[COMMUNICATION_TEMPERATURE - MASTERSOCKET_ERROR] : Lancement du serveur echoue pour les modules [THERMOMETRE].\n");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(t_master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0)
    {
        perror("[COMMUNICATION_TEMPERATURE - SETSOCKPORT_ERROR] : Lancement du serveur echoue pour les modules [THERMOMETRE].\n");
        exit(EXIT_FAILURE);
    }

    t_address.sin_family = AF_INET;
    t_address.sin_addr.s_addr = INADDR_ANY;
    t_address.sin_port = htons(THERMOMETRE_TO_SYSTEM_PORT);

    if (bind(t_master_socket, (struct sockaddr *)&t_address, sizeof(t_address)) < 0)
    {
        perror("[COMMUNICATION_TEMPERATURE - BINDING_ERROR] : Lancement du serveur echoue pour les modules [THERMOMETRE].");
        exit(EXIT_FAILURE);
    }
    printf("[COMMUNICATION_TEMPERATURE - INFO] : Lancement du serveur pour les modules [THERMOMETRE] sur le port # %d.\n", THERMOMETRE_TO_SYSTEM_PORT);

    if (listen(t_master_socket, MAX_THERMOMETRE) < 0)
    {
        perror("[COMMUNICATION_TEMPERATURE - LISTEN_ERROR] : Ecoute du serveur echoue pour les modules [THERMOMETRE].");
        exit(EXIT_FAILURE);
    }

    t_addrlen = sizeof(t_address);
    puts("[COMMUNICATION_TEMPERATURE - INFO] : Attente de connection pour des modules [THERMOMETRE]...\n");

    while (TRUE)
    {

        FD_ZERO(&t_readfds);

        FD_SET(t_master_socket, &t_readfds);
        t_max_sd = t_master_socket;

        for (i = 0; i < MAX_THERMOMETRE; i++)
        {
            if (thermometres[i])
            {
                sd = thermometres[i]->socket;

                if (sd > 0)
                    FD_SET(sd, &t_readfds);

                if (sd > t_max_sd)
                    t_max_sd = sd;
            }
        }

        t_activity = select(t_max_sd + 1, &t_readfds, NULL, NULL, NULL);

        if ((t_activity < 0) && (errno != EINTR))
        {
            printf("[COMMUNICATION_TEMPERATURE - T_ACTIVITY_ERROR] : Capture d'activite impossible pour les modules [THERMOMETRE].\n");
        }

        if (FD_ISSET(t_master_socket, &t_readfds))
        {
            if ((t_new_socket = accept(t_master_socket,
                                       (struct sockaddr *)&t_address, (socklen_t *)&t_addrlen)) < 0)
            {
                perror("[COMMUNICATION_TEMPERATURE - ACCEPT_ERROR] : Nouvelle connection echouee pour les modules [THERMOMETRE].\n");
                exit(EXIT_FAILURE);
            }
            // n = read(t_newsockfd, t_buffer, BUFFER_SIZE);
            // if (n < 0)
            //     error("[COMMUNICATION_TEMPERATURE - NEW_ITEM_CONNECTION_ERROR] : Impossible de lire le message transmis du module [THERMOMETRE].\n");
            struct Item *item = malloc(sizeof(struct Item *));
            item->socket = t_new_socket;

            message = "[COMMUNICATION_TEMPERATURE - INFO] : Connection etablie avec le module [THERMOMETRE] du module [PIECE] - ";
            printf("%s %s.\n", message, item->multicast);

            printf("[COMMUNICATION_TEMPERATURE - INFO] : %s - %d.\n", inet_ntoa(t_address.sin_addr), ntohs(t_address.sin_port));

            if (send(t_new_socket, message, strlen(message), 0) != strlen(message))
            {
                perror("[COMMUNICATION_TEMPERATURE - INFO] : Communication echoue avec les modules [THERMOMETRE].\n");
            }
            free(item);
            for (i = 0; i < MAX_THERMOMETRE; i++)
            {

                if (thermometres[i] == NULL)
                {
                    thermometres[i] = malloc(sizeof(struct Item *));
                    thermometres[i]->socket = t_new_socket;
                    printf("[COMMUNICATION_TEMPERATURE - INFO] : [THERMOMETRE] N° %d.\n", t_new_socket);

                    break;
                }
            }
        }

        for (i = 0; i < MAX_THERMOMETRE; i++)
        {
            if (thermometres[i])
            {
                sd = thermometres[i]->socket;

                if (FD_ISSET(sd, &t_readfds))
                {
                    if ((t_valread = read(sd, t_buffer, 1024)) == 0)
                    {
                        getpeername(sd, (struct sockaddr *)&t_address,
                                    (socklen_t *)&t_addrlen);
                        printf("[COMMUNICATION_TEMPERATURE - INFO] : Deconnection d'un [THERMOMETRE] - %s - %d.\n",
                               inet_ntoa(t_address.sin_addr), ntohs(t_address.sin_port));
                        close(sd);
                        thermometres[i] = NULL;
                    }
                    else
                    {
                        t_buffer[t_valread] = '\0';
                        if (strlen(t_buffer) > 0)
                        {
                            f_write_string(FILE_FROM_CT, t_buffer);
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

void *handle_c_send_message(struct Information *info)
{
    for (int i = 0; i < MAX_CHAUFFAGE; i++)
    {
        if (chauffages[i] != NULL)
        {
            // info->multicast == chauffages[i]->multicast &&
            if (chauffages[i]->socket >= 0)
            {
                if (info->valeur == 0)
                {
                    chauffages[i]->state = C_ITEM_STATE_OFF;
                }
                else
                {
                    chauffages[i]->state = C_ITEM_STATE_ON;
                }
                int n = write(chauffages[i]->socket, p_info_parse(info), strlen(p_info_parse(info)));
                if (n < 0)
                {
                    printf("[COMMUNICATION_TEMPERATURE - SEND_ERROR] : Impossible de communiquer avec le [CHAUFFAGE] de la [PIECE] : %d - %s.\n",
                           chauffages[i]->socket, chauffages[i]->multicast);
                }
                break;
            }
        }
    }
    return NULL;
}

void *handle_c_connection()
{

    char *message = "GESTION DE TEMPERATURE v1.0 \r\n";
    int i, sd;
    for (i = 0; i < MAX_CHAUFFAGE; i++)
    {
        chauffages[i] = NULL;
    }

    if ((c_master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("[COMMUNICATION_TEMPERATURE - MASTERSOCKET_ERROR] : Lancement du serveur echoue pour les modules [CHAUFFAGE].\n");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(c_master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0)
    {
        perror("[COMMUNICATION_TEMPERATURE - SETSOCKPORT_ERROR] : Lancement du serveur echoue pour les modules [CHAUFFAGE].\n");
        exit(EXIT_FAILURE);
    }

    c_address.sin_family = AF_INET;
    c_address.sin_addr.s_addr = INADDR_ANY;
    c_address.sin_port = htons(CHAUFFAGE_TO_SYSTEM_PORT);

    if (bind(c_master_socket, (struct sockaddr *)&c_address, sizeof(c_address)) < 0)
    {
        perror("[COMMUNICATION_TEMPERATURE - BINDING_ERROR] : Lancement du serveur echoue pour les modules [CHAUFFAGE].");
        exit(EXIT_FAILURE);
    }
    printf("[COMMUNICATION_TEMPERATURE - INFO] : Lancement du serveur pour les modules [CHAUFFAGE] sur le port # %d.\n", CHAUFFAGE_TO_SYSTEM_PORT);

    if (listen(c_master_socket, MAX_CHAUFFAGE) < 0)
    {
        perror("[COMMUNICATION_TEMPERATURE - LISTEN_ERROR] : Ecoute du serveur echoue pour les modules [CHAUFFAGE].");
        exit(EXIT_FAILURE);
    }

    c_addrlen = sizeof(c_address);
    puts("[COMMUNICATION_TEMPERATURE - INFO] : Attente de connection pour des modules [CHAUFFAGE]...\n");

    while (TRUE)
    {

        FD_ZERO(&c_readfds);

        FD_SET(c_master_socket, &c_readfds);
        c_max_sd = c_master_socket;

        for (i = 0; i < MAX_CHAUFFAGE; i++)
        {
            if (chauffages[i])
            {
                sd = chauffages[i]->socket;

                if (sd > 0)
                    FD_SET(sd, &c_readfds);

                if (sd > c_max_sd)
                    c_max_sd = sd;
            }
        }

        c_activity = select(c_max_sd + 1, &c_readfds, NULL, NULL, NULL);

        if ((c_activity < 0) && (errno != EINTR))
        {
            printf("[COMMUNICATION_TEMPERATURE - C_ACTIVITY_ERROR] : Capture d'activite impossible pour les modules [CHAUFFAGE].\n");
        }

        if (FD_ISSET(c_master_socket, &c_readfds))
        {
            if ((c_new_socket = accept(c_master_socket,
                                       (struct sockaddr *)&c_address, (socklen_t *)&c_addrlen)) < 0)
            {
                perror("[COMMUNICATION_TEMPERATURE - ACCEPT_ERROR] : Nouvelle connection echouee pour les modules [CHAUFFAGE].\n");
                exit(EXIT_FAILURE);
            }
            // n = read(c_newsockfd, c_buffer, BUFFER_SIZE);
            // if (n < 0)
            //     error("[COMMUNICATION_TEMPERATURE - NEW_ITEM_CONNECTION_ERROR] : Impossible de lire le message transmis du module [CHAUFFAGE].\n");
            struct Item *item = malloc(sizeof(struct Item *));
            item->socket = c_newsockfd;

            // chauffages[chauffages_count] = item;
            // chauffages_count++;
            message = "[COMMUNICATION_TEMPERATURE - INFO] : Connection etablie avec le module [CHAUFFAGE] du module [PIECE] - ";
            printf("%s %s %d.\n", message, item->multicast, c_new_socket);
            printf("[COMMUNICATION_TEMPERATURE - INFO] : %s - %d.\n", inet_ntoa(c_address.sin_addr), ntohs(c_address.sin_port));
            if (send(c_new_socket, message, strlen(message), 0) != strlen(message))
            {
                perror("[COMMUNICATION_TEMPERATURE - INFO] : Communication echoue avec les modules [CHAUFFAGE].\n");
            }
            free(item);
            for (i = 0; i < MAX_CHAUFFAGE; i++)
            {
                if (chauffages[i] == NULL)
                {
                    chauffages[i] = malloc(sizeof(struct Item *));
                    chauffages[i]->socket = c_new_socket;
                    printf("[COMMUNICATION_TEMPERATURE - INFO] : [CHAUFFAGE] S° %d.\n", c_new_socket);
                    break;
                }
            }
        }

        for (i = 0; i < MAX_CHAUFFAGE; i++)
        {
            if (chauffages[i])
            {
                sd = chauffages[i]->socket;

                if (FD_ISSET(sd, &c_readfds))
                {
                    if ((c_valread = read(sd, c_buffer, 1024)) == 0)
                    {
                        getpeername(sd, (struct sockaddr *)&c_address,
                                    (socklen_t *)&c_addrlen);
                        printf("[COMMUNICATION_TEMPERATURE - INFO] : Deconnection d'un [CHAUFFAGE] - %s - %d.\n",
                               inet_ntoa(c_address.sin_addr), ntohs(c_address.sin_port));
                        close(sd);
                        chauffages[i] = NULL;
                    }
                    else
                    {
                        fclose(fopen(FILE_FROM_CT, "w"));
                    }
                }
            }
        }
    }

    return NULL;
}

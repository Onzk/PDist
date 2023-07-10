#include "Sys_objects.h"

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

void *handle_rmi_connection()
{
	int sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in servaddr, cliaddr;

	printf("[GESTION CONSOLE - INFO] : Connection UDP au module [SERVER_RMI], demarree sur le port - %d.\n", RMI_SERVER_PORT);
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("[GESTION CONSOLE - ERROR] : Impossible de se connecter au module [SERVER_RMI].\n");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(RMI_SERVER_PORT);

	if (bind(sockfd, (const struct sockaddr *)&servaddr,
			 sizeof(servaddr)) < 0)
	{
		perror("[GESTION CONSOLE - ERROR] : Attribution de la connection echouee avec le module [SERVER_RMI].\n");
		exit(EXIT_FAILURE);
	}

	int n;
	socklen_t len;
	printf("[GESTION CONSOLE - INFO] : Connection ouverte pour le module [SERVER_RMI].\n");
	while (1)
	{
		sleep(2);
		FILE *file = fopen(FILE_FROM_CT, "r");
		if (file == NULL)
		{
			printf("[FILER - ERROR] : file not found at %s", FILE_FROM_CT);
			exit(1);
		}
		char message[BUFFER_SIZE];
		while (fgets(message, BUFFER_SIZE, file) != NULL)
		{
		}
		fclose(file);
		len = sizeof(cliaddr);

		n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE,
					 MSG_WAITALL, (struct sockaddr *)&cliaddr,
					 &len);
		n = sendto(sockfd, (const char *)message, strlen(message),
				   MSG_CONFIRM, (const struct sockaddr *)&cliaddr,
				   len);
	}
}

void *handle_ctl_c_connection()
{
	socklen_t len;
	char message[BUFFER_SIZE];
	int ctl_c_sockfd, ctl_portno;
	struct sockaddr_in ctl_serv_addr, ctl_cli_addr;
	socklen_t ctl_clilen;
	ctl_c_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (ctl_c_sockfd < 0)
		error("[GESTION_CONSOLE - SOCKET_ERROR] : Impossible de se connecter avec le module [CONTROLE_CONSOLE]\n");

	bzero((char *)&ctl_serv_addr, sizeof(ctl_serv_addr));
	ctl_portno = CONTROL_CONSOLE_PORT;
	ctl_serv_addr.sin_family = AF_INET;
	ctl_serv_addr.sin_addr.s_addr = INADDR_ANY;
	ctl_serv_addr.sin_port = htons(ctl_portno);

	if (bind(ctl_c_sockfd, (struct sockaddr *)&ctl_serv_addr, sizeof(ctl_serv_addr)) < 0)
		error("[GESTION_CONSOLE - BINDING_ERROR] : Impossible de se connecter avec le module [CONTROLE_CONSOLE]\n");

	listen(ctl_c_sockfd, 1);
	ctl_clilen = sizeof(ctl_cli_addr);
	ctl_c_sockfd = accept(ctl_c_sockfd, (struct sockaddr *)&ctl_cli_addr, &ctl_clilen);
	if (ctl_c_sockfd < 0)
		error("[GESTION_CONSOLE - ACCEPT_ERROR] : Impossible d'accepter la connection avec le module [CONTROLE_CONSOLE]\n");
	printf("[GESTION_CONSOLE - INFO] : Connection etablie avec le module [CONTROLE_CONSOLE]\n");
	while (1)
	{
		sleep(2);
		FILE *file = fopen(FILE_FROM_CT, "r");
		if (file == NULL)
		{
			printf("[FILER - ERROR] : file not found at %s", FILE_FROM_CT);
			exit(1);
		}
		char message[BUFFER_SIZE];
		while (fgets(message, BUFFER_SIZE, file) != NULL)
		{
		}
		if (strlen(message) > 0)
		{
			n = write(ctl_c_sockfd, message, strlen(message));
			if (n < 0)
				error("[GESTION CONSOLE - ERROR] : Reception impossible de [COMMUNICATION_TEMPERATURE]\n");
		}
		fclose(file);
	}
}

void *handle_cmd_c_connection()
{
	int sockfd, portno, n;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("[GESTION_CONSOLE - SOCKET_ERROR] : Impossible de se connecter avec le module [COMMANDE_CONSOLE]\n");

	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = COMMAND_CONSOLE_PORT;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("[GESTION_CONSOLE - BINDING_ERROR] : Impossible de se connecter avec le module [COMMANDE_CONSOLE]\n");

	listen(sockfd, 1);
	clilen = sizeof(cli_addr);

	cmd_c_sockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	if (cmd_c_sockfd < 0)
		error("[GESTION_CONSOLE - ACCEPT_ERROR] : Impossible d'accepter la connection avec le module [COMMANDE_CONSOLE]\n");
	printf("[GESTION_CONSOLE - INFO] : Connection etablie avec le module [COMMANDE_CONSOLE]\n");
	while (1)
	{
		char buffer[BUFFER_SIZE];
		bzero(buffer, BUFFER_SIZE);
		n = read(cmd_c_sockfd, buffer, BUFFER_SIZE);
		if (n < 0)
			error("[GESTION_CONSOLE - READING_ERROR] : Impossible de lire la requete du module [COMMANDE_CONSOLE]\n");
		f_write_string(FILE_FROM_GC, buffer);
	}
	close(cmd_c_sockfd);
	close(sockfd);
	return NULL;
}

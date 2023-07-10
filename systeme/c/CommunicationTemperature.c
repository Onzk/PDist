#include "../../pack/c/CommunicationTemperature.h"

int main()
{
    fclose(fopen(FILE_FROM_CT, "w"));
    pthread_t chauffageId, thermometreId;
    pthread_create(&thermometreId, NULL, handle_t_connection, NULL);
    pthread_create(&chauffageId, NULL, handle_c_connection, NULL);
    char message[BUFFER_SIZE];
    while (1)
    {
        FILE *file = fopen(FILE_FROM_GC, "r");
        sleep(2);
        if (file == NULL)
        {
            printf("[FILER - ERROR] : file not found at %s\n", FILE_FROM_GC);
            exit(1);
        }
        while (fgets(message, BUFFER_SIZE, file) != NULL)
        {
        }
        struct Information *info = p_info_read(message);
        if (info != NULL)
        {
            if (info->temperature != NO_TEMPERATURE)
            {
                handle_c_send_message(info);
            }
            fclose(fopen(FILE_FROM_GC, "w"));
        }
        bzero(message, BUFFER_SIZE);
        free(info);
        fclose(file);
    }
    pthread_join(chauffageId, NULL);
    pthread_join(thermometreId, NULL);
    close(t_sockfd);
    close(c_sockfd);
    return 0;
}

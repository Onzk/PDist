#include "../../pack/c/GestionConsole.h"

int main()
{
    fclose(fopen(FILE_FROM_GC, "w"));
    pthread_t rmi_id, ctl_c_id, cmd_c_id, h_t_check_id;
    pthread_create(&rmi_id, NULL, handle_rmi_connection, NULL);
    pthread_create(&cmd_c_id, NULL, handle_cmd_c_connection, NULL);
    pthread_create(&ctl_c_id, NULL, handle_ctl_c_connection, NULL);
    pthread_join(rmi_id, NULL);
    pthread_join(cmd_c_id, NULL);
    pthread_join(ctl_c_id, NULL);
    close(ct_socket);
    return 0;
}
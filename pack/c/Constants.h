// Booleans
#define TRUE 1
#define FALSE 0

// Ports
#define CONSOLE_PORT 7777
#define MULTICAST_PORT 8878
#define SYSTEM_PORT 64500
#define CHAUFFAGE_TO_SYSTEM_PORT 4444
#define THERMOMETRE_TO_SYSTEM_PORT 5544
#define RMI_SERVER_PORT 9999
#define COMMAND_CONSOLE_PORT 3333
#define CONTROL_CONSOLE_PORT 2222

// Hosts

#define MULTICAST_HOST (char*) "230.0.0.0"

// Message types
#define CONTROL_REQUEST "0"
#define COMMAND_REQUEST "1"
#define INFO_REQUEST "2"

// Buffer size
#define BUFFER_SIZE 2048

// Limits
#define MAX_THERMOMETRE 30
#define MAX_CHAUFFAGE 30
#define MAX_MESSAGE_LENGTH 2048

// Files names
#define FILE_FROM_CT "./systeme/c/FROM_CT.txt"
#define FILE_FROM_GC "./systeme/c/FROM_GC.txt"

// Timer
#define REFRESH_TIME 500

// Command modes
const char MODE_AUTOMATIC = 'A';
const char MODE_MANUAL = 'M';

// Others
const char *THERMOMETRE = "1";
const char *CHAUFFAGE = "0";
const int NO_TEMPERATURE = -1;

// Information struct keys
char* C_INFO_ID = (char*)"id";
char* C_INFO_PIECE = (char*)"piece";
char* C_INFO_TEMPERATURE = (char*)"temperature";
char* C_INFO_VALEUR = (char*)"valeur";
char* C_INFO_TYPE = (char*)"type";

//Item struct keys
char* C_ITEM_ID = (char*)"id";
char* C_ITEM_PIECE = (char*)"piece";
char* C_ITEM_SOCKET = (char*)"socket";
char* C_ITEM_TYPE = (char*)"type";
char* C_ITEM_STATE = (char*)"state";

// States
char* C_ITEM_STATE_ON = "ON";
char* C_ITEM_STATE_OFF = "OFF";

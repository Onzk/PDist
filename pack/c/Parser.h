#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>
#include <string.h>
#include "Information.h"
#include "Item.h"
#include "Constants.h"

struct Information *p_info_read(char *message)
{
    struct Information *info = malloc(sizeof(struct Information));
    struct json_object *parsed_json, *id, *piece, *temperature, *valeur, *type;
    parsed_json = json_tokener_parse(message);
    json_object_object_get_ex(parsed_json, C_INFO_ID, &id);
    json_object_object_get_ex(parsed_json, C_INFO_PIECE, &piece);
    json_object_object_get_ex(parsed_json, C_INFO_TEMPERATURE, &temperature);
    json_object_object_get_ex(parsed_json, C_INFO_VALEUR, &valeur);
    json_object_object_get_ex(parsed_json, C_INFO_TYPE, &type);
    info->multicast = json_object_get_string(id);
    info->piece = json_object_get_string(piece);
    info->temperature = json_object_get_double(temperature);
    info->valeur = json_object_get_int(valeur);
    info->type = json_object_get_int(type);
    if(info->type == 0){
        return NULL;
    }
    return info;
}

char *p_info_parse(struct Information *info)
{
    char *buffer;
    char message[MAX_MESSAGE_LENGTH] = "";
    snprintf(message, sizeof(char[MAX_MESSAGE_LENGTH]),
             "{\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%f\",\"%s\":\"%d\",\"%s\":%d}",
             C_INFO_ID, info->multicast,
             C_INFO_PIECE, info->piece,
             C_INFO_TEMPERATURE, info->temperature,
             C_INFO_VALEUR, info->valeur,
             C_INFO_TYPE, info->type);
    buffer = message;
    return buffer;
}

char *p_info_parse2(struct Information *info)
{
    char *buffer;
    char message[MAX_MESSAGE_LENGTH] = "";
    snprintf(message, sizeof(char[MAX_MESSAGE_LENGTH]),
             "[Piece = %s | temp = :%f]", info->piece, info->temperature);
    buffer = message;
    return buffer;
}

struct Item *p_item_read(char *message)
{
    struct Item *item;
    struct json_object *parsed_json, *id, *piece, *socket, *type;
    parsed_json = json_tokener_parse(message);
    json_object_object_get_ex(parsed_json, C_ITEM_ID, &id);
    json_object_object_get_ex(parsed_json, C_ITEM_PIECE, &piece);
    json_object_object_get_ex(parsed_json, C_ITEM_SOCKET, &socket);
    json_object_object_get_ex(parsed_json, C_ITEM_TYPE, &type);
    item->multicast = (char *)json_object_get_string(id);
    item->piece = json_object_get_string(piece);
    item->socket = json_object_get_int(socket);
    item->type = json_object_get_string(type);
    return item;
}

char *p_item_parse(struct Item *item)
{
    char *buffer;
    char message[MAX_MESSAGE_LENGTH] = "";
    snprintf(message, sizeof(char[MAX_MESSAGE_LENGTH]),
             "{\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%d\",\"%s\":\"%s\"}",
             C_ITEM_ID, item->multicast, C_ITEM_PIECE, item->piece, C_ITEM_SOCKET, item->socket, C_ITEM_TYPE, item->type);
    buffer = message;
    return buffer;
}

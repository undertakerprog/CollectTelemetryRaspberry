#include "json.h"

char *serialize_to_JSON(float temperature, const char *memory_info, float humidity, float dht11_temperature, int sound_value) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temperature of cpu", temperature);
    cJSON_AddStringToObject(root, "memory_info", memory_info);
    cJSON_AddNumberToObject(root, "humidity", humidity);
    cJSON_AddNumberToObject(root, "temperature", dht11_temperature);
    cJSON_AddNumberToObject(root, "sound_value", sound_value);

    char *serialized = cJSON_Print(root);
    cJSON_Delete(root);

    return serialized;
}

char *serialize_to_JSON_without_sencors(float temperature, const char *memory_info, int sound_value) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temperature of cpu", temperature);
    cJSON_AddStringToObject(root, "memory_info", memory_info);
    cJSON_AddStringToObject(root, "humidity", "N/A");
    cJSON_AddStringToObject(root, "temperature", "N/A");
    cJSON_AddNumberToObject(root, "sound_value", sound_value);

    char *serialized = cJSON_Print(root);
    cJSON_Delete(root);
    return serialized;
}
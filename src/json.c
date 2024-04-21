#include "json.h"

char *serialize_to_JSON(float temperature, const char *memory_info, float humidity, float dht11_temperature, int sound_value, int is_file) {
    cJSON *root = cJSON_CreateObject();

    char *serialized;
    if (is_file) {
        char temp_str[20]; 
        snprintf(temp_str, sizeof(temp_str), "%.3f", temperature); 
        cJSON_AddStringToObject(root, "temperature of cpu", temp_str);

        cJSON_AddStringToObject(root, "memory info", memory_info);

        cJSON_AddNumberToObject(root, "humidity", humidity);
    
        char dht11_temp_str[20]; 
        snprintf(dht11_temp_str, sizeof(dht11_temp_str), "%.3f", dht11_temperature); 
        cJSON_AddStringToObject(root, "temperature", dht11_temp_str);

        cJSON_AddNumberToObject(root, "sound value", sound_value);

        serialized = cJSON_Print(root); // Форматированная сериализация для терминала
    } else {
        char temp_str[20]; 
        snprintf(temp_str, sizeof(temp_str), "%.3f", temperature); 
        cJSON_AddStringToObject(root, "temp_cpu", temp_str);

        cJSON_AddStringToObject(root, "mem", memory_info);

        cJSON_AddNumberToObject(root, "hum", humidity);
    
        char dht11_temp_str[20]; 
        snprintf(dht11_temp_str, sizeof(dht11_temp_str), "%.3f", dht11_temperature); 
        cJSON_AddStringToObject(root, "temp", dht11_temp_str);

        cJSON_AddNumberToObject(root, "sound", sound_value);

        serialized = cJSON_PrintUnformatted(root); // Неформатированная сериализация для терминала
    }
    cJSON_Delete(root);

    return serialized;
}

char *serialize_to_JSON_without_sencors(float temperature, const char *memory_info, int sound_value) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temperature of cpu", temperature);
    cJSON_AddStringToObject(root, "memory info", memory_info);
    cJSON_AddStringToObject(root, "humidity", "N/A");
    cJSON_AddStringToObject(root, "temperature", "N/A");
    cJSON_AddNumberToObject(root, "sound value", sound_value);

    char *serialized = cJSON_Print(root);
    cJSON_Delete(root);
    return serialized;
}
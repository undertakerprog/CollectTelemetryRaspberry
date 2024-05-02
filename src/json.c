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

char *serialize_to_JSON_without_sencors(float temperature, const char *memory_info, float cpu_load, float cpu_frequency, int connection, long available_memory, float network_usage, long process_memory_usage) {
    cJSON *root = cJSON_CreateObject();
    char temp_str[20]; 
    snprintf(temp_str, sizeof(temp_str), "%.3f °C", temperature); 
    cJSON_AddStringToObject(root, "temperature of cpu", temp_str);
    cJSON_AddStringToObject(root, "memory info", memory_info);

    char cpu_load_str[20]; 
    snprintf(cpu_load_str, sizeof(cpu_load_str), "%.3f%%", cpu_load); 
    cJSON_AddStringToObject(root, "cpu load", cpu_load_str);

    char cpu_frequency_str[20]; 
    snprintf(cpu_frequency_str, sizeof(cpu_frequency_str), "%.3f GHz", cpu_frequency); 
    cJSON_AddStringToObject(root, "cpu frequency", cpu_frequency_str);

    cJSON_AddNumberToObject(root, "total available memory (bytes)", available_memory * 1024); 
    cJSON_AddNumberToObject(root, "network connections", connection);

    char network_usage_str[20]; 
    snprintf(network_usage_str, sizeof(network_usage_str), "%.3f KB", network_usage); 
    cJSON_AddStringToObject(root, "network usage", network_usage_str);

    cJSON_AddNumberToObject(root, "process memory usage (bytes)", process_memory_usage); 

    char *serialized = cJSON_Print(root);
    cJSON_Delete(root);
    return serialized;
}

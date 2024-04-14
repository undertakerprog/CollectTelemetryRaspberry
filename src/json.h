#include <cjson/cJSON.h>

char *serialize_to_JSON(float temperature, const char *memory_info, float humidity, float dht11_temperature, int sound_value);

char *serialize_to_JSON_without_sencors(float temperature, const char *memory_info, int sound_value);
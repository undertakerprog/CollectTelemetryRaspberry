#include "collect_data.h"
#include "json.h"

int main(int argc, char **argv) {
    char memory_info[128];
    float temperature = get_cpu_temperature();
    float humidity = 0, dht11_temperature = 0;
    int sound = 0;

    int success_memory = get_memory_info(memory_info, sizeof(memory_info));
    if (temperature == -1.0 || !success_memory) {
        printf("Failed to get CPU temperature or kernel version or memory info");
        return 1;
    }

    if (wiringPiSetup() == -1) {
        printf("Failed to initialize wiringPi.\n");
        return 1;
    }

    char *json_str;

    read_dht11_dat(&humidity, &dht11_temperature);
    if (humidity != -1.0 && dht11_temperature != -1.0) {
        while (1) {
            read_dht11_dat(&humidity, &dht11_temperature);

            json_str = serialize_to_JSON(temperature, memory_info, humidity, dht11_temperature, sound);
            printf("Serialized JSON: %s\n", json_str);
            free(json_str);
            delay(1000);
        }
    }
    else {
        json_str = serialize_to_JSON_without_sencors(temperature, memory_info, sound);
        printf("Serialized JSON: %s\n", json_str);
        free(json_str);
    }

    return 0;
}
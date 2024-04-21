#include "send_data.h"
#include "collect_data.h"
#include "json.h"

int main(int argc, char **argv) {

    char memory_info[128];
    float temperature = get_cpu_temperature();
    float humidity = 0, dht11_temperature = 0;
    int sound = 0;
    int sensors_available = 0; // Флаг, указывающий, доступны ли данные с датчиков
    char *json_str;

    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to create file %s\n", DATA_FILE);
        return 1;
    }
    fclose(file);

    send_file_to_computer();

    int success_memory = get_memory_info(memory_info, sizeof(memory_info));
    if (temperature == -1.0 || !success_memory) {
        printf("Failed to get CPU temperature or kernel version or memory info");
        return 1;
    }

    if (wiringPiSetup() == -1) {
        printf("Failed to initialize wiringPi.\n");
        return 1;
    }

    pinMode(SOUND_PIN, INPUT);
    
    unsigned long start_time = millis();
    while (millis() - start_time < SENSOR_CHECK_DELAY_MS) { 
        read_dht11_dat(&humidity, &dht11_temperature); 
        if (humidity != -1.0 && dht11_temperature != -1.0) { // Если данные успешно считаны хотя бы один раз
            sensors_available = 1; 
            break; 
        }
    }

    // Если данные с датчиков доступны, входим в основной цикл
    if (sensors_available) {
        while (1) {
            read_dht11_dat(&humidity, &dht11_temperature);
            if(temperature != -1 && humidity != -1){
                
                sound = read_sound_sensor();
                json_str = serialize_to_JSON(temperature, memory_info, humidity, dht11_temperature, sound, 1);
                printf("Serialized JSON: %s\n", json_str);
                json_str = serialize_to_JSON(temperature, memory_info, humidity, dht11_temperature, sound, 0);
                file = fopen(DATA_FILE, "a");
                if(file == NULL){
                    fprintf(stderr, "Failed to open file %s for append\n", DATA_FILE);
                    return 1;
                }
                fprintf(file, "%s \n", json_str);
                fclose(file);
                send_file_to_computer();
                free(json_str);
            }
            delay(1000);
        }   
    } else {
        // Если данные с датчиков недоступны, выводим JSON без данных о датчиках
        json_str = serialize_to_JSON_without_sencors(temperature, memory_info, sound);
        printf("Serialized JSON: %s\n", json_str);
        free(json_str);
    }

    return 0;
}

#include "send_data.h"
#include "collect_data.h"
#include "json.h"
#include "check.h"
#include <unistd.h> 


int sound = 0;
float humidity = 0, dht11_temperature = 0;
char memory_info[128];
char *json_str = NULL;


int perform_sensor_loop(float humidity, float dht11_temperature, float temperature, int sound, char *json_str, FILE *file) {
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
    return 0;
}

void handle_sensor_unavailable(char *json_str, float temperature, float cpu_load, float cpu_frequency, long available_memory, int connection_count, float network_usage, long process_memory_usage) {
    json_str = serialize_to_JSON_without_sencors(temperature, memory_info, cpu_load, cpu_frequency, available_memory, connection_count, network_usage, process_memory_usage);
    printf("Serialized JSON without sencors(Connect the sensors): %s\n", json_str);
    free(json_str);
}

int main(int argc, char **argv) {

    int opt;
    int run_sensor_loop = 0;
    int run_sensor_unavailable = 0;

    while ((opt = getopt(argc, argv, "ds")) != -1) {
        switch (opt) {
            case 'd':
                run_sensor_unavailable = 1;
                break;
            case 's':
                run_sensor_loop = 1;
                break;
            default: 
                fprintf(stderr, "Usage: %s [-d] [-s]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (run_sensor_loop && run_sensor_unavailable) {
        fprintf(stderr, "Cannot use both -d and -s flags simultaneously.\n");
        exit(EXIT_FAILURE);
    }

    if (!run_sensor_loop && !run_sensor_unavailable) {
        fprintf(stderr, "At least one of -d or -s flags must be provided.\n");
        exit(EXIT_FAILURE);
    }

    int sensors_available = 0; // Флаг, указывающий, доступны ли данные с датчиков
    int connection_count = get_network_connections();

    long process_memory_usage = get_process_memory_usage();
    long available_memory = get_available_memory();

    float cpu_load = get_cpu_load();
    float cpu_frequency = get_cpu_frequency();
    float temperature = get_cpu_temperature();
    float network_usage = get_power_usage();
    

    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to create file %s\n", DATA_FILE);
        return 1;
    }
    fclose(file);

    
    int success_memory = get_memory_info(memory_info, sizeof(memory_info));

    check_cpu_temperature_and_memory_info(temperature, get_memory_info(memory_info, sizeof(memory_info)));

    check_available_memory(available_memory);
    
    check_network_connections(connection_count);

    check_wiringPi_setup(wiringPiSetup());

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
    if (run_sensor_loop) {
        if (sensors_available) {
            send_file_to_computer();
            perform_sensor_loop(humidity, dht11_temperature, temperature, sound, json_str, file);
        } else {
            fprintf(stderr, "Sensors are not available. Cannot perform sensor loop.\n");
            exit(EXIT_FAILURE);
        }
    } else if (run_sensor_unavailable) {
        handle_sensor_unavailable(json_str, temperature, cpu_load, cpu_frequency, available_memory, connection_count, network_usage, process_memory_usage);
    }

    return 0;
}

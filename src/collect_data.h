#include <stdint.h>
#include <wiringPi.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXTIMINGS 85
#define DHTPIN 7
#define SOUND_PIN 0
#define GPIO_PIN "4"

//sencors 
int read_sound_sensor();

void read_dht11_dat(float *humidity, float *dht11_temperature);

int read_gpio_state();

//device
float get_cpu_temperature();

int get_memory_info(char *memory_info, int size);

float get_cpu_load();

float get_cpu_frequency();

long get_available_memory();

int get_network_connections();

float get_power_usage();

long get_process_memory_usage();

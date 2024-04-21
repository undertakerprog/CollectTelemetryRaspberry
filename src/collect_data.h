#include <stdint.h>
#include <wiringPi.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXTIMINGS 85
#define DHTPIN 7
#define SOUND_PIN 0

//sencors 
int read_sound_sensor();

void read_dht11_dat(float *humidity, float *dht11_temperature);

//device
float get_cpu_temperature();

int get_memory_info(char *memory_info, int size);
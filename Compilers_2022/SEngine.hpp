#include <stdint.h>
#include <stdio.h>
#include <chrono>

#include <glm/glm.hpp>

void sEngine_init(uint32_t width, uint32_t height);

bool sEngine_windowIsOpen() ;

extern "C" void put_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

extern "C" void flush();

extern "C" long long get_time_milliseconds();
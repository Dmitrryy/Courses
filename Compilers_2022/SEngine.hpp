#include <stdint.h>
#include <stdio.h>
#include <chrono>

#include <glm/glm.hpp>

/**
 * @brief create window
 * 
 * @param width - width of the window in pixels
 * @param height - height of the window in pixels
 */
void sEngine_init(uint32_t width, uint32_t height);

/**
 * @brief Checks the window events and closes it if necessary
 * 
 * @return true - window is open
 * @return false - window was closed
 * 
 * @note without a call sEngine_init the behavior is undefined
 */
bool sEngine_windowIsOpen();

/**
 * @brief returns the time since the start of the program
 * 
 */
extern "C" long long get_time_milliseconds();


extern "C" void put_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

extern "C" void flush();
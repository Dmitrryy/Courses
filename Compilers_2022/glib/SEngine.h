#include <stdint.h>

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
int sEngine_windowIsOpen();

/**
 * @brief returns the time since the start of the program
 * 
 */
long long get_time_milliseconds();


void put_pixel(uint32_t x, uint32_t y, uint32_t rgba);

void flush();

// DLC
int sEngine_rand();
void sEngine_clear();
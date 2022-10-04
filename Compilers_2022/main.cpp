#include "SEngine.hpp"


uint32_t shader_1(int64_t x, int64_t y, int64_t res_x, int64_t res_y, int64_t time);

uint8_t unpack_4ui8_r(uint32_t rgba);
uint8_t unpack_4ui8_g(uint32_t rgba);
uint8_t unpack_4ui8_b(uint32_t rgba);
uint8_t unpack_4ui8_a(uint32_t rgba);

int main()
{
    const int64_t width = 1024;
    const int64_t heigth = 576;
    sEngine_init(width, heigth);

    while(sEngine_windowIsOpen())
    {
        long long cur_time = get_time_milliseconds();

        for(int64_t y = 0; y < heigth; ++y)
        {
            for (int64_t x = 0; x < width; ++x)
            {
                uint32_t color = shader_1(x, y, width, heigth, cur_time);
                put_pixel(x, y, unpack_4ui8_r(color), unpack_4ui8_g(color), unpack_4ui8_b(color), unpack_4ui8_a(color));
            }
        }
        
        flush();
    }

    return 0;   
}

uint32_t pack_4ui8_to_ui32(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    uint32_t result = 0;
    result = result | (a);
    result = result | (b << 8);
    result = result | (g << 16);
    result = result | (r << 24);

    return result;
}

uint8_t unpack_4ui8_r(uint32_t rgba) { return (rgba & 0xFF000000) >> 24; }
uint8_t unpack_4ui8_g(uint32_t rgba) { return (rgba & 0x00FF0000) >> 16; }
uint8_t unpack_4ui8_b(uint32_t rgba) { return (rgba & 0x0000FF00) >> 8;  }
uint8_t unpack_4ui8_a(uint32_t rgba) { return (rgba & 0x000000FF);       }

/**
 * @brief Integer approximation of cos. Period T = 4000. 
 * A parabola is used for approximation.
 * 
 * @param x - arg
 * @return int32_t - approximate value of cos * 10^3.
 */
int64_t int_cos(int64_t x)  
{
    int64_t fracted_x = x % 1000;
    int64_t per_id = (std::abs(x) % 4000) / 1000;
    if(per_id % 2) {
        fracted_x = 1000 - fracted_x;
    }

    int64_t sign = (per_id == 1 || per_id == 2) ? 1 : -1;
    int64_t y = (fracted_x * fracted_x) / 1000 - 1000;
    return y * sign;
}

/**
 * @brief related to shader_1
 */
uint32_t palette_int(int64_t t, int64_t d1, int64_t d2, int64_t d3) 
{
    uint8_t x = (500 + 500 * int_cos(628318 * (1000 * t / 1000 + d1)/100000) / 1000) / 5;
    uint8_t y = (500 + 500 * int_cos(628318 * (1000 * t / 1000 + d2)/100000) / 1000) / 5;
    uint8_t z = (500 + 500 * int_cos(628318 * (0    * t / 1000 + d3)/100000) / 1000) / 5;
    return pack_4ui8_to_ui32(x, y, z, t / 4);
}


uint32_t shader_1(int64_t x, int64_t y, int64_t res_x, int64_t res_y, int64_t time)
{
    uint32_t result = 0x000000FF;

    int64_t uv_x = x * 1000 / res_x;
    int64_t uv_y = y * 1000 / res_y;
    time = time / 2;

    uint32_t maxIter = 75;
    int64_t r = 788*(int_cos((time / 3) - 2000) / 5 + 850) / 1000; // 10^-3
    
    int64_t c1 = r * int_cos(time / 3) / 1000;
    int64_t c2 = r * int_cos(time / 3 + 3000) / 1000;

    int64_t z1 = 3 * (uv_x - 500);
    int64_t z2 = 2 * (uv_y - 500);

    bool escaped = false;
    uint32_t iterations = 0;
    for (; iterations < maxIter; ++iterations) {
        int64_t z1_tmp = (z1 * z1 - z2 * z2) / 1000 + c1;
        z2 = (z2 * z1 + z1 * z2) / 1000 + c2;
        z1 = z1_tmp;
        if((z1 * z1 + z2 * z2) / 10000 > 4000) {
            escaped = true;
            break;
        }
    }

    if(escaped){
        result = palette_int(iterations * 1000 / maxIter,
            300 + (int_cos(time + 3000) * 2 / 5),
            200 + (int_cos(time + 3600) * 3 / 10 ),
            200 + (int_cos(time + 4400) * 3 / 10 )
            );
    }

    return result;
}
#include "SEngine.hpp"

#include <array>

std::array<uint8_t, 4> shader_1_f(const std::array<int64_t, 2> &fragCoord, const std::array<int64_t, 2>& resolution, int64_t time);


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
                auto &&color = shader_1_f({x, y}, {width, heigth}, cur_time);
                put_pixel(x, y, color[0], color[1], color[2], color[3]);
            }
        }
        
        flush();
    }

    return 0;   
}


/**
 * @brief T = 4000
 * 
 * @param x 
 * @return int32_t 
 */
int64_t int_cos(int64_t x)  {
    int64_t fracted_x = x % 1000;
    int64_t per_id = (std::abs(x) % 4000) / 1000;
    if(per_id == 1 || per_id == 3) {
        fracted_x = 1000 - fracted_x;
    }

    int64_t sign = (per_id == 1 || per_id == 2) ? 1 : -1;
    int64_t y = (fracted_x * fracted_x) / 1000 - 1000;
    return y * sign;
}


std::array<int8_t, 3> palette_int(int64_t t
, const std::array<int64_t, 3>& a
, const std::array<int64_t, 3>& b
, const std::array<int64_t, 3>& c
, const std::array<int64_t, 3>& d) {
    std::array<int8_t, 3> result;
    result[0] = (a[0] + b[0] * int_cos(628318 * (c[0] * t / 1000 + d[0])/100000) / 1000) / 5;
    result[1] = (a[1] + b[1] * int_cos(628318 * (c[1] * t / 1000 + d[1])/100000) / 1000) / 5;
    result[2] = (a[2] + b[2] * int_cos(628318 * (c[2] * t / 1000 + d[2])/100000) / 1000) / 5;
    return result;
}


std::array<int64_t, 2> fractal_step(const std::array<int64_t, 2>& x, const std::array<int64_t, 2>& c) {
    std::array<int64_t, 2> result;
    result[0] = (x[0] * x[0] - x[1] * x[1]) / 1000 + c[0];
    result[1] = (x[1] * x[0] + x[0] * x[1]) / 1000 + c[1];

    return result;
}


std::array<uint8_t, 4> shader_1_f(const std::array<int64_t, 2> &fc, const std::array<int64_t, 2>& res, int64_t time)
{
    std::array<uint8_t, 4> result = {0, 0, 0, 255};
    std::array<int64_t, 2> uv = {
        fc[0] * 1000 / res[0],
        fc[1] * 1000 / res[1]
    };
    time = time / 2;

    uint32_t maxIter = 100;
    int64_t r = 788*(int_cos((time / 3) - 2000) / 5 + 850) / 1000; // 10^-3
    std::array<int64_t, 2> c = {
        r * int_cos(time / 3) / 1000,
        r * int_cos(time / 3 + 3000) / 1000
    };

    std::array<int64_t, 2> z = {
        3 * (uv[0] - 500),
        2 * (uv[1] - 500)
    };

    bool escaped = false;
    uint32_t iterations = 0;
    for (; iterations < maxIter; ++iterations) {
        z = fractal_step(z, c);
        if((z[0] * z[0] + z[1] * z[1]) / 10000 > 4000) {
            escaped = true;
            break;
        }
    }

    auto&& iterCol = palette_int(iterations * 1000 / maxIter, 
    {500, 500, 500},
    {500, 500, 500},
    {1000, 1000, 0},
    { 300 + (int_cos(time + 3000) * 2 / 5),
    200 + (int_cos(time + 3600) * 3 / 10 ),
    200 + (int_cos(time + 4400) * 3 / 10 )}
    );

    if(escaped){
        result[0] = iterCol[0];
        result[1] = iterCol[1];
        result[2] = iterCol[2];
    }
    result[3] = iterations * 255 / maxIter;

    return result;
}
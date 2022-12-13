#include "SEngine.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>
#include <chrono>


std::unique_ptr< sf::RenderWindow > g_window;
sf::Image g_image;
uint32_t g_width, g_heigth;
std::chrono::time_point<std::chrono::high_resolution_clock> g_timer = std::chrono::high_resolution_clock::now();


uint8_t unpack_4ui8_r(uint32_t rgba) { return (rgba & 0xFF000000) >> 24; }
uint8_t unpack_4ui8_g(uint32_t rgba) { return (rgba & 0x00FF0000) >> 16; }
uint8_t unpack_4ui8_b(uint32_t rgba) { return (rgba & 0x0000FF00) >> 8;  }
uint8_t unpack_4ui8_a(uint32_t rgba) { return (rgba & 0x000000FF);       }


extern "C" void sEngine_init(uint32_t width, uint32_t heigth)
{
    g_width = width;
    g_heigth = heigth;
    g_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(g_width, g_heigth), "Compiler_2022");
    g_image.create(g_width, g_heigth);

    sf::View view(sf::FloatRect(0.f, 0.f, g_width, g_heigth));
}

extern "C" int sEngine_windowIsOpen() 
{
    sf::Event event;
    while(g_window->pollEvent(event)) {
        if(event.type == sf::Event::Closed)
            g_window->close();
    }
    return g_window->isOpen();
}

extern "C" void put_pixel(uint32_t x, uint32_t y, uint32_t rgba)
{
    sf::Color color{unpack_4ui8_r(rgba), unpack_4ui8_g(rgba), unpack_4ui8_b(rgba), unpack_4ui8_a(rgba)};
    g_image.setPixel(x, y, color);
}

extern "C" void flush()
{
    sf::Texture texture;
    texture.loadFromImage(g_image);

    sf::Sprite sprite(texture);

    g_window->clear();
    g_window->draw(sprite);
    g_window->display();
}

extern "C" long long get_time_milliseconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - g_timer).count();
}

// DLC
extern "C" int sEngine_rand()
{
    return rand();
}

extern "C" void sEngine_clear()
{
    g_window->clear(sf::Color::Black);
}

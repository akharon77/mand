#include "calc.h"
#include "view.h"

void MandGetImage(const MandConfig *conf, sf::Image *img)
{
    int32_t width  = conf->width,
            height = conf->height;

    for (int32_t yi = 0; yi < height; ++yi)
    {
        for (int32_t xi = 0; xi < width; ++xi)
        {
            int32_t pos = yi * width + xi;
            int32_t cnt = conf->cnt_arr[pos];

            if (cnt == N_MAX)
                img->setPixel(xi, yi, sf::Color::Black);
            else
            {
                img->setPixel(xi, yi, sf::Color((uint8_t) 15 * cnt + 10,
                                                (uint8_t) 4 * cnt + 20,
                                                (uint8_t) 5 * cnt + 30));
            }
        }
    }
}

void MandRun(MandConfig *conf)
{
    sf::RenderWindow window(sf::VideoMode(conf->width, conf->height), "Mandelbrot");

    sf::Image img = {};
    img.create(conf->width, conf->height);

    sf::Texture texture = {};
    sf::Sprite  sprite  = {};

    float acc_x = 0,
          acc_y = 0;

    float vel_x = 0,
          vel_y = 0;

    float acc_scale = 0,
          vel_scale = 1;
    
    while (window.isOpen())
    {
        sf::Event event = {};

        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        //==========================================
        acc_x = acc_y = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            acc_y = -ACC_COORD;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            acc_y = ACC_COORD;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            acc_x = -ACC_COORD;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            acc_x = ACC_COORD;

        vel_x += acc_x * conf->scale;
        vel_y += acc_y * conf->scale;

        conf->base_x += vel_x;
        conf->base_y += vel_y;

        vel_x *= K_DEC_VEL * conf->scale;
        vel_y *= K_DEC_VEL * conf->scale;
        //==========================================

        //==========================================
        acc_scale = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
            acc_scale = -ACC_SCALE;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
            acc_scale = ACC_SCALE;

        vel_scale += acc_scale;

        MandTransfScale(conf, vel_scale);

        --vel_scale;
        vel_scale *= K_DEC_SCALE;
        ++vel_scale;
        //==========================================

        MandCalcAVX512(conf);
        // MandCalcNoOpts(conf);
        MandGetImage(conf, &img);

        texture.loadFromImage(img);
        sprite.setTexture(texture);

        window.clear();
        window.draw(sprite);
        window.display();
    }
}


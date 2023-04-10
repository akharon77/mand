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
                img->setPixel(xi, yi, sf::Color((uint8_t) 3 * cnt + 10,
                                                (uint8_t) 4 * cnt + 20,
                                                (uint8_t) 5 * cnt + 30));

                // dprintf(2, "xi = %d, yi = %d, cnt = %d\n", xi, yi, cnt);
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
    
    while (window.isOpen())
    {
        sf::Event event = {};

        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            acc_y = -DEF_ACC;
            // conf->base_y -= STEP_Y;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            acc_y = DEF_ACC;
            // conf->base_y += STEP_Y;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            acc_x = -DEF_ACC;
            // conf->base_x -= STEP_X;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            acc_x = DEF_ACC;
            // conf->base_x += STEP_X;
        else
            acc_x = acc_y = 0;

        acc_x *= conf->scale;
        acc_y *= conf->scale;

        vel_x += acc_x;
        vel_y += acc_y;

        conf->base_x += vel_x;
        conf->base_y += vel_y;

        vel_x *= K_DEC;
        vel_y *= K_DEC;

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


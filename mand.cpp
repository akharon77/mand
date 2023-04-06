#include <immintrin.h>

#include "mand.h"

void TransformCoordScale(MandConfig *config, float *x, float *y)
{
    *x = *x / config->width  * 2 * X_TRANSF_RAD;
    *y = *y / config->height * 2 * Y_TRANSF_RAD;
}

void TransfromCoord(MandConfig *config, float *x, float *y)
{
    TransformCoordScale(config, x, y);

    *x = *x - config->x_transf_rad;
    *y = config->y_transf_rad - *y;
}

void CalculateMandelbrotNoAVX(MandConfig *config)
{
    float *x_arr = config->x_arr,
          *y_arr = config->y_arr;

    float base_x = config->base_x,
          base_y = config->base_y;

    TransfromCoord(config, &base_x, &base_y);

    float delta_x = 1,
          delta_y = 1;

    TransformCoordScale(config, &delta_x, &delta_y);

    int32_t width  = config->width,
            height = config->heigh;

    float r_max_quad = config->r_max * config->r_max;

    float y0 = base_y;

    for (int32_t yi = 0; yi < height; ++yi, y0 += delta_y)
    {
        float x0 = base_x;

        for (int32_t xi = 0; xi < width; ++xi, x0 += delta_x)
        {
            int32_t n = 0;

            float x1 = x0,
                  y1 = y0;

            while (n < N_MAX)
            {
                x1_sqr   = x1 * x1;
                y1_sqr   = y1 * y1;
                dbl_x1y1 = 2 * x1 * y1;

                x1 = x1_sqr - y1_sqr + x0;
                y1 = dbl_x1y1 + y0;

                float dist_sqr = x1 * x1 + y1 * y1;

                if (dist_sqr > r_max_quad)
                    break;
                ++n;
            }
        }
    }
}

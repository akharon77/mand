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

void CalculateMandelbrotNoOptimizations(MandConfig *config)
{
    int32_t *cnt_arr = config->cnt_arr;

    float base_x = config->base_x,
          base_y = config->base_y;

    TransfromCoord(config, &base_x, &base_y);

    float delta_x = 1,
          delta_y = 1;

    TransformCoordScale(config, &delta_x, &delta_y);

    int32_t width  = config->width,
            height = config->heigh;

    float r_max_sqr = config->r_max * config->r_max;

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
                float x1_sqr   =     x1 * x1;
                float y1_sqr   =     y1 * y1;

                float dist_sqr = x1_sqr + y1 * y1;

                if (dist_sqr > r_max_sqr)
                    break;

                float dbl_x1y1 = 2 * x1 * y1;

                x1 = x1_sqr - y1_sqr + x0;
                y1 = dbl_x1y1 + y0;

                ++n;
            }

            cnt_arr[yi * width + xi] = n;
        }
    }
}

void CalculateMandelbrotAVX512(MandConfig *config)
{
    int32_t *cnt_arr = config->cnt_arr;

    float base_x = config->base_x,
          base_y = config->base_y;

    TransfromCoord(config, &base_x, &base_y);

    float delta_x = 1,
          delta_y = 1;

    TransformCoordScale(config, &delta_x, &delta_y);

    int32_t width  = config->width,
            height = config->heigh;

    // float r_max_sqr = config->r_max * config->r_max;
    __m512 r_max     = _mm512_set1_ps(config->r_max);
    __m512 r_max_sqr = _mm512_mul_ps(r_max, r_max);

    __m512 it16 = _mm512_set_ps(15, 14, 13, 12, 11, 10,  9,  8,
                                 7,  6,  5,  4,  3,  2,  1,  0);

    __m512 delta_x_it16 = _mm512_mul_ps(__m512_set1_ps(delta_x), it16);
    __m512 delta_y_unif =               __m512_set1_ps(delta_y)       ;
    __m512 delta_x_unif = _mm512_mul_ps(__m512_set1_ps(delta_x),
                                        __m512_set1_ps(16));

    // float y0 = base_y;
    __m512 y0 = _mm512_set1_ps(base_y);

    for (int32_t yi = 0; yi < height; ++yi, y0 = _mm512_add_ps(y0, delta_y_unif))
    {
        // float x0 = base_x;
        __m512 x0 = _mm512_add_ps(_mm512_set1_ps(base_x), delta_x_it16);

        for (int32_t xi = 0; xi < width; xi += 16, x0 = _mm512_add_ps(x0, delta_x_unif))
        {

            // int32_t n = 0;
            __m512i n = _mm512i_set1_epi32(0);

            // float x1 = x0,
            //       y1 = y0;

            __m512 x1 = x0;
            __m512 y1 = y0;

            for (int32_t i = 0; i < N_MAX; ++i)
            {
                // x1_sqr   = x1 * x1;
                __m512 x1_sqr = _mm512_mul_ps(x1, x1);

                // y1_sqr   = y1 * y1;
                __m512 y1_sqr = _mm512_mul_ps(y1, y1);

                // float dist_sqr = x1 * x1 + y1 * y1;
                __m512 dist_sqr = _mm512_add_ps(x1_sqr, y1_sqr);

                // CHECK AND BREAK
                __mmask16 mask  = _mm512_cmp_ps_mask(dist_sqr, r_max_sqr, _CMP_LT_OS);
                if (!mask)
                    break;

                __m512i d_cnt = _mm512_broadcastmw_epi32(mask);
                        n     = _mm512_add_ps(n, d_cnt);

                // dbl_x1y1 = 2 * x1 * y1;
                __m512 x1y1     = _mm512_mul_ps(x1,   y1  );
                __m512 dbl_x1y1 = _mm512_add_ps(x1y1, x1y1);

                // x1 = x1_sqr - y1_sqr + x0;
                x1 = _mm512_add_ps(_mm512_sub_ps(x1_sqr, y1_sqr), x0);

                // y1 = dbl_x1y1 + y0;
                y1 = _mm512_add_ps(dbl_x1y1,                      y0);
            }

            // cnt_arr[yi * width + xi] = n;
            _mm512_storeu_epi32(cnt_arr + yi * width + xi, n);
        }
    }
}

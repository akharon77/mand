#include <immintrin.h>

#include "calc.h"

void MandTransfScale(MandConfig *conf, float k)
{
    conf->base_x  += conf->delta_x * conf->width  / 2 * (1 - k);
    conf->base_y  += conf->delta_y * conf->height / 2 * (1 - k);

    conf->delta_x *= k;
    conf->delta_y *= k;

    if (conf->scale * k < 1 - EPS)
        conf->scale *= k;
}

void MandCalcNoOpts(MandConfig *conf)
{
    int32_t *cnt_arr = conf->cnt_arr;

    float    base_x  = conf->base_x,
             base_y  = conf->base_y;

    float    delta_x = conf->delta_x,
             delta_y = conf->delta_y;

    int32_t  width   = conf->width,
             height  = conf->height;

    float r_max_sqr = conf->r_max * conf->r_max;

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
                float x1_sqr   =          x1 * x1;
                float y1_sqr   =          y1 * y1;

                float dist_sqr = x1_sqr + y1 * y1;

                if (dist_sqr > r_max_sqr + EPS)
                    break;

                float dbl_x1y1 =      2 * x1 * y1;

                x1 = x1_sqr - y1_sqr + x0;
                y1 = dbl_x1y1        + y0;

                ++n;
            }

            cnt_arr[yi * width + xi] = n;
        }
    }
}

void MandCalcAVX512(MandConfig *conf)
{
    int32_t *cnt_arr = conf->cnt_arr;

    float    base_x  = conf->base_x,
             base_y  = conf->base_y;

    float    delta_x = conf->delta_x,
             delta_y = conf->delta_y;

    int32_t  width   = conf->width,
             height  = conf->height;

    // float r_max_sqr = config->r_max * config->r_max;
    __m512 r_max_sqr = _mm512_set1_ps(conf->r_max * conf->r_max);
           r_max_sqr = _mm512_sub_ps(r_max_sqr, _mm512_set1_ps(EPS));

    __m512 it16 = _mm512_set_ps(15, 14, 13, 12, 11, 10,  9,  8,
                                 7,  6,  5,  4,  3,  2,  1,  0);

    __m512 delta_x_it16 = _mm512_mul_ps(_mm512_set1_ps(delta_x), it16);
    __m512 delta_y_unif =               _mm512_set1_ps(delta_y)       ;
    __m512 delta_x_unif = _mm512_mul_ps(_mm512_set1_ps(delta_x),
                                        _mm512_set1_ps(16));

    // float y0 = base_y;
    __m512 y0 = _mm512_set1_ps(base_y);

    for (int32_t yi = 0; yi < height; ++yi, y0 = _mm512_add_ps(y0, delta_y_unif))
    {
        // float x0 = base_x;
        __m512 x0 = _mm512_add_ps(_mm512_set1_ps(base_x), delta_x_it16);

        for (int32_t xi = 0; xi < width; xi += 16, x0 = _mm512_add_ps(x0, delta_x_unif))
        {

            // int32_t n = 0;
            __m512i n = _mm512_set1_epi32(N_MAX);

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

                __m512i d_cnt = _mm512_movm_epi32(mask);
                        n     = _mm512_add_epi32(n, d_cnt);

                // dbl_x1y1 = 2 * x1 * y1;
                __m512 x1y1     = _mm512_mul_ps(x1,   y1  );
                __m512 dbl_x1y1 = _mm512_add_ps(x1y1, x1y1);

                // x1 = x1_sqr - y1_sqr + x0;
                x1 = _mm512_add_ps(_mm512_sub_ps(x1_sqr, y1_sqr), x0);

                // y1 = dbl_x1y1 + y0;
                y1 = _mm512_add_ps(dbl_x1y1,                      y0);
            }

            // cnt_arr[yi * width + xi] = n;
            __m512i store_cnt = _mm512_set1_epi32(N_MAX);
                    store_cnt = _mm512_sub_epi32(store_cnt, n);

            _mm512_storeu_si512(cnt_arr + yi * width + xi, store_cnt);
        }
    }
}


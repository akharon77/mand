
#include "mand.h"

void MandConfigCtor(MandConfig *conf, int32_t width, int32_t height)
{
    conf->cnt_arr      = calloc(width * height, sizeof(int32_t));

    conf->base_x       = DEF_BASE_X;
    conf->base_y       = DEF_BASE_Y;

    conf->width        = width;
    conf->height       = height;

    conf->scale        = DEF_SCALE;
    conf->x_transf_rad = DEF_TRANSF_RAD;
    conf->y_transf_rad = DEF_TRANSF_RAD;

    conf->r_max        = DEF_R_MAX;
}

void MandConfigDtor(MandConfig *conf)
{
    free(conf->cnt_arr);

    for (int32_t i = 0, uint32_t *ptr = conf; i < sizeof(MandConfig) / sizeof(uint32_t); ++i, ++ptr)
        *ptr = MAGIC;
}


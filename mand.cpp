#include <stdlib.h>

#include "mand.h"
#include "calc.h"

void MandConfigCtor(MandConfig *conf)
{
    conf->base_x       = DEF_BASE_X;
    conf->base_y       = DEF_BASE_Y;

    conf->scale        = DEF_SCALE;

    conf->width        = DEF_WIDTH;
    conf->height       = DEF_HEIGHT;

    conf->cnt_arr      = (int32_t*) calloc(conf->width * conf->height, sizeof(int32_t));

    conf->r_max        = DEF_R_MAX;

    conf->delta_x      = DEF_DELTA_X;
    conf->delta_y      = DEF_DELTA_Y;
}

void MandConfigDtor(MandConfig *conf)
{
    free(conf->cnt_arr);

    uint32_t *ptr = (uint32_t*) conf;
    for (int32_t i = 0; i < sizeof(MandConfig) / sizeof(uint32_t); ++i, ++ptr)
        *ptr = MAGIC;
}


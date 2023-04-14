#ifndef MAND_H
#define MAND_H

#include <stdint.h>

const int32_t DEF_WIDTH   = 800;
const int32_t DEF_HEIGHT  = 800;

const float  DEF_BASE_X     = -1.85;
const float  DEF_BASE_Y     = -0.8;

const float  DEF_SCALE      = 0.2;

const float  DEF_DELTA_X    = 0.002;
const float  DEF_DELTA_Y    = 0.002;

const float  DEF_R_MAX      = 10;

const uint32_t MAGIC = 0xDEADBEEF;

struct MandConfig
{
    int32_t *cnt_arr;

    float    base_x;
    float    base_y;

    int32_t  width;
    int32_t  height;

    float    scale;

    float    delta_x;
    float    delta_y;

    float    r_max;
};

void MandConfigCtor (MandConfig *conf);
void MandConfigDtor (MandConfig *conf);

#endif  // MAND_H

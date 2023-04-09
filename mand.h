#ifndef MAND_H
#define MAND_H

const float EPS   = 1e-6;
const float N_MAX = 255;

struct MandConfig
{
    int32_t *cnt_arr;

    int32_t base_x;
    int32_t base_y;

    int32_t width;
    int32_t height;

    float scale;
    float x_transf_rad;
    float y_transf_rad;

    float r_max;
};

void TransfCoordScale (const MandConfig *config, float *x, float *y);
void TransfCoord      (const MandConfig *config, float *x, float *y);

void CalcMandNoOpts   (MandConfig *config);
void CalcMandAVX512   (MandConfig *config);

void GetMandImage     (const MandConfig *config, sf::Image *img);

#endif  // MAND_H

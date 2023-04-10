#ifndef CALC_H
#define CALC_H

#include <stdint.h>

#include "mand.h"

const float EPS   = 1e-6;
const float N_MAX = 255;

void MandTransfCoordScale (const MandConfig *conf, float *x, float *y);
void MandTransfCoord      (const MandConfig *conf, float *x, float *y);

void MandCalcNoOpts       (MandConfig *config);
void MandCalcAVX512       (MandConfig *config);

#endif  // CALC_H

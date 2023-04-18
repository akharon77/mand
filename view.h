#ifndef VIEW_H
#define VIEW_H

#include <SFML/Graphics.hpp>

#include "mand.h"

const int32_t MAX_FRAME_CNT = 1000;

#ifdef _MEASURE
const int32_t ACCURANCY     = 100;
#else
const int32_t ACCURANCY     = 1;
#endif

const float   ACC_COORD     = 0.3;
const float   ACC_SCALE     = 0.03;
const float   K_DEC_VEL     = 0.3;
const float   K_DEC_SCALE   = 0.2;

void  MandGetImage (const MandConfig *conf, sf::Image *img);
void  MandRun      (      MandConfig *conf);

float GetFPS       (sf::Clock *clock, int32_t *frame_cnt);

#endif  // VIEW_H

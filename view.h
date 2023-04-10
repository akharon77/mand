#ifndef VIEW_H
#define VIEW_H

#include <SFML/Graphics.hpp>

#include "mand.h"

const int32_t WIDTH       = 800;
const int32_t HEIGHT      = 800;

const float   ACC_COORD   = 3;
const float   ACC_SCALE   = 0.01;
const float   K_DEC_VEL   = 0.2;
const float   K_DEC_SCALE = 0.5;

void MandGetImage(const MandConfig *conf, sf::Image *img);
void MandRun(MandConfig *conf);

#endif  // VIEW_H

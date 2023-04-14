#ifndef VIEW_H
#define VIEW_H

#include <SFML/Graphics.hpp>

#include "mand.h"

const float   ACC_COORD   = 0.1;
const float   ACC_SCALE   = 0.01;
const float   K_DEC_VEL   = 0.3;
const float   K_DEC_SCALE = 0.2;

void MandGetImage(const MandConfig *conf, sf::Image *img);
void MandRun(MandConfig *conf);

#endif  // VIEW_H

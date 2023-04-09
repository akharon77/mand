#ifndef VIEW_H
#define VIEW_H

#include <SFML/Graphics.hpp>

#include "mand.h"

const int32_t WIDTH  = 800;
const int32_t HEIGHT = 800;

void MandGetImage(const MandConfig *conf, sf::Image *img);
void MandRun(MandConfig *conf);

#endif  // VIEW_H

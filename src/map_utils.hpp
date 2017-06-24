/**
 * Copyright (C) Paul Wortmann, PhysHex Games, www.physhexgames.com
 * This file is part of "Random 2D Map Generator"
 *
 * "Random 2D Map Generator" is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 only.
 *
 * "Random 2D Map Generator" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with "Random 2D Map Generator" If not, see <http://www.gnu.org/licenses/>.
 *
 * @author  Paul Wortmann
 * @email   physhex@gmail.com
 * @website www.physhexgames.com
 * @license GPL V2
 * @date 2017-06-05
 */

#ifndef MAP_UTILS_HPP
#define MAP_UTILS_HPP

#include "map_data_types.hpp"

void mapCheck(sGenerationData &_data);
int16_t mapGenerator_tilesRoom(sGenerationData &_data, uint32_t _tile);
void mapGenerator_findNeighborRooms(sGenerationData &_data);
void mapGenerator_connectRooms_direct(sGenerationData &_data);
void mapGenerator_connectRooms_90d(sGenerationData &_data);

#endif // MAP_UTILS_HPP

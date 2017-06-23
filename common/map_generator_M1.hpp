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

#ifndef MAPGENERATOR_M1_HPP
#define MAPGENERATOR_M1_HPP

#include "map_data_types.hpp"
#include "map_floodfill.hpp"
#include "map_utils.hpp"

bool isWallTile(sGenerationData &_data, int32_t _tile);
bool map_gen_maze_check_tile(sGenerationData &_data, int32_t _tile, eDirection _directionBias);
void map_gen_maze(sGenerationData &_data, uint32_t _tile, eDirection _directionBias);
bool add_room(sGenerationData &_data, sRoom &_room);
bool connect_room(sGenerationData &_data, sRoom &_room);
void mapGenerator_M1(sGenerationData &_data);

#endif // MAPGENERATOR_M1_HPP

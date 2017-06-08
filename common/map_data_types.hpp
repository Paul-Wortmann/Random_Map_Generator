
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

#ifndef MAPDATATYPES_HPP
#define MAPDATATYPES_HPP

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#define RMG_VERSION "0.0.1"

#define WALL_WIDTH  3
#define EXITS_COUNT 4

#define DENSITY 0.6f
#define ITTERATIONS 2

#define ROOM_MIN_X 11
#define ROOM_MIN_Y ROOM_MIN_X
#define ROOM_MAX_X 15
#define ROOM_MAX_Y ROOM_MAX_X

#define DIRECTION_BIAS_THRESHOLD 32
#define DIRECTION_BIAS_NONE  0
#define DIRECTION_BIAS_UP    1
#define DIRECTION_BIAS_DOWN  2
#define DIRECTION_BIAS_LEFT  3
#define DIRECTION_BIAS_RIGHT 4

#define AXIS_BIAS_NONE 0
#define AXIS_BIAS_X    1
#define AXIS_BIAS_Y    2

enum class eAlgorithm : uint16_t { AC1 = 0, AD1 = 1, AD2 = 2, AM1 = 3 };
enum class eExporter  : uint16_t { ED1 = 0, EF1 = 1 };
enum class eTile      : uint16_t { FLOOR = 0, WALL = 1, LIQUID = 2, VOID = 3, PATH = 4 };
//enum eObject    : uint16_t { NONE = 0, EXIT = 1, CHEST = 2 };

struct sFillData
{
    eTile tile = eTile::FLOOR;
    bool  *valid = nullptr;
};

struct sGenerationData
{
    std::string version = RMG_VERSION;
    uint64_t seed = 0;
    uint16_t x = 100;
    uint16_t y = 100;
    uint32_t mapSize = x * y;
    eAlgorithm algorithm = eAlgorithm::AC1;
    eExporter exporter = eExporter::EF1;
    std::string fileName = "default.txt";
    eTile *tile = nullptr;
//    eObject *object = nullptr;
};

struct sMapNode
{
    sGenerationData data = {};
    sMapNode* left = nullptr;
    sMapNode* right = nullptr;
};

struct sRoom
{
    bool valid;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
};

#endif // MAPDATATYPES_HPP

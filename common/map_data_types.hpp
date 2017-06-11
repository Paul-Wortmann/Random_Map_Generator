
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

enum class eAlgorithm : uint16_t { AC1 = 0, AD1 = 1, AD2 = 2, AM1 = 3 };
enum class eExporter  : uint16_t { ED1 = 0, EF1 = 1 };
enum class eTile      : uint16_t { FLOOR = 0, WALL = 1, LIQUID = 2, VOID = 3, PATH = 4 };
enum class eDirection : uint16_t { NONE = 0, UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4 };
//enum eObject    : uint16_t { NONE = 0, EXIT = 1, CHEST = 2 };

struct sRoomGenData
{
    eDirection direction = eDirection::NONE;
    bool end_node = false;
    uint16_t sx = 0; // start x
    uint16_t ex = 0; // end x
    uint16_t sy = 0; // start y
    uint16_t ey = 0; // end y
    sRoomGenData *parent = nullptr;
    sRoomGenData *left = nullptr;
    sRoomGenData *right = nullptr;
};

struct sFillData
{
    eTile tile = eTile::FLOOR;
    bool  *valid = nullptr;
};

struct sRoomData
{
    uint16_t ID = 0;
    bool valid = false;
    uint32_t position = 0;
    uint16_t w = 0;
    uint16_t h = 0;
    bool connection[4] = {false};
};

struct sGenerationData
{
    uint16_t wallWidth = 3;
    uint16_t exitCount = 4;

    float density = 0.6f;
    uint16_t itterations = 2;

    uint16_t roomMin_x = 11;
    uint16_t roomMin_y = roomMin_x;
    uint16_t roomMax_x = 15;
    uint16_t roomMax_y = roomMax_x;

    uint16_t directionBias_Threshold = 32;
    uint16_t directionBias_none  = 0;
    uint16_t directionBias_up    = 1;
    uint16_t directionBias_down  = 2;
    uint16_t directionBias_left  = 3;
    uint16_t directionBias_right = 4;

    uint16_t axisBias_none = 0;
    uint16_t axisBias_x    = 1;
    uint16_t axisBias_y    = 2;

    std::string version = RMG_VERSION;
    uint64_t seed = 0;
    uint16_t x = 50;
    uint16_t y = 50;
    uint32_t mapSize = x * y;
    eAlgorithm algorithm = eAlgorithm::AC1;
    eExporter exporter = eExporter::EF1;
    std::string fileExport = "default.txt";
    std::string fileSettings = "";
    uint16_t roomCount = 0;
    sRoomData *room = nullptr;
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


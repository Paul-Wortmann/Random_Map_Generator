
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
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <random>

#define RMG_VERSION "0.0.1"

enum class eError     : uint16_t { NONE = 0, ALGORITHM = 1 };
enum class eAlgorithm : uint16_t { AC1 = 0, AD1 = 1, AD2 = 2, AD3 = 3, AM1 = 4 };
enum class eExporter  : uint16_t { ED1 = 0, ER1 = 1, EF1 = 2 };
enum class eTile      : uint16_t { NONE = 0, FLOOR = 1, WALL = 2, LIQUID = 3, VOID = 4, PATH = 5, DOOR = 6, NOGEN = 7, GEN = 8 };
enum class eDirection : uint16_t { NONE = 0, UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4 };
enum class eAxis      : uint16_t { NONE = 0, X = 1, Y = 2, Z = 3 };
enum class eRoomShape : uint16_t { NONE = 0, SQUARE = 1, CIRCLE = 2};

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

struct sRoomID
{
    int16_t ID = -1;
    eDirection direction = eDirection::NONE;
};

struct sRoomData
{
    uint16_t ID = 0;
    bool valid = false;
    uint32_t position = 0;
    uint16_t w = 0;
    uint16_t h = 0;
    eRoomShape shape = eRoomShape::NONE;
    uint16_t connectionCount = 0;
    sRoomID connection[4] = {};
};

struct sExitData
{
    uint16_t ID = 0;
    bool valid = false;
    uint32_t position = 0;
};

struct sGenerationData
{
    sGenerationData(void) {};
    ~sGenerationData(void) = default;
    sGenerationData(const sGenerationData&) = default;
    sGenerationData& operator=(const sGenerationData& _data) {if (this == &_data) return *this; return *this;};

    uint64_t rmg_rand(void) {return rand();};
    void rmg_rseed(uint64_t _seed) {srand((uint32_t)_seed);};

    uint16_t wallWidth = 3;
    float density = 0.625f;
    uint16_t iterations = 2;

    uint16_t roomMin_x = 11;
    uint16_t roomMin_y = roomMin_x;
    uint16_t roomMax_x = 15;
    uint16_t roomMax_y = roomMax_x;
    const uint16_t roomMaxConnections = 4;

    uint16_t directionBias_Threshold = 32;
    eDirection directionBias = eDirection::NONE;
    eError error = eError::NONE;

    std::string version = RMG_VERSION;
    uint64_t seed = 0;
    uint16_t x = 100;
    uint16_t y = 100;
    uint32_t tileCount = x * y;
    eAlgorithm algorithm = eAlgorithm::AC1;
    eExporter exporter = eExporter::ED1;
    std::string fileExport = "default.txt";
    std::string fileSettings = "settings.txt";
    uint16_t roomCount = 0;
    sRoomData *room = nullptr;
    uint16_t exitCount = 0;
    sExitData *exit = nullptr;
    eTile *tile = nullptr;
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


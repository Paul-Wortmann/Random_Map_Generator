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

#include "map_generator_C1.hpp"

void mapGenerator_C1(sGenerationData &_data)
{
    _data.exitCount = 0;
    uint32_t mapSize = _data.x * _data.y;
    _data.tile = new eTile[mapSize];
    for (uint16_t i = 0; i < _data.y; i++)
        for (uint16_t j = 0; j < _data.x; j++)
            _data.tile[(i * _data.x) + j] = ((i == 0)||(i == _data.y-1)||(j == 0)||(j == _data.x-1)) ? eTile::WALL : eTile::FLOOR;
    for (int i = 0; i < (mapSize * _data.density); i++)
        _data.tile[_data.rmg_rand() % mapSize] = eTile::WALL;
    for(int i = -1; i < 2; i++)
        for(int j = -1; j < 2; j++)
            _data.tile[(((_data.y / 2) + i) * _data.x) + ((_data.x / 2) + j)] = eTile::FLOOR;
    for (uint16_t iteration = 0; iteration < _data.iterations; iteration++)
    {
        for (uint16_t i = 1; i < _data.x-1; i++)
        {
            for (uint16_t j = 1; j < _data.y-1; j++)
            {
                uint16_t num_walls = 0;
                if (_data.tile[((i + 0) * _data.x) + (j - 1)] == eTile::WALL) num_walls++;
                if (_data.tile[((i + 0) * _data.x) + (j + 1)] == eTile::WALL) num_walls++;
                if (_data.tile[((i - 1) * _data.x) + (j + 0)] == eTile::WALL) num_walls++;
                if (_data.tile[((i - 1) * _data.x) + (j - 1)] == eTile::WALL) num_walls++;
                if (_data.tile[((i - 1) * _data.x) + (j + 1)] == eTile::WALL) num_walls++;
                if (_data.tile[((i + 1) * _data.x) + (j + 0)] == eTile::WALL) num_walls++;
                if (_data.tile[((i + 1) * _data.x) + (j - 1)] == eTile::WALL) num_walls++;
                if (_data.tile[((i + 1) * _data.x) + (j + 1)] == eTile::WALL) num_walls++;
                if ((_data.tile[(i*_data.x)+j] == eTile::WALL) && (num_walls > 3))
                    _data.tile[(i*_data.x)+j] = eTile::WALL;
                else if ((_data.tile[(i*_data.x)+j] == eTile::FLOOR) && (num_walls > 4))
                    _data.tile[(i*_data.x)+j] = eTile::WALL;
                else
                    _data.tile[(i*_data.x)+j] = eTile::FLOOR;
            }
        }
    }
    mapCheck(_data);
    // setup fill data
    sFillData fillData;
    fillData.valid = new bool[mapSize];
    for (uint32_t i = 0; i < mapSize; i++)
        fillData.valid[i] = false;
    uint32_t startTile = ((_data.y / 2) * _data.x) + (_data.x / 2);
    fillData.tile = _data.tile[startTile];
    fillData.valid[startTile] = true;
    mapFloodFill(_data, fillData, startTile);
    for (uint32_t i = 0; i < mapSize; i++)
        _data.tile[i] = (fillData.valid[i]) ? eTile::FLOOR : eTile::WALL;
    // clean up
    if (fillData.valid != nullptr)
    {
        delete [] fillData.valid;
        fillData.valid = nullptr;
    }
}

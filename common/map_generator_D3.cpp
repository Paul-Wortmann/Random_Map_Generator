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

#include "map_generator_D3.hpp"

void mapGenerator_D3_generateRooms(sGenerationData &_data)
{
    _data.roomCount = ((_data.x / _data.roomMax_x) * (_data.y / _data.roomMax_y)) / 2;
    //std::cout << "roomCount: " << ((_data.x / _data.roomMax_x) * (_data.y / _data.roomMax_y)) /2 << std::endl;
    uint16_t roomPlacementAttempts = 4;
    _data.room = new sRoomData[_data.roomCount];
    uint16_t t_x = (_data.rmg_rand() % (_data.x - 2)) + 1;
    uint16_t t_y = (_data.rmg_rand() % (_data.y - 2)) + 1;
    uint16_t t_w = (_data.roomMin_x + _data.rmg_rand() % (_data.roomMax_x - _data.roomMin_x)) / 2;
    _data.room[0].position = (t_y * _data.x) + t_x;
    _data.room[0].w = t_w;
    _data.room[0].h = t_w;
    _data.room[0].valid = true;
    for (uint16_t i = 1; i < _data.roomCount; i++)
    {
        bool placeFound = false;
        for (uint16_t j = 0; j < roomPlacementAttempts; j++)
        {
            t_x = (_data.rmg_rand() % (_data.x - 1 - _data.roomMax_x/2)) + (_data.roomMax_x/2) + 1;
            t_y = (_data.rmg_rand() % (_data.y - 1 - _data.roomMax_x/2)) + (_data.roomMax_x/2) + 1;
            t_w = (_data.roomMin_x + _data.rmg_rand() % (_data.roomMax_x - _data.roomMin_x)) / 2;
            for (uint16_t k = 0; k <= i; k++)
            {
                if (_data.room[k].valid)
                {
                    uint16_t t_rx = _data.room[k].position % _data.x;
                    uint16_t t_ry = _data.room[k].position / _data.x;
                    uint16_t t_rw = _data.room[k].w;
                    int16_t dx = t_x - t_rx;
                    int16_t dy = t_y - t_ry;
                    int16_t dr = t_w + t_rw;
                    placeFound = ((dx * dx) + (dy * dy) > (dr * dr));
                    //std::cout << "collision detected" << std::endl;
                }
                if (placeFound)
                    k = i+1;
            }
            if (placeFound)
            {
                _data.room[i].position = (t_y * _data.x) + t_x;
                _data.room[i].w = t_w;
                _data.room[i].h = t_w;
                placeFound = true;
                j = roomPlacementAttempts;
            }
        }
        _data.room[i].valid = placeFound;
    }
}

void mapGenerator_D3_fillRooms(sGenerationData &_data)
{
    for (uint32_t i = 0; i < _data.roomCount; i++)
    {
        if (_data.room[i].valid)
            for (uint32_t j = 0; j < _data.mapSize; j++)
            {
                uint16_t point_x = j % _data.x;
                uint16_t point_y =  j / _data.x;
                uint16_t radius = _data.room[i].w;
                uint16_t circle_x = _data.room[i].position % _data.x;
                uint16_t circle_y = _data.room[i].position / _data.x;
                if (((point_x - circle_x) * (point_x - circle_x)) + ((point_y - circle_y) * (point_y - circle_y)) < (radius * radius))
                    _data.tile[j] = eTile::FLOOR;
            }
    }
}

void mapGenerator_D3(sGenerationData &_data)
{
    _data.mapSize = _data.x * _data.y;
    _data.tile = new eTile[_data.mapSize];
    for (uint16_t i = 0; i < _data.mapSize; i++)
        _data.tile[i] = eTile::WALL;
    mapGenerator_D3_generateRooms(_data);
    mapGenerator_D3_fillRooms(_data);
}

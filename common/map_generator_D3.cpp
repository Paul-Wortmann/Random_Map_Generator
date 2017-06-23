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
    uint16_t t_roomCount = ((_data.x / _data.roomMin_x) * (_data.y / _data.roomMin_y)) * 2;
    sRoomData *t_room = new sRoomData[t_roomCount];
    for (uint16_t i = 0; i < t_roomCount; i++)
    {
        uint16_t t_r = (_data.roomMin_x + _data.rmg_rand() % (_data.roomMax_x - _data.roomMin_x)) / 2;
        uint16_t t_x = t_r + (_data.rmg_rand() % (_data.x - t_r * 2));
        uint16_t t_y = t_r + (_data.rmg_rand() % (_data.y - t_r * 2));
        t_room[i].position = (t_y * _data.x) + t_x;
        t_room[i].w = t_r;
        t_room[i].h = t_room[i].w;
        t_room[i].valid = true;
    }
    for (uint16_t i = 0; i < t_roomCount; i++)
    {
        for (uint16_t j = i+1; j < t_roomCount; j++)
        {
            if ((j < t_roomCount) && (t_room[i].valid) && (t_room[j].valid))
            {
                uint16_t t_x1 = t_room[i].position % _data.x;
                uint16_t t_y1 = t_room[i].position / _data.x;
                uint16_t t_r1 = t_room[i].w + 1;
                uint16_t t_x2 = t_room[j].position % _data.x;
                uint16_t t_y2 = t_room[j].position / _data.x;
                uint16_t t_r2 = t_room[j].w + 1;
                bool valid = (((t_x1 - t_x2) * (t_x1 - t_x2)) + ((t_y1 - t_y2) * (t_y1 - t_y2)) > ((t_r1 + t_r2) * (t_r1 + t_r2)));
                t_room[j].valid = valid;
            }
        }
    }
    _data.roomCount = 0;
    for (uint16_t i = 0; i < t_roomCount; i++)
        if (t_room[i].valid)
            _data.roomCount++;
    _data.room = new sRoomData[_data.roomCount];
    uint16_t roomCount = 0;
    for (uint16_t i = 0; i < t_roomCount; i++)
    {
        if (t_room[i].valid)
        {
            _data.room[roomCount].position = t_room[i].position;
            _data.room[roomCount].w = t_room[i].w;
            _data.room[roomCount].h = t_room[i].h;
            _data.room[roomCount].valid = true;
            roomCount++;
        }
    }
    delete [] t_room;
}

void mapGenerator_D3_fillRooms(sGenerationData &_data)
{
    for (uint32_t i = 0; i < _data.roomCount; i++)
    {
        if (_data.room[i].valid)
            for (uint32_t j = 0; j < _data.mapSize; j++)
            {
                uint16_t point_x = j % _data.x;
                uint16_t point_y = j / _data.x;
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
    _data.exitCount = 0;
    _data.mapSize = _data.x * _data.y;
    _data.tile = new eTile[_data.mapSize];
    for (uint16_t i = 0; i < _data.mapSize; i++)
        _data.tile[i] = eTile::WALL;
    mapGenerator_D3_generateRooms(_data);
    mapGenerator_D3_fillRooms(_data);
    mapGenerator_connectRooms_90d(_data);
    for (uint32_t i = 0; i < _data.mapSize; i++)
    {
        if (_data.tile[i] == eTile::PATH)
            _data.tile[i] = eTile::FLOOR;
    }
}

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

#include "map_generator_D2.hpp"

void mapGenerator_D2_generateRooms(sGenerationData &_data)
{
    uint32_t max_r = (uint32_t)sqrt((float)(_data.roomMax_x*_data.roomMax_x)+(_data.roomMax_y*_data.roomMax_y));
    _data.roomCount = (_data.mapSize) / (_data.roomMin_x*_data.roomMin_y);
    _data.room = new sRoomData[_data.roomCount];
    for (uint32_t i = 0; i < _data.roomCount; i++)
    {
        _data.room[i].valid = true;
        _data.room[i].w = _data.roomMin_x + _data.rmg_rand() % (_data.roomMax_x-_data.roomMin_x);
        _data.room[i].h = _data.roomMin_y + _data.rmg_rand() % (_data.roomMax_y-_data.roomMin_y);
        uint16_t t_x = (_data.room[i].w/2) + _data.rmg_rand() % (uint32_t)(_data.x - _data.room[i].w)-2;
        uint16_t t_y = (_data.room[i].h/2) + _data.rmg_rand() % (uint32_t)(_data.y - _data.room[i].h)-2;
        _data.room[i].position = (t_y * _data.x) + t_x;

    }
    for (uint32_t i = 0; i < _data.roomCount; i++)
    {
        for (uint32_t j = 0; j < _data.roomCount; j++)
        {
            if ((_data.room[i].valid && _data.room[j].valid)&&(i!=j))
            {
                uint16_t t_xi = _data.room[i].position % _data.x;
                uint16_t t_yi = _data.room[i].position / _data.x;
                uint16_t t_xj = _data.room[j].position % _data.x;
                uint16_t t_yj = _data.room[j].position / _data.x;
                if (max_r > (uint32_t)sqrt((float)((t_xi-t_xj)*(t_xi-t_xj))+((t_yi-t_yj)*(t_yi-t_yj))))
                    _data.room[j].valid = false;
            }
        }
    }
    for (uint32_t i = 0; i < _data.roomCount; i++)
    {
        if (_data.room[i].valid)
        {
            for (uint32_t j = 1; j < _data.room[i].w; j++)
            {
                for (uint32_t k = 1; k < _data.room[i].h; k++)
                {
                    uint16_t t_xi = _data.room[i].position % _data.x;
                    uint16_t t_yi = _data.room[i].position / _data.x;
                    uint32_t tilePos = ((t_yi-(_data.room[i].h/2)+k) * _data.x) + t_xi-(_data.room[i].w/2)+j;
                    if (tilePos < _data.mapSize)
                        _data.tile[tilePos] = eTile::FLOOR;
                }
            }
        }
    }
}

void mapGenerator_D2_fillRooms(sGenerationData &_data)
{
    for (uint16_t i = 0; i < _data.y; i++)
    {
        for (uint16_t j = 0; j < _data.x; j++)
        {
            if ((i == 0)||(i == _data.y-1)||(j == 0)||(j == _data.x-1))
                _data.tile[(i * _data.x) + j] = eTile::WALL;
        }
    }
    for (uint32_t i = 0; i < _data.mapSize; i++)
    {
        if (_data.tile[i] == eTile::PATH)
            _data.tile[i] = eTile::FLOOR;
    }

    // setup fill data
    uint32_t startTile = 0;
    for (uint16_t i = 0; i < _data.mapSize; i++)
    {
        if (_data.tile[i] == eTile::FLOOR)
            startTile = i;
    }
    sFillData fillData;
    fillData.valid = new bool[_data.mapSize];
    for (uint32_t i = 0; i < _data.mapSize; i++)
        fillData.valid[i] = false;
    fillData.tile = _data.tile[startTile];
    fillData.valid[startTile] = true;
    mapFloodFill(_data, fillData, startTile);
    for (uint32_t i = 0; i < _data.mapSize; i++)
        _data.tile[i] = (fillData.valid[i]) ? eTile::FLOOR : eTile::WALL;
    // clean up
    if (fillData.valid != nullptr)
    {
        delete [] fillData.valid;
        fillData.valid = nullptr;
    }
}

void mapGenerator_D2(sGenerationData &_data)
{
    _data.exitCount = 0;
    _data.mapSize = _data.x * _data.y;
    _data.tile = new eTile[_data.mapSize];
    for (uint16_t i = 0; i < _data.mapSize; i++)
        _data.tile[i] = eTile::WALL;
    mapGenerator_D2_generateRooms(_data);
    mapGenerator_connectRooms_90d(_data);
    mapGenerator_D2_fillRooms(_data);
}

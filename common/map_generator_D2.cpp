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

void mapGenerator_D2(sGenerationData &_data)
{
    _data.mapSize = _data.x * _data.y;
    _data.tile = new eTile[_data.mapSize];
    for (uint32_t i = 0; i < _data.mapSize; i++)
        _data.tile[i] = eTile::WALL;
    uint32_t max_r = (uint32_t)sqrt((float)(_data.roomMax_x*_data.roomMax_x)+(_data.roomMax_y*_data.roomMax_y));
    uint32_t max_rooms = (_data.mapSize) / (_data.roomMin_x*_data.roomMin_y);
    sRoom* room = new sRoom[max_rooms];
    for (uint32_t i = 0; i < max_rooms; i++)
    {
        room[i].valid = true;
        room[i].w = _data.roomMin_x + _data.rmg_rand() % (_data.roomMax_x-_data.roomMin_x);
        room[i].h = _data.roomMin_y + _data.rmg_rand() % (_data.roomMax_y-_data.roomMin_y);
        room[i].x = (room[i].w/2) + _data.rmg_rand() % (uint32_t)(_data.x - room[i].w)-2;
        room[i].y = (room[i].h/2) + _data.rmg_rand() % (uint32_t)(_data.y - room[i].h)-2;
    }
    for (uint32_t i = 0; i < max_rooms; i++)
    {
        for (uint32_t j = 0; j < max_rooms; j++)
        {
            if ((room[i].valid && room[j].valid)&&(i!=j))
            {
                if (max_r > (uint32_t)sqrt((float)((room[i].x-room[j].x)*(room[i].x-room[j].x))+((room[i].y-room[j].y)*(room[i].y-room[j].y))))
                    room[j].valid = false;
            }
        }
    }
    for (uint32_t i = 0; i < max_rooms; i++)
    {
        if (room[i].valid)
        {
            for (uint32_t j = 1; j < room[i].w; j++)
            {
                for (uint32_t k = 1; k < room[i].h; k++)
                {
                    uint32_t tilePos = ((room[i].y-(room[i].h/2)+k) * _data.x) + room[i].x-(room[i].w/2)+j;
                    if (tilePos < _data.mapSize)
                        _data.tile[tilePos] = eTile::FLOOR;
                }
            }
        }
    }
    for (uint32_t i = 0; i < max_rooms; i++)
    {
        if (room[i].valid)
        {
            for (uint32_t j = 0; j < max_rooms; j++)
            {
                bool path_found = false;
                if ((room[j].valid)&&(i != j))
                {
                    if (!path_found)
                    {
                        eTile previous_tile = eTile::FLOOR;
                        uint32_t transitions = 0;
                        uint32_t current_x = room[i].x;
                        uint32_t current_y = room[i].y;
                        for (current_x = room[i].x; current_x != room[j].x;)
                        {
                            if (previous_tile != _data.tile[(current_y * _data.x) + current_x])
                                transitions++;
                            if (_data.tile[(current_y * _data.x) + current_x] == eTile::PATH)
                                transitions++;
                            previous_tile = _data.tile[(current_y * _data.x) + current_x];
                            if (room[i].x < room[j].x) current_x++;
                                    else current_x--;
                        }
                        for (current_y = room[i].y; current_y != room[j].y;)
                        {
                            if (previous_tile != _data.tile[(current_y * _data.x) + current_x])
                                transitions++;
                            if (_data.tile[(current_y * _data.x) + current_x] == eTile::PATH)
                                transitions++;
                            previous_tile = _data.tile[(current_y * _data.x) + current_x];
                            if (room[i].y < room[j].y) current_y++;
                                    else current_y--;
                        }
                        if ((!path_found)&&(transitions == 2))
                        {
                            path_found = true;
                            current_x = room[i].x;
                            current_y = room[i].y;
                            for (current_x = room[i].x; current_x != room[j].x;)
                            {
                                _data.tile[(current_y * _data.x) + current_x] = eTile::PATH;
                                if (room[i].x < room[j].x) current_x++;
                                        else current_x--;
                            }
                            for (current_y = room[i].y; current_y != room[j].y;)
                            {
                                _data.tile[(current_y * _data.x) + current_x] = eTile::PATH;
                                if (room[i].y < room[j].y) current_y++;
                                        else current_y--;
                            }
                        }
                    }
                    if (!path_found)
                    {
                        eTile previous_tile = eTile::FLOOR;
                        uint32_t transitions = 0;
                        uint32_t current_x = room[i].x;
                        uint32_t current_y = room[i].y;
                        for (current_y = room[i].y; current_y != room[j].y;)
                        {
                            if (previous_tile != _data.tile[(current_y * _data.x) + current_x])
                                transitions++;
                            if (_data.tile[(current_y * _data.x) + current_x] == eTile::PATH)
                                transitions++;
                            previous_tile = _data.tile[(current_y * _data.x) + current_x];
                            if (room[i].y < room[j].y) current_y++;
                                    else current_y--;
                        }
                        for (current_x = room[i].x; current_x != room[j].x;)
                        {
                            if (previous_tile != _data.tile[(current_y * _data.x) + current_x])
                                transitions++;
                            if (_data.tile[(current_y * _data.x) + current_x] == eTile::PATH)
                                transitions++;
                            previous_tile = _data.tile[(current_y * _data.x) + current_x];
                            if (room[i].x < room[j].x) current_x++;
                                    else current_x--;
                        }
                        if ((!path_found)&&(transitions == 2))
                        {
                            path_found = true;
                            current_x = room[i].x;
                            current_y = room[i].y;
                            for (current_y = room[i].y; current_y != room[j].y;)
                            {
                                _data.tile[(current_y * _data.x) + current_x] = eTile::PATH;
                                if (room[i].y < room[j].y) current_y++;
                                        else current_y--;
                            }
                            for (current_x = room[i].x; current_x != room[j].x;)
                            {
                                _data.tile[(current_y * _data.x) + current_x] = eTile::PATH;
                                if (room[i].x < room[j].x) current_x++;
                                        else current_x--;
                            }
                        }
                    }
                }
            }
        }
    }
    for (uint16_t i = 0; i < _data.y; i++)
    {
        for (uint16_t j = 0; j < _data.x; j++)
        {
            if ((i == 0)||(i == _data.y-1)||(j == 0)||(j == _data.x-1))
                _data.tile[(i * _data.x) + j] = eTile::WALL;
        }
    }
	delete [] room;

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

/**
 * Copyright (C) 2017 Paul Wortmann, PhysHex Games, www.physhexgames.com
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

#include "map_utils.hpp"

void mapCheck(sGenerationData &_data)
{
    uint32_t mapTile = 0;
    for (uint16_t k = 0; k < _data.iterations; k++)
    {
        for (uint16_t i = 1; i < _data.y-1; i++)
        {
            for (uint16_t j = 1; j < _data.x-1; j++)
            {
                mapTile = (i * _data.x) + j;
                if (_data.tile[mapTile] == eTile::FLOOR)
                {
                    // Remove single floor tiles causing narrow concave wall structures.
                    // XXX
                    // XOX
                    // OOO
                    //--------------------
                    uint16_t wallCount = 0;
                    if (_data.tile[mapTile+1] == eTile::WALL)
                        wallCount++;
                    if (_data.tile[mapTile-1] == eTile::WALL)
                        wallCount++;
                    if (_data.tile[mapTile+_data.x] == eTile::WALL)
                        wallCount++;
                    if (_data.tile[mapTile-_data.x] == eTile::WALL)
                        wallCount++;
                    if (wallCount >= 3)
                        _data.tile[mapTile] = eTile::WALL;
                }
                if (_data.tile[mapTile] == eTile::WALL)
                {
                    // Remove single horizontal tiles
                    // ???
                    // OXO
                    // ???
                    //--------------------
                    if ((_data.tile[mapTile+1] == eTile::FLOOR)
                     && (_data.tile[mapTile-1] == eTile::FLOOR))
                    {
                        _data.tile[mapTile] = eTile::FLOOR;
                    }
                    // Remove single vertical tiles
                    // ?O?
                    // ?X?
                    // ?O?
                    //--------------------
                    if ((_data.tile[mapTile+_data.x] == eTile::FLOOR)
                     && (_data.tile[mapTile-_data.x] == eTile::FLOOR))
                    {
                        _data.tile[mapTile] = eTile::FLOOR;
                    }
                    // Remove diagonal right tiles
                    // O?X
                    // ?X?
                    // X?O
                    //--------------------
                    if ((_data.tile[mapTile+_data.x+1] == eTile::WALL)
                     && (_data.tile[mapTile+_data.x-1] == eTile::FLOOR)
                     && (_data.tile[mapTile-_data.x+1] == eTile::FLOOR)
                     && (_data.tile[mapTile-_data.x-1] == eTile::WALL))
                    {
                        _data.tile[mapTile+_data.x-1] = eTile::WALL;
                        _data.tile[mapTile-_data.x+1] = eTile::WALL;
                        _data.tile[mapTile+_data.x] = eTile::WALL;
                        _data.tile[mapTile-_data.x] = eTile::WALL;
                        _data.tile[mapTile+1] = eTile::WALL;
                        _data.tile[mapTile-1] = eTile::WALL;
                    }
                    // Remove diagonal left tiles
                    // X?O
                    // ?X?
                    // O?X
                    //--------------------
                    if ((_data.tile[mapTile+_data.x+1] == eTile::FLOOR)
                     && (_data.tile[mapTile+_data.x-1] == eTile::WALL)
                     && (_data.tile[mapTile-_data.x+1] == eTile::WALL)
                     && (_data.tile[mapTile-_data.x-1] == eTile::FLOOR))
                    {
                        _data.tile[mapTile+_data.x+1] = eTile::WALL;
                        _data.tile[mapTile-_data.x-1] = eTile::WALL;
                        _data.tile[mapTile+_data.x] = eTile::WALL;
                        _data.tile[mapTile-_data.x] = eTile::WALL;
                        _data.tile[mapTile+1] = eTile::WALL;
                        _data.tile[mapTile-1] = eTile::WALL;
                    }
                }
            }
        }
    }
}

int16_t mapGenerator_tilesRoom(sGenerationData &_data, uint32_t _tile)
{
    if (_tile >= _data.mapSize)
        return -1;
    if (_data.tile[_tile] != eTile::FLOOR)
        return -1;
    for (uint16_t i = 0; i < _data.roomCount; i++)
    {
        uint16_t tile_x = _tile % _data.x;
        uint16_t tile_y = _tile / _data.x;
        uint16_t room_x = _data.room[i].position % _data.x;
        uint16_t room_y = _data.room[i].position / _data.x;
        if    ((tile_x >= (room_x - (_data.room[i].w/2)))
            && (tile_x <= (room_x + (_data.room[i].w/2)))
            && (tile_y >= (room_y - (_data.room[i].h/2)))
            && (tile_y <= (room_y + (_data.room[i].h/2))))
            return i;
    }
    return -1;
}

void mapGenerator_findNeighborRooms(sGenerationData &_data)
{
    for (uint16_t i = 0; i < _data.roomCount; i++)
    {
        uint16_t pos_x = _data.room[i].position % _data.x;
        uint16_t pos_y = _data.room[i].position / _data.y;
        for (uint16_t j = pos_x; j < _data.x; j++)
        {
            int16_t tile_r2 = mapGenerator_tilesRoom(_data, (pos_y * _data.x) + j);
            if ((tile_r2 >= 0) && (tile_r2 != i))
            {
                _data.room[i].connection[_data.room[i].connectionCount].ID = tile_r2;
                _data.room[i].connection[_data.room[i].connectionCount].direction = eDirection::RIGHT;
                if (_data.room[i].connectionCount < _data.roomMaxConnections)
                    _data.room[i].connectionCount++;
                 j = _data.x;
            }
        }
        for (int16_t j = pos_x; j >= 0; j--)
        {
            int16_t tile_r2 = mapGenerator_tilesRoom(_data, (pos_y * _data.x) + j);
            if ((tile_r2 >= 0) && (tile_r2 != i))
            {
                _data.room[i].connection[_data.room[i].connectionCount].ID = tile_r2;
                _data.room[i].connection[_data.room[i].connectionCount].direction = eDirection::LEFT;
                if (_data.room[i].connectionCount < _data.roomMaxConnections)
                    _data.room[i].connectionCount++;
                j = -1;
            }
        }
        for (uint16_t j = pos_y; j < _data.y; j++)
        {
            int16_t tile_r2 = mapGenerator_tilesRoom(_data, (j * _data.x) + pos_x);
            if ((tile_r2 >= 0) && (tile_r2 != i))
            {
                _data.room[i].connection[_data.room[i].connectionCount].ID = tile_r2;
                _data.room[i].connection[_data.room[i].connectionCount].direction = eDirection::DOWN;
                if (_data.room[i].connectionCount < _data.roomMaxConnections)
                    _data.room[i].connectionCount++;
                 j = _data.y;
            }
        }
        for (int16_t j = pos_y; j >= 0; j--)
        {
            int16_t tile_r2 = mapGenerator_tilesRoom(_data, (j * _data.x) + pos_x);
            if ((tile_r2 >= 0) && (tile_r2 != i))
            {
                _data.room[i].connection[_data.room[i].connectionCount].ID = tile_r2;
                _data.room[i].connection[_data.room[i].connectionCount].direction = eDirection::UP;
                if (_data.room[i].connectionCount < _data.roomMaxConnections)
                    _data.room[i].connectionCount++;
                j = -1;
            }
        }
    }
}

void mapGenerator_connectRooms_direct(sGenerationData &_data)
{
    mapGenerator_findNeighborRooms(_data);
    for (uint16_t i = 0; i < _data.roomCount; i++)
    {
        for (uint16_t j = 0; j < _data.room[i].connectionCount; j++)
        {
            uint16_t r1_x = _data.room[i].position % _data.x;
            uint16_t r1_y = _data.room[i].position / _data.y;
            uint16_t r2_x = _data.room[j].position % _data.x;
            uint16_t r2_y = _data.room[j].position / _data.y;
            if ((_data.room[i].connection[j].direction == eDirection::UP) || (_data.room[i].connection[j].direction == eDirection::DOWN))
                for (uint16_t k = ((r1_y < r2_y) ? r1_y : r2_y); k < ((r1_y > r2_y) ? r1_y : r2_y); k++)
                {
                    uint32_t tile = (k * _data.x) + r1_x;
                    if (tile < _data.mapSize)
                        _data.tile[tile] = eTile::FLOOR;
                }
            if ((_data.room[i].connection[j].direction == eDirection::LEFT) || (_data.room[i].connection[j].direction == eDirection::RIGHT))
                for (uint16_t k = ((r1_x < r2_x) ? r1_x : r2_x); k < ((r1_x > r2_x) ? r1_x : r2_x); k++)
                {
                    uint32_t tile = (r1_y * _data.x) + k;
                    if (tile < _data.mapSize)
                        _data.tile[tile] = eTile::FLOOR;
                }
        }
    }
}

void mapGenerator_connectRooms_90d(sGenerationData &_data)
{
    for (uint32_t i = 0; i < _data.roomCount; i++)
    {
        if (_data.room[i].valid)
        {
            for (uint32_t j = 0; j < _data.roomCount; j++)
            {
                bool path_found = false;
                if ((_data.room[j].valid)&&(i != j))
                {
                    if (!path_found)
                    {
                        eTile previous_tile = eTile::FLOOR;
                        uint32_t transitions = 0;
                        uint32_t current_x = _data.room[i].position % _data.x;
                        uint32_t current_y = _data.room[i].position / _data.x;
                        for (current_x = (_data.room[i].position % _data.x); current_x != (_data.room[j].position % _data.x);)
                        {
                            if (previous_tile != _data.tile[(current_y * _data.x) + current_x])
                                transitions++;
                            if (_data.tile[(current_y * _data.x) + current_x] == eTile::PATH)
                                transitions++;
                            previous_tile = _data.tile[(current_y * _data.x) + current_x];
                            if ((_data.room[i].position % _data.x) < (_data.room[j].position % _data.x)) current_x++;
                                    else current_x--;
                        }
                        for (current_y = (_data.room[i].position / _data.x); current_y != (_data.room[j].position / _data.x);)
                        {
                            if (previous_tile != _data.tile[(current_y * _data.x) + current_x])
                                transitions++;
                            if (_data.tile[(current_y * _data.x) + current_x] == eTile::PATH)
                                transitions++;
                            previous_tile = _data.tile[(current_y * _data.x) + current_x];
                            if ((_data.room[i].position / _data.x) < (_data.room[j].position / _data.x)) current_y++;
                                    else current_y--;
                        }
                        if ((!path_found)&&(transitions == 2))
                        {
                            path_found = true;
                            current_x = (_data.room[i].position % _data.x);
                            current_y = (_data.room[i].position / _data.x);
                            for (current_x = (_data.room[i].position % _data.x); current_x != (_data.room[j].position % _data.x);)
                            {
                                _data.tile[(current_y * _data.x) + current_x] = eTile::PATH;
                                if ((_data.room[i].position % _data.x) < (_data.room[j].position % _data.x)) current_x++;
                                        else current_x--;
                            }
                            for (current_y = (_data.room[i].position / _data.x); current_y != (_data.room[j].position / _data.x);)
                            {
                                _data.tile[(current_y * _data.x) + current_x] = eTile::PATH;
                                if ((_data.room[i].position / _data.x) < (_data.room[j].position / _data.x)) current_y++;
                                        else current_y--;
                            }
                        }
                    }
                    if (!path_found)
                    {
                        eTile previous_tile = eTile::FLOOR;
                        uint32_t transitions = 0;
                        uint32_t current_x = (_data.room[i].position % _data.x);
                        uint32_t current_y = (_data.room[i].position / _data.x);
                        for (current_y = (_data.room[i].position / _data.x); current_y != (_data.room[j].position / _data.x);)
                        {
                            if (previous_tile != _data.tile[(current_y * _data.x) + current_x])
                                transitions++;
                            if (_data.tile[(current_y * _data.x) + current_x] == eTile::PATH)
                                transitions++;
                            previous_tile = _data.tile[(current_y * _data.x) + current_x];
                            if ((_data.room[i].position / _data.x) < (_data.room[j].position / _data.x)) current_y++;
                                    else current_y--;
                        }
                        for (current_x = (_data.room[i].position % _data.x); current_x != (_data.room[j].position % _data.x);)
                        {
                            if (previous_tile != _data.tile[(current_y * _data.x) + current_x])
                                transitions++;
                            if (_data.tile[(current_y * _data.x) + current_x] == eTile::PATH)
                                transitions++;
                            previous_tile = _data.tile[(current_y * _data.x) + current_x];
                            if ((_data.room[i].position % _data.x) < (_data.room[j].position % _data.x)) current_x++;
                                    else current_x--;
                        }
                        if ((!path_found)&&(transitions == 2))
                        {
                            path_found = true;
                            current_x = (_data.room[i].position % _data.x);
                            current_y = (_data.room[i].position / _data.x);
                            for (current_y = (_data.room[i].position / _data.x); current_y != (_data.room[j].position / _data.x);)
                            {
                                _data.tile[(current_y * _data.x) + current_x] = eTile::PATH;
                                if ((_data.room[i].position / _data.x) < (_data.room[j].position / _data.x)) current_y++;
                                        else current_y--;
                            }
                            for (current_x = (_data.room[i].position % _data.x); current_x != (_data.room[j].position % _data.x);)
                            {
                                _data.tile[(current_y * _data.x) + current_x] = eTile::PATH;
                                if ((_data.room[i].position % _data.x) < (_data.room[j].position % _data.x)) current_x++;
                                        else current_x--;
                            }
                        }
                    }
                }
            }
        }
    }
}

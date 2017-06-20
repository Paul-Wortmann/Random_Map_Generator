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

#include "map_generator_M1.hpp"

bool isWallTile(sGenerationData &_data, uint32_t _tile)
{
    uint16_t x = _tile % _data.x;
    uint16_t y = _tile / _data.x;
    if (x < _data.x && y < _data.y && _data.tile[_tile] != eTile::WALL)
        return false;
    return true;
}

bool map_gen_maze_check_tile(sGenerationData &_data, uint32_t _tile, uint16_t _direction_bias)
{
    uint16_t tile_x = _tile % _data.x;
    uint16_t tile_y = _tile / _data.x;
    bool return_value = true;
    if (tile_y == 0 || tile_y == _data.y-1 || tile_x == 0 || tile_x == _data.x-1)
        return_value = false;
    if (_data.tile[_tile] == eTile::FLOOR)
        return_value = false;
    uint32_t tile_count = tile_y * _data.x + tile_x;
    if (return_value)
    {
        if (_direction_bias == _data.directionBias_up)
        {
            for (uint16_t wall_count = 1; wall_count <= _data.wallWidth; wall_count++)
            {
                uint16_t t[5];
                t[0] = tile_count-wall_count;
                t[1] = tile_count+wall_count;
                t[2] = tile_count-_data.x*wall_count;
                t[3] = tile_count-_data.x-wall_count;
                t[4] = tile_count-_data.x+wall_count;

                for (uint16_t i = 0; i < 5; i++)
                {
                    if (!isWallTile(_data, t[i])) return_value = false;
                }
            }
        }
        else if (_direction_bias == _data.directionBias_down)
        {
            for (uint16_t wall_count = 1; wall_count <= _data.wallWidth; wall_count++)
            {
                uint16_t t[5];
                t[0] = tile_count-wall_count;
                t[1] = tile_count+wall_count;
                t[2] = tile_count+_data.x*wall_count;
                t[3] = tile_count+_data.x-wall_count;
                t[4] = tile_count+_data.x+wall_count;

                for (uint16_t i = 0; i < 5; i++)
                {
                    if (!isWallTile(_data, t[i])) return_value = false;
                }
            }
        }
        else if (_direction_bias == _data.directionBias_right)
        {
            for (uint16_t wall_count = 1; wall_count <= _data.wallWidth; wall_count++)
            {
                uint16_t t[5];
                t[0] = tile_count-wall_count;
                t[1] = tile_count-_data.x*wall_count;
                t[2] = tile_count-_data.x-wall_count;
                t[3] = tile_count+_data.x*wall_count;
                t[4] = tile_count+_data.x-wall_count;

                for (uint16_t i = 0; i < 5; i++)
                {
                    if (!isWallTile(_data, t[i])) return_value = false;
                }
            }
        }
        else if (_direction_bias == _data.directionBias_left)
        {
            for (uint16_t wall_count = 1; wall_count <= _data.wallWidth; wall_count++)
            {
                uint16_t t[5];
                t[0] = tile_count+wall_count;
                t[1] = tile_count-_data.x*wall_count;
                t[2] = tile_count-_data.x+wall_count;
                t[3] = tile_count+_data.x*wall_count;
                t[4] = tile_count+_data.x+wall_count;

                for (uint16_t i = 0; i < 5; i++)
                {
                    if (!isWallTile(_data, t[i])) return_value = false;
                }
            }
        }
    }
    return (return_value);
}

void map_gen_maze(sGenerationData &_data, uint32_t _tile, uint16_t _direction_bias)
{
    _data.tile[_tile] = eTile::FLOOR;
    uint32_t tiles_available = 1;
    bool new_direction_bias = false;
    uint16_t tile_direction = 0;
    while (tiles_available > 0)
    {
        tiles_available = 0;
        uint32_t tile_up = _tile-_data.x;
        bool tile_up_ok = map_gen_maze_check_tile(_data, tile_up, _data.directionBias_up);
        if (tile_up_ok) tiles_available++;

        uint32_t tile_down = _tile+_data.x;
        bool tile_down_ok = map_gen_maze_check_tile(_data, tile_down, _data.directionBias_down);
        if (tile_down_ok) tiles_available++;

        uint32_t tile_right = _tile+1;
        bool tile_right_ok = map_gen_maze_check_tile(_data, tile_right, _data.directionBias_right);
        if (tile_right_ok) tiles_available++;

        uint32_t tile_left = _tile-1;
        bool tile_left_ok = map_gen_maze_check_tile(_data, tile_left, _data.directionBias_left);
        if (tile_left_ok) tiles_available++;

        if ((new_direction_bias) || (_direction_bias == _data.directionBias_none)) tile_direction = (_data.rmg_rand() % 4) + 1;
        else
        {
            tile_direction = (_data.rmg_rand()%(4+_data.directionBias_Threshold))+1;
            if (tile_direction > 4) tile_direction = _direction_bias;
            else
            {
                _direction_bias = tile_direction;
                new_direction_bias = true;
            }
        }
        if (tile_direction == _data.directionBias_up)
        {
            if (tile_up_ok)
            {
                tiles_available--;
                tile_up_ok = false;
                map_gen_maze(_data, tile_up, _direction_bias);
            }
        }
        else if (tile_direction == _data.directionBias_down)
        {
            if (tile_down_ok)
            {
                tiles_available--;
                tile_down_ok = false;
                map_gen_maze(_data, tile_down, _direction_bias);
            }
        }
        else if (tile_direction == _data.directionBias_right)
        {
            if (tile_right_ok)
            {
                tiles_available--;
                tile_right_ok = false;
                map_gen_maze(_data, tile_right, _direction_bias);
            }
        }
        else if (tile_direction == _data.directionBias_left)
        {
            if (tile_left_ok)
            {
                tiles_available--;
                tile_left_ok = false;
                map_gen_maze(_data, tile_left, _direction_bias);
            }
        }
        else
            tiles_available--;
    }
}

bool add_room(sGenerationData &_data, sRoom &_room)
{

    _room.x = _data.roomMax_x + _data.rmg_rand() % (_data.x - (_data.roomMax_x*2));
    _room.y = _data.roomMax_y + _data.rmg_rand() % (_data.y - (_data.roomMax_y*2));
    _room.w = _data.roomMin_x + _data.rmg_rand() % (_data.roomMax_x - _data.roomMin_x);
    _room.h = _data.roomMin_y + _data.rmg_rand() % (_data.roomMax_y - _data.roomMin_y);
    for (uint16_t i = _room.y-1-(uint16_t)_room.h/2.0f; i < _room.y+1+(uint16_t)_room.h/2.0f; i++)
    {
        for (uint16_t j = _room.x-1-(uint16_t)_room.w/2.0f; j < _room.x+1+(uint16_t)_room.w/2.0f; j++)
        {
            if (_data.tile[(i * _data.x) + j] == eTile::FLOOR)
                return false;
        }
    }
    for (uint16_t i = _room.y-(uint16_t)_room.h/2.0f; i < _room.y+(uint16_t)_room.h/2.0f; i++)
    {
        for (uint16_t j = _room.x-(uint16_t)_room.w/2.0f; j < _room.x+(uint16_t)_room.w/2.0f; j++)
        {
            _data.tile[(i * _data.x) + j] = eTile::FLOOR;
        }
    }
    return true;
}

bool connect_room(sGenerationData &_data, sRoom &_room)
{
    uint16_t direction_axis = (abs(_room.x - _data.x) > abs(_room.y - _data.y)) ? _data.axisBias_x : _data.axisBias_y;
    uint16_t direction_bias = (direction_axis == _data.axisBias_x) ? ((_room.x > _data.x/2) ? _data.directionBias_right : _data.directionBias_left) : ((_room.y > _data.y/2) ? _data.directionBias_down : _data.directionBias_up);
    bool foundWall = false;
    bool pathFound = false;
    uint32_t pathPos = (_room.y * _data.x) + _room.x;
    uint16_t pathMax = (direction_axis == _data.axisBias_x) ? (abs(_data.x - _room.x) - 1) : (abs(_data.y - _room.y) - 1);
    for (uint16_t i = 0; i < pathMax; i++)
    {
        if (!pathFound)
        {
            if ((!foundWall)&&(_data.tile[pathPos] == eTile::WALL))
                foundWall = true;
            if ((foundWall)&&(_data.tile[pathPos] == eTile::FLOOR))
                pathFound = true;
            if ((foundWall)&&(_data.tile[pathPos] == eTile::WALL))
                _data.tile[pathPos] = eTile::FLOOR;
            pathPos = (direction_axis == _data.axisBias_x) ? ((direction_bias == _data.directionBias_right) ? pathPos-1 : pathPos+1) : ((direction_bias == _data.directionBias_down) ? pathPos-_data.x : pathPos+_data.x);
        }
    }
    return true;
}

void mapGenerator_M1(sGenerationData &_data)
{
    uint32_t mapSize = _data.x * _data.y;
    _data.tile = new eTile[mapSize];
    for (uint32_t i = 0; i < _data.mapSize; i++)
        _data.tile[i] = eTile::WALL;
    uint16_t no_of_rooms = 2+ _data.rmg_rand() % 4;
    sRoom *room = new sRoom[no_of_rooms];
    for (uint16_t i = 0; i < no_of_rooms; i++)
        while (!add_room(_data, room[i]));
    map_gen_maze(_data, ((_data.x/2) + (_data.y/2)), _data.directionBias_none);
    for (uint16_t i = 0; i < no_of_rooms; i++)
        connect_room(_data, room[i]);

    delete[] room;

}

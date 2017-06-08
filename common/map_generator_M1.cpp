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
    if (x >= 0 && y >= 0 && x < _data.x && y < _data.y && _data.tile[_tile] != eTile::WALL)
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
        switch (_direction_bias)
        {
            case DIRECTION_BIAS_UP:
                for (uint16_t wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
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
            break;
            case DIRECTION_BIAS_DOWN:
                for (uint16_t wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
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
            break;
            case DIRECTION_BIAS_RIGHT:
                for (uint16_t wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
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
            break;
            case DIRECTION_BIAS_LEFT:
                for (uint16_t wall_count = 1; wall_count <= WALL_WIDTH; wall_count++)
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
            break;
            case DIRECTION_BIAS_NONE:
            default:
                // throw error
                return_value = true;
            break;
        }
    }
    return (return_value);
}

void map_gen_maze(sGenerationData &_data, uint32_t _tile, uint16_t _direction_bias)
{
    _data.tile[_tile] = eTile::FLOOR;
    uint32_t tiles_available = 1;
    bool new__direction_bias = false;
    uint16_t tile_direction = 0;
    while (tiles_available > 0)
    {
        tiles_available = 0;
        uint32_t tile_north = _tile-_data.x;
        bool tile_north_ok = map_gen_maze_check_tile(_data, tile_north, DIRECTION_BIAS_UP);
        if (tile_north_ok) tiles_available++;

        uint32_t tile_south = _tile+_data.x;
        bool tile_south_ok = map_gen_maze_check_tile(_data, tile_south, DIRECTION_BIAS_DOWN);
        if (tile_south_ok) tiles_available++;

        uint32_t tile_east = _tile-1;
        bool tile_east_ok = map_gen_maze_check_tile(_data, tile_east, DIRECTION_BIAS_RIGHT);
        if (tile_east_ok) tiles_available++;

        uint32_t tile_west = _tile+1;
        bool tile_west_ok = map_gen_maze_check_tile(_data, tile_west, DIRECTION_BIAS_LEFT);
        if (tile_west_ok) tiles_available++;

        if ((new__direction_bias) || (_direction_bias == DIRECTION_BIAS_NONE)) tile_direction = (rand() % 4) + 1;
        else
        {
            tile_direction = (rand()%(4+DIRECTION_BIAS_THRESHOLD))+1;
            if (tile_direction > 4) tile_direction = _direction_bias;
            else
            {
                _direction_bias = tile_direction;
                new__direction_bias = true;
            }
        }
        switch (tile_direction)
        {
            case DIRECTION_BIAS_UP:
                if (tile_north_ok)
                {
                    tiles_available--;
                    tile_north_ok = false;
                    map_gen_maze(_data, tile_north, _direction_bias);
                }
            break;
            case DIRECTION_BIAS_DOWN:
                if (tile_south_ok)
                {
                    tiles_available--;
                    tile_south_ok = false;
                    map_gen_maze(_data, tile_south, _direction_bias);
                }
            break;
            case DIRECTION_BIAS_RIGHT:
                if (tile_east_ok)
                {
                    tiles_available--;
                    tile_east_ok = false;
                    map_gen_maze(_data, tile_east, _direction_bias);
                }
            break;
            case DIRECTION_BIAS_LEFT:
                if (tile_west_ok)
                {
                    tiles_available--;
                    tile_west_ok = false;
                    map_gen_maze(_data, tile_west, _direction_bias);
                }
            break;
            default:
                tiles_available--;
            break;
        }
    }
}

bool add_room(sGenerationData &_data, sRoom &_room)
{

    _room.x = ROOM_MAX_X + rand() % (_data.x - (ROOM_MAX_X*2));
    _room.y = ROOM_MAX_X + rand() % (_data.y - (ROOM_MAX_X*2));
    _room.w = ROOM_MIN_X + rand() % (ROOM_MAX_X - ROOM_MIN_X);
    _room.h = ROOM_MIN_Y + rand() % (ROOM_MAX_Y - ROOM_MIN_Y);
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
    uint16_t direction_axis = (abs(_room.x - _data.x) > abs(_room.y - _data.y)) ? AXIS_BIAS_X : AXIS_BIAS_Y;
    uint16_t direction_bias = DIRECTION_BIAS_NONE;
    direction_bias = (direction_axis == AXIS_BIAS_X) ? ((_room.x > _data.x/2) ? DIRECTION_BIAS_RIGHT : DIRECTION_BIAS_LEFT) : ((_room.y > _data.y/2) ? DIRECTION_BIAS_DOWN : DIRECTION_BIAS_UP);
    bool foundWall = false;
    bool pathFound = false;
    uint32_t pathPos = (_room.y * _data.x) + _room.x;
    uint16_t pathMax = (direction_axis == AXIS_BIAS_X) ? (abs(_data.x - _room.x) - 1) : (abs(_data.y - _room.y) - 1);
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
            pathPos = (direction_axis == AXIS_BIAS_X) ? ((direction_bias == DIRECTION_BIAS_RIGHT) ? pathPos-1 : pathPos+1) : ((direction_bias == DIRECTION_BIAS_DOWN) ? pathPos-_data.x : pathPos+_data.x);
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
    uint16_t no_of_rooms = 2+ rand() % 4;
    sRoom *room = new sRoom[no_of_rooms];
    for (uint16_t i = 0; i < no_of_rooms; i++)
        while (!add_room(_data, room[i]));
    map_gen_maze(_data, ((_data.x/2) + (_data.y/2)), DIRECTION_BIAS_NONE);
    for (uint16_t i = 0; i < no_of_rooms; i++)
        connect_room(_data, room[i]);

    delete[] room;

}

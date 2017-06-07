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
            case DIRECTION_BIAS_NORTH:
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
            case DIRECTION_BIAS_SOUTH:
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
            case DIRECTION_BIAS_EAST:
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
            case DIRECTION_BIAS_WEST:
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
    uint32_t tiles_available    = 1;
    bool new__direction_bias = false;
    uint16_t tile_direction      = 0;
    while (tiles_available > 0)
    {
        tiles_available = 0;
        uint32_t tile_north = _tile-_data.x;
        bool tile_north_ok = map_gen_maze_check_tile(_data, tile_north, DIRECTION_BIAS_NORTH);
        if (tile_north_ok) tiles_available++;

        uint32_t tile_south = _tile+_data.x;
        bool tile_south_ok = map_gen_maze_check_tile(_data, tile_south, DIRECTION_BIAS_SOUTH);
        if (tile_south_ok) tiles_available++;

        uint32_t tile_east = _tile-1;
        bool tile_east_ok = map_gen_maze_check_tile(_data, tile_east, DIRECTION_BIAS_EAST);
        if (tile_east_ok) tiles_available++;

        uint32_t tile_west = _tile+1;
        bool tile_west_ok = map_gen_maze_check_tile(_data, tile_west, DIRECTION_BIAS_WEST);
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
            case DIRECTION_BIAS_NORTH:
                if (tile_north_ok)
                {
                    tiles_available--;
                    tile_north_ok = false;
                    map_gen_maze(_data, tile_north, _direction_bias);
                }
            break;
            case DIRECTION_BIAS_SOUTH:
                if (tile_south_ok)
                {
                    tiles_available--;
                    tile_south_ok = false;
                    map_gen_maze(_data, tile_south, _direction_bias);
                }
            break;
            case DIRECTION_BIAS_EAST:
                if (tile_east_ok)
                {
                    tiles_available--;
                    tile_east_ok = false;
                    map_gen_maze(_data, tile_east, _direction_bias);
                }
            break;
            case DIRECTION_BIAS_WEST:
                if (tile_west_ok)
                {
                    tiles_available--;
                    tile_west_ok = false;
                    map_gen_maze(_data, tile_west, _direction_bias);
                }
            break;
            default:
                // report error?
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
    for (uint16_t i = _room.x-(uint16_t)_room.w/2.0f; i < _room.x+(uint16_t)_room.w/2.0f; i++)
    {
        for (uint16_t j = _room.y-(uint16_t)_room.h/2.0f; j < _room.y+(uint16_t)_room.h/2.0f; j++)
        {
            _data.tile[(i * _data.x) + j] = eTile::FLOOR;
        }
    }
    return true;
}

bool connect_room(sGenerationData &_data, sRoom &_room)
{
    if (abs(_room.x - _data.x/2) > abs(_room.y - _data.y/2))
    {
        bool found_wall = false;
        for (uint16_t i = _room.x; (i != _data.x-1)||(i != 0);)
        {
            if ((found_wall)&&(_data.tile[(_room.y * _data.x) + i] == eTile::FLOOR))
                break;
            if (_data.tile[(_room.y * _data.x) + i] == eTile::WALL) found_wall = true;
            _data.tile[(_room.y * _data.x) + i] = eTile::FLOOR;
            if (_room.x > _data.x/2) i--;
            else  i++;
        }
    }
    else
    {
        bool found_wall = false;
        for (uint16_t i = _room.y; (i != _data.y-1)||(i != 0);)
        {
            if ((found_wall)&&(_data.tile[(i * _data.x) + _room.x] == eTile::FLOOR)) break;
            if (_data.tile[(i * _data.x) + _room.x] == eTile::WALL) found_wall = true;
            _data.tile[(i * _data.x) + _room.x] = eTile::FLOOR;
            if (_room.y > _data.y/2) i--;
            else  i++;
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

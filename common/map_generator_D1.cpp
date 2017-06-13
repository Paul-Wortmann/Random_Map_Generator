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

#include "map_generator_D1.hpp"

uint16_t mapGenerator_D1_build_subrooms(sRoomGenData *_room, sGenerationData &_data)
{
    if ((_room->ex > _data.x) || (_room->ey > _data.y))
    {
        _data.error = eError::ALGORITHM;
        return 0;
    }
    uint16_t room_count = 0;
    uint16_t size_x = _room->ex - _room->sx;
    uint16_t size_y = _room->ey - _room->sy;
    bool split_x = (size_x > ((_data.roomMax_x * 2.125f) + _data.wallWidth));
    bool split_y = (size_y > ((_data.roomMax_y * 2.125f) + _data.wallWidth));
    if ((split_x) && (split_y))
        ((rand() % 100) > 50) ? split_x = false : split_y = false;
    if (split_x)
    {
        uint16_t split_pos = (rand() % (size_x - (_data.roomMin_x * 2))) + _data.roomMin_x;
        if (_data.error == eError::NONE)
        {
            _room->left = new sRoomGenData;
            _room->left->sx = _room->sx;
            _room->left->ex = _room->left->sx+split_pos;
            _room->left->sy = _room->sy;
            _room->left->ey = _room->ey;
            _room->left->parent = _room;
            _room->left->direction = eDirection::LEFT;
            room_count += mapGenerator_D1_build_subrooms(_room->left, _data);
        }
        if (_data.error == eError::NONE)
        {
            _room->right = new sRoomGenData;
            _room->right->sx = _room->left->sx+split_pos;
            _room->right->ex = _room->ex;
            _room->right->sy = _room->sy;
            _room->right->ey = _room->ey;
            _room->right->parent = _room;
            _room->right->direction = eDirection::RIGHT;
            room_count += mapGenerator_D1_build_subrooms(_room->right, _data);
        }
    }
    if (split_y)
    {
        uint16_t split_pos = (rand() % (size_y - (_data.roomMin_y * 2))) + _data.roomMin_y;
        if (_data.error == eError::NONE)
        {
            _room->left = new sRoomGenData;
            _room->left->sx = _room->sx;
            _room->left->ex = _room->ex;
            _room->left->sy = _room->sy;
            _room->left->ey = _room->left->sy+split_pos;
            _room->left->parent = _room;
            _room->left->direction = eDirection::LEFT;
            room_count += mapGenerator_D1_build_subrooms(_room->left, _data);
        }
        if (_data.error == eError::NONE)
        {
            _room->right = new sRoomGenData;
            _room->right->sx = _room->sx;
            _room->right->ex = _room->ex;
            _room->right->sy = _room->left->sy+split_pos;
            _room->right->ey = _room->ey;
            _room->right->parent = _room;
            _room->right->direction = eDirection::RIGHT;
            room_count += mapGenerator_D1_build_subrooms(_room->right, _data);
        }
    }
    if ((!split_x) && (!split_y))
    {
        _room->end_node = true;
        room_count += 1;
    }
    return room_count;
}

void mapGenerator_D1_genRooms(sRoomGenData *_room, sGenerationData &_data)
{
    bool debug = false;
    uint16_t room_ID = 0;
    bool done = false;
    _data.room = new sRoomData[_data.roomCount];
    sRoomGenData *tempRoom = _room;
     while (!done)
     {
        if ((tempRoom->parent == nullptr) && (tempRoom->left == nullptr) && (tempRoom->right == nullptr))
            done = true;
        if (tempRoom->left != nullptr)
            tempRoom = tempRoom->left;
        else if (tempRoom->right != nullptr)
            tempRoom = tempRoom->right;
        if ((tempRoom->left == nullptr) && (tempRoom->right == nullptr))
        {
            if(tempRoom->end_node)
            {
                _data.room[room_ID].valid = true;
                _data.room[room_ID].w = tempRoom->ex - tempRoom->sx;
                _data.room[room_ID].h = tempRoom->ey - tempRoom->sy;
                uint16_t x = tempRoom->sx + (_data.room[room_ID].w/2);
                uint16_t y = tempRoom->sy + (_data.room[room_ID].h/2);
                _data.room[room_ID].position = (y * _data.x) + x;
                room_ID++;
            }
            if (tempRoom->parent != nullptr)
            {
                eDirection direction = tempRoom->direction;
                tempRoom = tempRoom->parent;
                if (direction == eDirection::LEFT)
                {
                    delete tempRoom->left;
                    tempRoom->left = nullptr;
                }
                if (direction == eDirection::RIGHT)
                {
                    delete tempRoom->right;
                    tempRoom->right = nullptr;
                }
            }
        }
     }
}

void mapGenerator_D1_genRoomTileData(sGenerationData &_data)
{
    for (uint16_t i = 0; i < _data.roomCount; i++)
    {
        uint16_t sx = (_data.room[i].position % _data.x) - (_data.room[i].w / 2) +1;
        uint16_t ex = sx + _data.room[i].w -1;
        uint16_t sy = (_data.room[i].position / _data.x) - (_data.room[i].h / 2) +1;
        uint16_t ey = sy + _data.room[i].h -1;
        for (uint16_t i = sx; i < ex; i++)
        {
            for (uint16_t j = sy; j < ey; j++)
            {
                _data.tile[(j * _data.x) + i] = eTile::FLOOR;
            }
        }
    }
}

int16_t mapGenerator_D1_tilesRoom(sGenerationData &_data, uint32_t _tile)
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
}

void mapGenerator_D1_findNeighborRooms(sGenerationData &_data)
{
    for (uint16_t i = 0; i < _data.roomCount; i++)
    {
        uint16_t pos_x = _data.room[i].position % _data.x;
        uint16_t pos_y = _data.room[i].position / _data.y;
        for (uint16_t j = pos_x; j < _data.x; j++)
        {
            int16_t tile_r2 = mapGenerator_D1_tilesRoom(_data, (pos_y * _data.x) + j);
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
            int16_t tile_r2 = mapGenerator_D1_tilesRoom(_data, (pos_y * _data.x) + j);
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
            int16_t tile_r2 = mapGenerator_D1_tilesRoom(_data, (j * _data.x) + pos_x);
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
            int16_t tile_r2 = mapGenerator_D1_tilesRoom(_data, (j * _data.x) + pos_x);
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

void mapGenerator_D1_connectRooms(sGenerationData &_data)
{
    mapGenerator_D1_findNeighborRooms(_data);
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

void mapGenerator_D1(sGenerationData &_data)
{
    _data.mapSize = _data.x * _data.y;
    _data.tile = new eTile[_data.mapSize];
    for (uint16_t i = 0; i < _data.mapSize; i++)
        _data.tile[i] = eTile::WALL;
    sRoomGenData *room = new sRoomGenData;
    room->sx = 0;
    room->ex = _data.x-1;
    room->sy = 0;
    room->ey = _data.y-1;
    room->end_node = false;
    _data.roomCount = mapGenerator_D1_build_subrooms(room, _data);
    mapGenerator_D1_genRooms(room, _data);
    delete room;
    mapGenerator_D1_genRoomTileData(_data);
    mapGenerator_D1_connectRooms(_data);
}

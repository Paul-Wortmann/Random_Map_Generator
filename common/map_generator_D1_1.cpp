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

#include "map_generator_D1_1.hpp"

uint16_t mapGenerator_D1_1_build_subrooms(sRoomGenData *_room, sGenerationData &_data)
{
    uint16_t room_count = 0;
    uint16_t size_x = abs(_room->ex - _room->sx);
    uint16_t size_y = abs(_room->ey - _room->sy);
    bool split_x = (size_x > (_data.roomMax_x * 2));
    bool split_y = (size_y > (_data.roomMax_y * 2));
    if ((split_x) && (split_y))
        (rand() % 100 > 50) ? split_x = false : split_y = false;
    if (split_x)
    {
        uint16_t split_pos = (rand() % (size_x - (_data.roomMax_x * 2))) + _data.roomMin_x;
        _room->left = new sRoomGenData;
        _room->left->sx = 0;
        _room->left->ex = split_pos-1;
        _room->left->sy = _room->sy;
        _room->left->ey = _room->ey;
        _room->left->parent = _room;
        _room->left->direction = eDirection::LEFT;
        room_count += mapGenerator_D1_1_build_subrooms(_room->left, _data);

        _room->right = new sRoomGenData;
        _room->right->sx = split_pos;
        _room->right->ex = size_x;
        _room->right->sy = _room->sy;
        _room->right->ey = _room->ey;
        _room->right->parent = _room;
        _room->right->direction = eDirection::RIGHT;
        room_count += mapGenerator_D1_1_build_subrooms(_room->right, _data);
    }
    if (split_y)
    {
        uint16_t split_pos = (rand() % (size_y - (_data.roomMax_y * 2))) + _data.roomMin_y;
        _room->left = new sRoomGenData;
        _room->left->sx = _room->sx;
        _room->left->ex = _room->ex;
        _room->left->sy = 0;
        _room->left->ey = split_pos-1;
        _room->left->parent = _room;
        _room->left->direction = eDirection::LEFT;
        room_count += mapGenerator_D1_1_build_subrooms(_room->left, _data);

        _room->right = new sRoomGenData;
        _room->right->sx = _room->sx;
        _room->right->ex = _room->ex;
        _room->right->sy = split_pos;
        _room->right->ey = size_y;
        _room->right->parent = _room;
        _room->right->direction = eDirection::RIGHT;
        room_count += mapGenerator_D1_1_build_subrooms(_room->right, _data);
    }
    if ((!split_x) && (!split_y))
    {
        //std::cout << "Initial room build." << std::endl;
        //std::cout << "X: " << _room->sx << " - " << _room->ex << std::endl;
        //std::cout << "Y: " << _room->sy << " - " << _room->ey << std::endl;
        //std::cout << std::endl;
        _room->end_node = true;
        room_count += 1;
    }
    return room_count;
}

void mapGenerator_D1_1_genRooms(sRoomGenData *_room, sGenerationData &_data)
{
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
                _data.room[room_ID].position = (((_data.room[room_ID].h/2)+tempRoom->sy) * _data.x) + ((_data.room[room_ID].w/2)+tempRoom->sx);

                std::cout << "Assigning room:" << room_ID << "to map." << std::endl;
                std::cout << "Width: " << _data.room[room_ID].w << " - Height: " << _data.room[room_ID].h << std::endl;
                std::cout << "Position: " << _data.room[room_ID].position << std::endl;
                std::cout << std::endl;

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

void mapGenerator_D1_1_genRoomTileData(sGenerationData &_data)
{
    for (uint16_t i = 0; i < _data.roomCount; i++)
    {
        std::cout << _data.room[i].position << std::endl;
        _data.tile[_data.room[i].position] = eTile::FLOOR;
    }
}

void mapGenerator_D1_1(sGenerationData &_data)
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
    _data.roomCount = mapGenerator_D1_1_build_subrooms(room, _data);
    mapGenerator_D1_1_genRooms(room, _data);
    delete room;
    mapGenerator_D1_1_genRoomTileData(_data);


    //full rooms will floor tiles
    // generate rooms in _data.tile, using room data
    // connect rooms
}

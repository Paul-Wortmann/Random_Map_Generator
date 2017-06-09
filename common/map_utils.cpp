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
    for (uint16_t k = 0; k < _data.itterations; k++)
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

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

#include "map_import_roboengine.hpp"

void mapImport_RoboEngine(const sGenerationData &_data)
{
    bool graphicalOutput = true;
    std::ofstream t_fstream(_data.fileExport.c_str(), std::ifstream::out);
    t_fstream << "[ Version - " << _data.version << " ]" << std::endl;
    t_fstream << "[ Seed - " << _data.seed << " ]" << std::endl;
    t_fstream << "[ Dimensions - x: " << _data.x << " - y: " << _data.y << " ]" << std::endl;
    t_fstream << "[ Algorithm - " << (uint16_t)_data.algorithm << " ]" << std::endl << std::endl;
    for (uint16_t i = 0; i < _data.y; i++)
    {
        for (uint16_t j = 0; j < _data.x; j++)
        {
            uint32_t mapTile = (i * _data.y) + j;
            if (graphicalOutput)
            {
                if (_data.tile[mapTile] == eTile::FLOOR)
                    t_fstream << "_";
                else if (_data.tile[mapTile] == eTile::WALL)
                    t_fstream << "W";
                else if (_data.tile[mapTile] == eTile::PATH)
                    t_fstream << "P";
                else
                    t_fstream << "?";
            }
            else
            {
                t_fstream << (uint16_t)_data.tile[mapTile];
                if (j < _data.x-1)
                    t_fstream << ",";
            }
        }
        t_fstream << std::endl;
    }
    t_fstream << std::endl;
    t_fstream.close();
}

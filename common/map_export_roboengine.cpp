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

#include "map_export_roboengine.hpp"

void mapExport_RoboEngine(const sGenerationData &_data)
{
    uint16_t indentLevel = 0;
    std::string indent = "   ";
    bool graphicalOutput = true;
    std::ofstream t_fstream(_data.fileExport.c_str(), std::ifstream::out);
    // settings
    for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
    t_fstream << "<map>" << std::endl;
    indentLevel++;
    for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
    t_fstream << "<settings>" << std::endl;
    indentLevel++;
    for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
    t_fstream << "<version = " << _data.version << "/>" << std::endl;
    for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
    t_fstream << "<seed = " << _data.seed << "/>" << std::endl;
    for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
    t_fstream << "<algorithm = " << (uint16_t)_data.algorithm << "/>" << std::endl;
    for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
    t_fstream << "<dimension_x = " << (uint16_t)_data.x << "/>" << std::endl;
    for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
    t_fstream << "<dimension_y = " << (uint16_t)_data.y << "/>" << std::endl;
    indentLevel--;
    for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
    t_fstream << "</settings>" << std::endl;
    // room data
    if ((_data.roomCount > 0) && (_data.room != nullptr))
    {
        for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
        t_fstream << "<room_data>" << std::endl;
        indentLevel++;
        for (uint16_t i = 0; i < _data.roomCount; i++)
        {
            if (_data.room[i].valid)
            {
                for (uint16_t k = 0; k < indentLevel; k++) t_fstream << indent;
                t_fstream << "<room = " << i+1 << ">" << std::endl;
                indentLevel++;

                for (uint16_t k = 0; k < indentLevel; k++) t_fstream << indent;
                t_fstream << "<room_x = " << (_data.room[i].position % _data.x) << ">" << std::endl;
                for (uint16_t k = 0; k < indentLevel; k++) t_fstream << indent;
                t_fstream << "<room_y = " << (_data.room[i].position / _data.x) << ">" << std::endl;
                for (uint16_t k = 0; k < indentLevel; k++) t_fstream << indent;
                t_fstream << "<room_w = " << (_data.room[i].w) << ">" << std::endl;
                for (uint16_t k = 0; k < indentLevel; k++) t_fstream << indent;
                t_fstream << "<room_h = " << (_data.room[i].h) << ">" << std::endl;

                indentLevel--;
                for (uint16_t k = 0; k < indentLevel; k++) t_fstream << indent;
                t_fstream << "</room>" << std::endl;
            }
        }
        indentLevel--;
        for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
        t_fstream << "</room_data>" << std::endl;
    }
    // exit data
    if ((_data.exitCount > 0) && (_data.exit != nullptr))
    {
        for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
        t_fstream << "<exit_data>" << std::endl;
        indentLevel++;
        for (uint16_t i = 0; i < _data.roomCount; i++)
        {
            for (uint16_t k = 0; k < indentLevel; k++) t_fstream << indent;
            t_fstream << "<exit = " << i+1 << ">" << std::endl;
            indentLevel++;

            for (uint16_t k = 0; k < indentLevel; k++) t_fstream << indent;
            t_fstream << "<exit_x = " << (_data.exit[i].position % _data.x) << ">" << std::endl;
            for (uint16_t k = 0; k < indentLevel; k++) t_fstream << indent;
            t_fstream << "<exit_y = " << (_data.exit[i].position / _data.x) << ">" << std::endl;
            for (uint16_t k = 0; k < indentLevel; k++) t_fstream << indent;
            t_fstream << "<exit_uid = " << (_data.exit[i].ID) << ">" << std::endl;

            indentLevel--;
            for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
            t_fstream << "</exit>" << std::endl;
        }
        indentLevel--;
        for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
        t_fstream << "</exit_data>" << std::endl;
    }
    // tile data
    for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
    t_fstream << "<data>" << std::endl;
    indentLevel++;
    for (uint16_t i = 0; i < _data.y; i++)
    {
        for (uint16_t k = 0; k < indentLevel; k++) t_fstream << indent;
        t_fstream << "<tiles" << " = ";
        for (uint16_t j = 0; j < _data.x; j++)
        {
            uint32_t mapTile = (i * _data.y) + j;
            {
                t_fstream << (uint16_t)_data.tile[mapTile];
                if (j < _data.x-1)
                    t_fstream << ",";
            }
        }
        t_fstream << "/>" << std::endl;
    }
    indentLevel--;
    for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
    t_fstream << "</data>" << std::endl;
    indentLevel--;
    for (uint16_t i = 0; i < indentLevel; i++) t_fstream << indent;
    t_fstream << "</map>" << std::endl;

    t_fstream << std::endl;
    t_fstream.close();
}

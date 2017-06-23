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

void mapImport_RoboEngine(const std::string &_fileName, sGenerationData &_data)
{
    std::ifstream t_fstream(_fileName.c_str(), std::ifstream::in);
    if (t_fstream.is_open())
    {
        _data.roomCount = 0;
        _data.exitCount = 0;
        while (t_fstream.good())
        {
            std::string tData = "";
            std::getline(t_fstream, tData);
            uint32_t tLength = tData.length();
            std::string lData = "";
            if (tLength > 2)
            {
                for (uint32_t i = 0; i < tLength; i++)
                {
                    if (tData[i] != ' ')
                        lData += tData[i];
                    if (tData[i] == '>')
                        i = tLength;
                }
            }
            if (lData.compare("<room>") == 0)
            {
                _data.roomCount++;
                std::cout << "room: " << _data.roomCount << std::endl;
            }
            if (lData.compare("<exit>") == 0)
                _data.exitCount++;
        }
        if (_data.roomCount > 0)
            _data.room = new sRoomData[_data.roomCount];
        if (_data.exitCount > 0)
            _data.exit = new sExitData[_data.exitCount];
        t_fstream.clear();
        t_fstream.seekg (0, t_fstream.beg);
        int16_t tileCount = 0;
        int16_t roomCount = -1;
        int16_t roomDoorCount = 0;
        int16_t exitCount = -1;
        uint16_t initTiles = 0;
        while (t_fstream.good())
        {
            bool commentFound = false;
            bool dataBegin = false;
            bool keyBegin = false;
            bool equalBegin = false;
            bool valueBegin = false;
            std::string keyData = "";
            std::string valueData = "";
            std::string tData = "";
            std::getline(t_fstream, tData);
            uint32_t tLength = tData.length();
            std::string lData = "";
            for (uint32_t i = 0; i < tLength; i++)
            {
                if (tData[i] != ' ')
                    dataBegin = true;
                if (dataBegin)
                    lData += tData[i];
                if (tData[i] == '>')
                    i = tLength;
            }
            if (lData.compare("<room>") == 0)
            {
                roomCount++;
                roomDoorCount = 0;
            }
            if (lData.compare("<exit>") == 0)
                exitCount++;
            uint32_t lLength = lData.length();
            for (uint32_t i = 0; i < lLength; i++)
            {
                if (lData[0] == '#')
                    commentFound = true;
                if (!commentFound)
                {
                    if (keyBegin)
                    {
                        if (lData[i] == ' ')
                            keyBegin = false;
                        if (lData[i] == '>')
                        {
                            keyBegin = false;
                            keyData = "";
                        }
                        if (keyBegin)
                            keyData += lData[i];
                    }
                    if ((valueBegin) && (lData[i] == '/'))
                        valueBegin = false;
                    if (valueBegin)
                        valueData += lData[i];
                    if ((equalBegin) && (lData[i] == ' '))
                    {
                        equalBegin = false;
                        valueBegin = true;
                    }
                    if ((lData[i] == '<') && (lData[i+1] != '/'))
                        keyBegin = true;
                    if (lData[i] == '=')
                        equalBegin = true;
                }
            }
            // map settings:
            if (keyData.compare("seed") == 0)
                _data.seed = std::stoull(valueData);
            if (keyData.compare("dimension_x") == 0)
            {
                _data.x = std::stoul(valueData);
                initTiles++;
            }
            if (keyData.compare("dimension_y") == 0)
            {
                _data.y = std::stoul(valueData);
                initTiles++;
            }
            if (initTiles == 2)
            {
                initTiles++;
                if (_data.tile != nullptr)
                {
                    delete [] _data.tile;
                    _data.tile = nullptr;
                }
                _data.tileCount = _data.x * _data.y;
                _data.tile = new eTile[_data.tileCount];
                for (uint16_t k = 0; k < _data.tileCount; k++)
                    _data.tile[k] = eTile::FLOOR;
            }
            if (keyData.compare("algorithm") == 0)
            {
                if (valueData.compare("AC1") == 0)
                    _data.algorithm = eAlgorithm::AC1;
                if (valueData.compare("AD1") == 0)
                    _data.algorithm = eAlgorithm::AD1;
                if (valueData.compare("AD2") == 0)
                    _data.algorithm = eAlgorithm::AD2;
                if (valueData.compare("AD3") == 0)
                    _data.algorithm = eAlgorithm::AD3;
                if (valueData.compare("AM1") == 0)
                    _data.algorithm = eAlgorithm::AM1;
            }
            if (keyData.compare("tiles") == 0)
            {
                uint16_t tilePos = 0;
                uint16_t tilesLength = valueData.length();
                for (uint16_t k = 0; k < tilesLength; k++)
                {
                    if (valueData[k] != ',')
                    {
                        _data.tile[(tileCount * _data.x) + tilePos] = (eTile)(valueData[k]-'0');
                        tilePos++;
                    }
                }
                tileCount++;
            }

            // map rooms:
            // map exits:

        }

        t_fstream.close();
    }
    else
    {
        std::cout << "Error opening setting file: " << _fileName << std::endl;
    }
}


/*
<map>
   <settings>
      <version = 0.0.1/>
      <seed = 1498146878/>
      <algorithm = 2/>
      <dimension_x = 100/>
      <dimension_y = 100/>
   </settings>
   <data>
   </data>
   <room_data>
   </room_data>
</map>

*/

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

#include "map_import_settings_file.hpp"

void mapImportSettings(const std::string &_fileName, sGenerationData &_data)
{
    std::ifstream t_fstream(_fileName.c_str(), std::ifstream::in);
    if (t_fstream.is_open())
    {
        while (t_fstream.good())
        {
            bool commentFound = false;
            bool keyBegin = false;
            bool equalBegin = false;
            bool valueBegin = false;
            bool dataBegin = false;
            std::string keyData = "";
            std::string valueData = "";
            std::string tData = "";
            std::getline(t_fstream, tData);
            uint32_t tLength = tData.length();
            std::string lData = "";
            for (uint32_t i = 0; i < tLength; i++)
            {
                if ((!dataBegin) && (tData[i] != ' '))
                    dataBegin = true;
                if (dataBegin)
                    lData += tData[i];
            }

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
            if (keyData.compare("map_seed") == 0)
                _data.seed = std::stoull(valueData);
            if (keyData.compare("map_width") == 0)
                _data.x = std::stoul(valueData);
            if (keyData.compare("map_height") == 0)
                _data.y = std::stoul(valueData);
            if (keyData.compare("map_algorithm") == 0)
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
            if (keyData.compare("map_exporter") == 0)
            {
                if (valueData.compare("ED1") == 0)
                    _data.exporter = eExporter::ED1;
                if (valueData.compare("EF1") == 0)
                    _data.exporter = eExporter::EF1;
            }
            if (keyData.compare("file_export") == 0)
                    _data.fileExport = valueData;
            if (keyData.compare("wall_Width") == 0)
                    _data.wallWidth = std::stoul(valueData);
            if (keyData.compare("cell_density") == 0)
                    _data.density = std::stof(valueData);
            if (keyData.compare("iterations") == 0)
                    _data.iterations = std::stoul(valueData);
            if (keyData.compare("room_min_x") == 0)
                    _data.roomMin_x = std::stoul(valueData);
            if (keyData.compare("room_min_y") == 0)
                    _data.roomMin_y = std::stoul(valueData);
            if (keyData.compare("room_max_x") == 0)
                    _data.roomMax_x = std::stoul(valueData);
            if (keyData.compare("room_max_y") == 0)
                    _data.roomMax_y = std::stoul(valueData);
        }
        t_fstream.close();
        if (_data.x < 10)
            _data.x = 10;
        if (_data.y < 10)
            _data.y = 10;
        _data.mapSize = _data.x * _data.y;
        if (_data.roomMin_x < 3)
            _data.roomMin_x = 3;
        if (_data.roomMin_y < 3)
            _data.roomMin_y = 3;
        if (_data.roomMax_x < 3)
            _data.roomMax_x = 3;
        if (_data.roomMax_y < 3)
            _data.roomMax_y = 3;
        if (_data.roomMin_x > _data.x / 2)
            _data.roomMin_x = _data.x / 2;
        if (_data.roomMin_y > _data.y / 2)
            _data.roomMin_y = _data.y / 2;
        if (_data.roomMax_x > _data.x / 2)
            _data.roomMax_x = _data.x / 2;
        if (_data.roomMax_y > _data.y / 2)
            _data.roomMax_y = _data.y / 2;
        if (_data.roomMax_x < _data.roomMin_x)
            _data.roomMax_x = _data.roomMin_x + 2;
        if (_data.roomMax_y < _data.roomMin_y)
            _data.roomMax_y = _data.roomMin_y + 2;
    }
    else
    {
        std::cout << "Error opening setting file: " << _fileName << std::endl;
    }
}

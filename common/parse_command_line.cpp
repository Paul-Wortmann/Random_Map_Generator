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

#include "parse_command_line.hpp"

void _parseCommandLine(sGenerationData &_data, int _argc, char** _argv)
{
    std::string temp_string = "";
    for (int i = 0; i < _argc; i++)
    {
        temp_string = _argv[i];
        if ((temp_string.compare("-s") == 0) && (i+1 < _argc))
            _data.fileSettings = _argv[i+1];
        else if ((temp_string.compare("-e") == 0) && (i+1 < _argc))
            _data.fileExport = _argv[i+1];
        else
        {
            std::cout << "Random Map Generator." << std::endl;
            std::cout << "Copyright (C) 2017 Paul Wortmann, PhysHex Games, www.physhexgames.com" << std::endl << std::endl;
            std::cout << "Random_Map_Generator -s fileName.txt -e fileName.txt" << std::endl << std::endl;
            std::cout << "-s   Specify optional settings file to use." << std::endl;
            std::cout << "-e   Export to file name, else 'default.txt' will be used." << std::endl;
        }
    }
}


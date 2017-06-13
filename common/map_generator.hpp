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

#ifndef MAPGENERATOR_HPP
#define MAPGENERATOR_HPP

#include "map_data_types.hpp"
#include "map_export_debug.hpp"
#include "map_export_flare.hpp"
#include "map_generator_C1.hpp"
#include "map_generator_D1.hpp"
#include "map_generator_D2.hpp"
#include "map_generator_M1.hpp"
#include "map_import_settings_file.hpp"
#include "parse_command_line.hpp"

class cMapGenerator
{
    public:
        cMapGenerator(void) = default;
        virtual ~cMapGenerator(void) = default;
        cMapGenerator(const cMapGenerator& _other) = default;
        cMapGenerator& operator=(const cMapGenerator& _other) {if (this == &_other) return *this; return *this;}
        void free(sGenerationData &_data);
        uint16_t generate(sGenerationData &_data);
        void parseCommandLine(sGenerationData &_data, int _argc, char** _argv) {_parseCommandLine(_data, _argc, _argv); mapImportSettings(_data.fileSettings, _data);}
    protected:
    private:
};

#endif // MAPGENERATOR_HPP

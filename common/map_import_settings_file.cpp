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
    if (_fileName.length() <= 2);

    _data.algorithm = eAlgorithm::AM1;
    _data.exporter = eExporter::ED1;
}




/* possible Command line arguments to implement

-s fileName
-e fileName

--version
-v
--width (minimum 10)
-w
--height (minimum 10)
-h
--algorithm
-a
--filename
-f
--export
-e
*/

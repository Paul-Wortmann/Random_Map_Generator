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

#include "map_generator.hpp"

void cMapGenerator::free(sGenerationData &_data)
{
    if (_data.tile != nullptr)
    {
        delete [] _data.tile;
        _data.tile = nullptr;
    }
    if (_data.room != nullptr)
    {
        delete [] _data.room;
        _data.room = nullptr;
    }
    if (_data.exit != nullptr)
    {
        delete [] _data.exit;
        _data.exit = nullptr;
    }
}

uint16_t cMapGenerator::generate(sGenerationData &_data)
{
    _data.seed = (_data.seed == 0) ? time(nullptr) : _data.seed;
    _data.rmg_rseed(_data.seed);
    mapImport_RoboEngine("test.rmx", _data);
    if ((_data.x < 10) || (_data.y < 10))
        return EXIT_FAILURE;
    else
    {
        /*
        // generate
        switch (_data.algorithm)
        {
            case eAlgorithm::AC1:
                mapGenerator_C1(_data);
            break;
            case eAlgorithm::AD1:
                mapGenerator_D1(_data);
            break;
            case eAlgorithm::AD2:
                mapGenerator_D2(_data);
            break;
            case eAlgorithm::AD3:
                mapGenerator_D3(_data);
            break;
            case eAlgorithm::AM1:
                mapGenerator_M1(_data);
            break;
            default:
                return EXIT_FAILURE;
            break;
        }
        */
        // export
        switch (_data.exporter)
        {
            case eExporter::EF1:
                mapExport_Flare(_data);
            break;
            case eExporter::ER1:
                mapExport_RoboEngine(_data);
            break;
            default:
            case eExporter::ED1:
                mapExport_Debug(_data);
            break;
        }
    }
    return EXIT_SUCCESS;
}


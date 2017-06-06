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

#include "map_export_debug.hpp"

uint32_t checkTile(const sGenerationData &_data, sFillData &_fillData, const uint32_t &_parentTile, const uint32_t &_tile)
{
    uint32_t mapSize = _data.x * _data.y;
    uint32_t returnValue = 0;
    if ((_tile >= 0) && (_tile < mapSize))
    {
        if ((_fillData.valid[_parentTile]) && (_data.tile[_tile] == _fillData.tile))
            _fillData.valid[_tile] = true;
        returnValue = (_fillData.valid[_tile]) ? 1 : 0;
    }
    return returnValue;
}

uint32_t checkNeighboringTiles(const sGenerationData &_data, sFillData &_fillData, const uint32_t &_tile)
{
    uint32_t mapSize = _data.x * _data.y;
    uint32_t returnValue = 1;
    _fillData.valid[_tile] = true;
    for (uint32_t i = 0; i < _data.x + _data.y; i++)
    {
        for (uint32_t j = 0; j < mapSize; j++)
        {
            returnValue += checkTile(_data, _fillData, j, j+1);
            returnValue += checkTile(_data, _fillData, j, j-1);
            returnValue += checkTile(_data, _fillData, j, j+_data.x);
            returnValue += checkTile(_data, _fillData, j, j+_data.x+1);
            returnValue += checkTile(_data, _fillData, j, j+_data.x-1);
            returnValue += checkTile(_data, _fillData, j, j-_data.x);
            returnValue += checkTile(_data, _fillData, j, j-_data.x+1);
            returnValue += checkTile(_data, _fillData, j, j-_data.x-1);
        }
    }
    return returnValue;
}

void mapFloodFill(const sGenerationData &_data, sFillData &_fillData, const uint32_t &_startTile)
{
    checkNeighboringTiles(_data, _fillData, _startTile);
}

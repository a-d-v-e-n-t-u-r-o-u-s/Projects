/*!
 * \file
 * \brief 1-WIRE implementation file
 * \author Dawid Babula
 * \email dbabula@adventurous.pl
 *
 * \par Copyright (C) Dawid Babula, 2019
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "1wire.h"
#include <stddef.h>

static WIRE_config_t driver_config;

static inline bool is_mode_valid(WIRE_mode_t mode)
{
    switch(mode)
    {
        case WIRE_9BIT_MODE:
            /* no break */
        case WIRE_10BIT_MODE:
            /* no break */
        case WIRE_11BIT_MODE:
            /* no break */
        case WIRE_12BIT_MODE:
            return true;
        default:
            return false;
    }
}

int8_t WIRE_configure(const WIRE_config_t *config)
{
    if(config == NULL)
    {
        return -1;
    }

    if(!is_mode_valid(config->mode))
    {
        return -1;
    }

    driver_config = *config;

    return 0;
}

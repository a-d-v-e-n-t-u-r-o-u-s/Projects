/*!
 * \file
 * \brief PWM driver
 * \author Dawid Babula
 * \email dbabula@adventurous.pl
 *
 * \par Copyright (C) Dawid Babula, 2016
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
#ifndef PWM_H
#define PWM_H

/*!
 *
 * \addtogroup pwm
 * \ingroup drivers
 * \brief Driver for PWM
 */

#include "stm8l15x.h"

/*@{*/

struct PWM_handle_t;

typedef enum
{
    PWM_TIMER2_GROUP,
    PWM_TIMER3_GROUP
} PWM_group_t;

typedef enum
{
    PWM_PRESCALER1,
    PWM_PRESCALER2,
    PWM_PRESCALER4,
    PWM_PRESCALER8,
    PWM_PRESCALER16,
    PWM_PRESCALER32,
    PWM_PRESCALER64,
    PWM_PRESCALER128
} PWM_prescaler_t;

typedef struct
{
    PWM_prescaler_t prescaler;
    PWM_group_t group;
    uint16_t counter;
    uint16_t *channel_duty_cycle1;
    uint16_t *channel_duty_cycle2;
} PWM_config_t;

int8_t PWM_attach(struct PWM_handle_t *handle);
int8_t PWM_detach(struct PWM_handle_t *handle);
/*!
 * \brief Configuration function for pwm driver
 */
int8_t PWM_configure(struct PWM_handle_t *handle,
        const PWM_config_t *config,uint8_t is_attached);

/*@}*/
#endif // PWM_H

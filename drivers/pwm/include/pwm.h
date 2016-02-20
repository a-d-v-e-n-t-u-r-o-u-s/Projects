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

/*!
 * \brief Forward declaration for driver handle
 */
struct PWM_handle_t;

/*!
 * \brief Identifies available timer groups
 */
typedef enum
{
    PWM_TIMER2_GROUP,   /*!< PWM group for timer No.2 */
    PWM_TIMER3_GROUP    /*!< PWM group for timer No.3 */
} PWM_group_t;

/*!
 * \brief Identifies available prescaler values
 */
typedef enum
{
    PWM_PRESCALER1,     /*!< devides PWM clock by 1 */
    PWM_PRESCALER2,     /*!< devides PWM clock by 2 */
    PWM_PRESCALER4,     /*!< devides PWM clock by 4 */
    PWM_PRESCALER8,     /*!< devides PWM clock by 8 */
    PWM_PRESCALER16,    /*!< devides PWM clock by 16 */
    PWM_PRESCALER32,    /*!< devides PWM clock by 32 */
    PWM_PRESCALER64,    /*!< devides PWM clock by 64 */
    PWM_PRESCALER128    /*!< devides PWM clock by 128 */
} PWM_prescaler_t;

/*!
 * \brief Configuration structure contains configuration data for given
 * timer group
 */
typedef struct
{
    PWM_prescaler_t prescaler;      /*!< prescaler value */
    PWM_group_t group;              /*!< timer group value */
    uint16_t counter;               /*!< upper counter value */
    uint16_t *channel_duty_cycle1;  /*!< low/high transition counter value for channel No.1
                                    \note it shall be set to NULL if not configured */
    uint16_t *channel_duty_cycle2;  /*!< low/high transition counter value for channel No.2
                                    \note it shall be set to NULL if not configured */
} PWM_config_t;

/*!
 * \brief Attaches PWM driver by enabling PWM clocks, gpios and outputs
 *
 * \param handle handle to driver which shall be attached
 *
 * \retval 0    success
 * \retval -1   handle is NULL
 * \retval -2   driver isn't configured
 * \retval -3   driver is already attached
 */
int8_t PWM_attach(struct PWM_handle_t *handle);

/*!
 * \brief Detaches PWM driver by disabling PWM clocks, gpios and outputs
 *
 * \param handle handle to driver which shall be detached
 *
 * \retval 0    success
 * \retval -1   handle is NULL
 * \retval -2   driver isn't configured
 * \retval -3   driver wasn't attached
 */
int8_t PWM_detach(struct PWM_handle_t *handle);

/*!
 * \brief Configuration function for PWM driver. Setup driver according to configuration
 * data passed and assigns handle in order to allow further actions with driver. It can be
 * called separately for each timer group.
 *
 * \param handle destination handle for further actions
 * \param config configuration data
 * \param is_attached shall driver be attached
 *
 * \retval 0 success
 * \retval -1 configuration data is NULL
 * \retval -2 configuration data is invalid
 */
int8_t PWM_configure(struct PWM_handle_t **handle,
        const PWM_config_t *config,uint8_t is_attached);

/*@}*/
#endif // PWM_H

/*!
 * \file
 * \brief PCB0000 pin assignment file
 * \author Dawid Babula
 * \email dbabula@adventurous.pl
 *
 * \par Copyright (C) Dawid Babula, 2018
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
#ifndef PCB0000_H
#define PCB0000_H

/*!
 *
 * \addtogroup hardware
 * \ingroup RemoteControl
 * \brief Configures pin assignment for PCB0000
 */

/*@{*/

/*!
 * \brief Defines DB4 port for display
 */
#define DISP_D4_PORT            GPIOA

/*!
 * \brief Defines DB4 pin for display
 */
#define DISP_D4_PIN             GPIO_Pin_4

/*!
 * \brief Defines DB5 port for display
 */
#define DISP_D5_PORT            GPIOA

/*!
 * \brief Defines DB5 pin for display
 */
#define DISP_D5_PIN             GPIO_Pin_5

/*!
 * \brief Defines DB6 port for display
 */
#define DISP_D6_PORT            GPIOA

/*!
 * \brief Defines DB6 pin for display
 */
#define DISP_D6_PIN             GPIO_Pin_6

/*!
 * \brief Defines DB7 port for display
 */
#define DISP_D7_PORT            GPIOA

/*!
 * \brief Defines DB7 pin for display
 */
#define DISP_D7_PIN             GPIO_Pin_7

/*!
 * \brief Defines Register select port for display
 */
#define DISP_RS_PORT            GPIOD

/*!
 * \brief Defines Register select pin for display
 */
#define DISP_RS_PIN             GPIO_Pin_4

/*!
 * \brief Defines data read/write port for display
 */
#define DISP_RW_PORT            GPIOD

/*!
 * \brief Defines data read/write pin for display
 */
#define DISP_RW_PIN             GPIO_Pin_5

/*!
 * \brief Defines enable signal port for display
 */
#define DISP_E_PORT             GPIOD

/*!
 * \brief Defines enable signal pin for display
 */
#define DISP_E_PIN              GPIO_Pin_6

#define PWM_TIMER2_CHANNEL1_PORT    GPIOB
#define PWM_TIMER2_CHANNEL1_PIN     GPIO_Pin_0
#define PWM_TIMER2_CHANNEL2_PORT    GPIOB
#define PWM_TIMER2_CHANNEL2_PIN     GPIO_Pin_2

#define PWM_TIMER3_CHANNEL1_PORT    GPIOB
#define PWM_TIMER3_CHANNEL1_PIN     GPIO_Pin_1
#define PWM_TIMER3_CHANNEL2_PORT    GPIOD
#define PWM_TIMER3_CHANNEL2_PIN     GPIO_Pin_0

/*@}*/
#endif /* end of PCB0000_H */

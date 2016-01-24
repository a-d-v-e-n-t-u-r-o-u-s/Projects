/*!
 * \file
 * \brief PCB0000 pin assignment file
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
#ifndef PCB0000_H
#define PCB0000_H

/*!
 *
 * \addtogroup hardware
 * \ingroup WeatherStation
 * \brief Configures pin assignment for PCB0000
 */

/*@{*/

/*!
 * \brief Defines port for display
 */
#define DISP_PORT               GPIOB

/*!
 * \brief Defines DB4 pin for display
 */
#define DISP_D4                 GPIO_Pin_3

/*!
 * \brief Defines DB5 pin for display
 */
#define DISP_D5                 GPIO_Pin_4

/*!
 * \brief Defines DB6 pin for display
 */
#define DISP_D6                 GPIO_Pin_5

/*!
 * \brief Defines DB7 pin for display
 */
#define DISP_D7                 GPIO_Pin_6

/*!
 * \brief Defines Register select pin for display
 */
#define DISP_RS                 GPIO_Pin_0

/*!
 * \brief Defines data read/write pin for display
 */
#define DISP_RW                 GPIO_Pin_1

/*!
 * \brief Defines enable signal pin for display
 */
#define DISP_E                  GPIO_Pin_2

/*!
 * \brief Defines display pins for data bus
 */
#define DISP_DATA_PINS          DISP_D4|DISP_D5|DISP_D6|DISP_D7

/*!
 * \brief Defines display pins for control bus
 */
#define DISP_CTRL_PINS          DISP_RS|DISP_RW|DISP_E

/*!
 * \brief Defines all display pins
 */
#define DISP_PINS_ALL           DISP_CTRL_PINS|DISP_DATA_PINS

/*@}*/
#endif /* end of PCB0000_H */

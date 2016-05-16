#ifndef PWM_INTERNALS_H
#define PWM_INTERNALS_H

/*!
 * \brief Handle structure containing all information about driver
 */
typedef struct PWM_handle_t
{
    uint8_t is_configured;  /*!< indicates is driver configured */
    uint8_t is_attached;    /*!< indicates is driver attached */
    PWM_config_t config;    /*!< holds driver's configuration data */
} PWM_handle_t;

#endif

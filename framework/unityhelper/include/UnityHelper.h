#ifndef _TESTHELPER_H
#define _TESTHELPER_H

#include "pwm.h"
#include "../source/pwm_internals.h"

void AssertEqualPWM_handle_t(const struct PWM_handle_t expected, const struct PWM_handle_t actual, const unsigned short line);

#define UNITY_TEST_ASSERT_EQUAL_PWM_handle_t(expected, actual, line, message) AssertEqualPWM_handle_t(expected, actual, line);

#define TEST_ASSERT_EQUAL_PWM_handle_t(expected, actual) UNITY_TEST_ASSERT_EQUAL_PWM_handle_t(expected, actual, __LINE__, NULL);

#endif // _TESTHELPER_H

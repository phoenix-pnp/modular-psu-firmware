/*
 * EEZ Modular Firmware
 * Copyright (C) 2015-present, Envox d.o.o.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#pragma once

static const uint8_t EXT_TRIG1 = 0;
static const uint8_t EXT_TRIG2 = 1;
static const uint8_t DIN1 = 0;
static const uint8_t DIN2 = 1;
static const uint8_t DOUT1 = 2;
static const uint8_t DOUT2 = 3;
static const uint8_t NUM_IO_PINS = 4;

#include <bb3/uart.h>

namespace eez {
namespace psu {
namespace io_pins {

enum Polarity {
    POLARITY_NEGATIVE,
    POLARITY_POSITIVE
};

enum Function {
    FUNCTION_NONE,
    FUNCTION_INPUT,
    FUNCTION_OUTPUT,
    FUNCTION_FAULT,
    FUNCTION_INHIBIT,
    FUNCTION_ON_COUPLE,
    FUNCTION_SYSTRIG,
    FUNCTION_TOUTPUT,
    FUNCTION_PWM,
    FUNCTION_DLOGTRIG,
    FUNCTION_UART
};

struct IOPin {
    unsigned polarity : 1;
    unsigned function : 6;
    unsigned state : 1;
};

extern IOPin g_ioPins[NUM_IO_PINS];
extern float g_pwmFrequency[NUM_IO_PINS - DOUT1];
extern float g_pwmDuty[NUM_IO_PINS - DOUT1];

#if defined(EEZ_PLATFORM_SIMULATOR)
#define UART_WORDLENGTH_7B          USART_CR1_M1   /*!< 7-bit long UART frame */
#define UART_WORDLENGTH_8B          0x00000000U    /*!< 8-bit long UART frame */
#define UART_WORDLENGTH_9B          USART_CR1_M0   /*!< 9-bit long UART frame */

#define UART_STOPBITS_0_5                    USART_CR2_STOP_0                     /*!< UART frame with 0.5 stop bit  */
#define UART_STOPBITS_1                     0x00000000U                           /*!< UART frame with 1 stop bit    */
#define UART_STOPBITS_1_5                   (USART_CR2_STOP_0 | USART_CR2_STOP_1) /*!< UART frame with 1.5 stop bits */
#define UART_STOPBITS_2                      USART_CR2_STOP_1                     /*!< UART frame with 2 stop bits   */

#define UART_PARITY_NONE                    0x00000000U                        /*!< No parity   */
#define UART_PARITY_EVEN                    USART_CR1_PCE                      /*!< Even parity */
#define UART_PARITY_ODD                     (USART_CR1_PCE | USART_CR1_PS)     /*!< Odd parity  */
#endif

extern uart::UartMode g_uartMode;
extern uint32_t g_uartBaudRate;
extern uint32_t g_uartDataBits;
extern uint32_t g_uartStopBits;
extern uint32_t g_uartParity; // 0 - None, 1 - Even, 2 - Odd

void reset();
void tick();
void onTrigger();
void refresh();

// When PSU is in inhibited state all outputs are disabled and execution of LIST on channels is stopped.
bool isInhibited();

void setPinPolarity(int pin, unsigned polarity);
void setPinFunction(int pin, unsigned function);

void setPinState(int pin, bool state);
bool getPinState(int pin);

static const float PWM_MIN_FREQUENCY = 0.03f;
static const float PWM_MAX_FREQUENCY = 5000000.0f;
static const float PWM_DEFAULT_FREQUENCY = 0.0f;

void setPwmFrequency(int pin, float frequency);
float getPwmFrequency(int pin);

static const float PWM_MIN_DUTY = 0.0f;
static const float PWM_MAX_DUTY = 100.0f;
static const float PWM_DEFAULT_DUTY = 50.0f;

void setPwmDuty(int pin, float duty);
float getPwmDuty(int pin);

int ioPinRead(int pin);
void ioPinWrite(int pin, int state);

bool getIsInhibitedByUser();
void setIsInhibitedByUser(bool isInhibitedByUser);

}
}
} // namespace eez::psu::io_pins

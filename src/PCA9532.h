/*
 * Copyright (C) 2021 Daniel Guedel
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef PCA9532_H
#define PCA9532_H

#include <Wire.h>

// Register definitions (page 6, table 3)
#define REG_INPUT0 0x00 // Input register 0
#define REG_INPUT1 0x01 // Input register 1
#define REG_PSC0   0x02 // Frequency prescaler 0
#define REG_PWM0   0x03 // PWM register 0
#define REG_PSC1   0x04 // Frequency prescaler 1
#define REG_PWM1   0x05 // PWM register 1
#define REG_LS0    0x06 // LED0  to LED3 selector
#define REG_LS1    0x07 // LED4  to LED7 selector
#define REG_LS2    0x08 // LED8  to LED11 selector
#define REG_LS3    0x09 // LED12 to LED15 selector

// Input register 0, INPUT0 (page 6, table 4)
#define BIT_IN_LED7 128 // LED7 state
#define BIT_IN_LED6 64  // LED6 state
#define BIT_IN_LED5 32  // LED5 state
#define BIT_IN_LED4 16  // LED4 state
#define BIT_IN_LED3 8   // LED3 state
#define BIT_IN_LED2 4   // LED2 state
#define BIT_IN_LED1 2   // LED1 state
#define BIT_IN_LED0 0   // LED0 state

// Input register 1, INPUT1 (page 6, table 5)
#define BIT_IN_LED15 128 // LED15 state
#define BIT_IN_LED14 64  // LED14 state
#define BIT_IN_LED13 32  // LED13 state
#define BIT_IN_LED12 16  // LED12 state
#define BIT_IN_LED11 8   // LED11 state
#define BIT_IN_LED10 4   // LED10 state
#define BIT_IN_LED9  2   // LED9  state
#define BIT_IN_LED8  0   // LED8  state

// Frequency Prescaler 0, PCS0 (page 7, table 6)
#define BLINKING_PERIOD_125_MS 18  //  18 =  125ms / (1 / 152Hz) - 1
#define BLINKING_PERIOD_250_MS 37  //  37 =  250ms / (1 / 152Hz) - 1
#define BLINKING_PERIOD_500_MS 75  //  75 =  500ms / (1 / 152Hz) - 1
#define BLINKING_PERIOD_1_S    151 // 151 = 1000ms / (1 / 152Hz) - 1
#define BLINKING_PERIOD_MAX    255 // 255 = 1.69s

// Pulse Width Modulation 0, PWM0 (page 7, table 7)

// Frequency Prescaler 1, PCS1 (page 7, table 8)

// Pulse Width Modulation 1, PWM1 (page 7, table 9)

// LED selector registers, LS0 to LS3 (page 8, table 10)
#define BIT_LS_LED3  6 // LED3  selected
#define BIT_LS_LED2  4 // LED2  selected
#define BIT_LS_LED1  2 // LED1  selected
#define BIT_LS_LED0  0 // LED0  selected
#define BIT_LS_LED7  6 // LED7  selected
#define BIT_LS_LED6  4 // LED6  selected
#define BIT_LS_LED5  2 // LED5  selected
#define BIT_LS_LED4  0 // LED4  selected
#define BIT_LS_LED11 6 // LED11 selected
#define BIT_LS_LED10 4 // LED10 selected
#define BIT_LS_LED9  2 // LED9  selected
#define BIT_LS_LED8  0 // LED8  selected
#define BIT_LS_LED15 6 // LED15 selected
#define BIT_LS_LED14 4 // LED14 selected
#define BIT_LS_LED13 2 // LED13 selected
#define BIT_LS_LED12 0 // LED12 selected

// LED driver output state, LSn (page 8, above table 10)
#define LS_STATE_OFF   0x00 // Output is set high-impedance (LED off; default)
#define LS_STATE_ON    0x01 // Output is set LOW (LED on)
#define LS_STATE_BLNK0 0x02 // Output blinks at PWM0 rate
#define LS_STATE_BLNK1 0x03 // Output blinks at PWM1 rate

class PCA9532 {

/******************************* PUBLIC METHODS *******************************/
public:

    /**
     * Constructor for PCA9532 with PWM1, PWM2
     *
     * @param regPwm1 Register address for pwm channel 1
     * @param regPwm2 Register address for pwm channel 2
     */
    PCA9532(uint8_t regPwm1, uint8_t regPwm2);

    /**
     * Initialization of the PCA9532
     *
     * @param deviceAddress I2C address of the PCA9532
     * @param wire          Reference to TwoWire for I2C communication
     */
    void begin(uint8_t deviceAddress, TwoWire *wire);

    /**
     * Turn on all LEDs. Restores settings saved at turnOff()
     *
     * WARNING: If you call turnOff() twice without calling turnOn() in between,
     *          then the restored state will be LS_STATE_OFF!
     */
    void turnOn();

    /**
     * Turn off all LEDs. Saves current settings for turnOn()
     * For power saving, see sleep()
     *
     * WARNING: If you call turnOff() twice without calling turnOn() in between,
     *          then the restored state will be LS_STATE_OFF!
     */
    void turnOff();

    /**
     * Set PWM value for channels IO_0...IO_7 or IO_8...IO_15
     *
     * @param regPwm Register address for PWM channel
     * @param pwm    PWM value
     */
    void setPwm(uint8_t regPwm, uint8_t pwm);

    /**
     * Set PWM value for all channels
     *
     * @param pwm PWM value
     */
    void setGrpPwm(uint8_t pwm);

    /**
     * Set blinking period for channels IO_0...IO_7 or IO_8...IO_15
     *
     * @param blinkPeriod Period for one blink (turning off and on)
     */
    void setBlinking(uint8_t regPsc, uint8_t blinkPeriod);

    /**
    * Set the LED output state for a given channel. There are four states:
    *   - LS_STATE_OFF
    *   - LS_STATE_ON
    *   - LS_STATE_IND
    *   - LS_STATE_IND_GRP
    *
    * @param state  One of the four possible states
    * @param ldrBit Lower bit of LDR* (see BIT_LDR*)
    */
    void setLsState(uint8_t state, uint8_t regLS, uint8_t lsBit);

    /**
    * Set the LED output state for all channels. There are four states:
    *   - LS_STATE_OFF
    *   - LS_STATE_ON
    *   - LS_STATE_IND
    *   - LS_STATE_IND_GRP
    *
    * @param state One of the four possible states
    */
    void setLsStateAll(uint8_t state);

/****************************** PRIVATE METHODS *******************************/
private:

    /**
     * PWM channels
     */
    uint8_t _regPwm1, _regPwm2;

    /**
     * Stored register content of LS when writing LS_STATE_OFF to all LEDs
     * when calling turnOff()
     */
    uint8_t _storedRegLs0;
    uint8_t _storedRegLs1;
    uint8_t _storedRegLs2;
    uint8_t _storedRegLs3;

    /**
    * Write data to a register
    *
    * @param registerAddress Register address to write to
    * @param data            Data to write
    */
    void writeReg(uint8_t registerAddress, uint8_t data);

    /**
    * Read data from a register
    *
    * @param registerAddress Register address to read from
     *
     * @return byte read from given registerAddress
     * @return -1 if no byte was available to be read
    */
    uint8_t readReg(uint8_t registerAddress);

    /**
     * I2C address of device.
     */
    uint8_t _deviceAddress;

    /**
     * Object for I2C communication
     */
    TwoWire *_wire;
};
#endif //PCA9532_H
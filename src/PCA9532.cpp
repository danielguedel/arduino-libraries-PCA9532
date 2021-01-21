/*
 * Copyright (C) 2021 Daniel Guedel
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "PCA9532.h"

/******************************* PUBLIC METHODS *******************************/


    /**
     * Constructor for PCA9532 with PWM1, PWM2
     *
     * @param regPwm1 Register address for pwm channel 1
     * @param regPwm2 Register address for pwm channel 2
     */
PCA9532::PCA9532(uint8_t regPwm1, uint8_t regPwm2) {

  _regPwm1 = regPwm1;
  _regPwm2 = regPwm2;
}

    /**
     * Initialization of the PCA9532
     *
     * @param deviceAddress I2C address of the PCA9532
     * @param wire          Reference to TwoWire for I2C communication
     */
void PCA9532::begin(uint8_t deviceAddress, TwoWire *wire) {

  _deviceAddress = deviceAddress;

  _wire = wire;
  _wire->begin();
}

    /**
     * Turn on all LEDs. Restores settings saved at turnOff()
     *
     * WARNING: If you call turnOff() twice without calling turnOn() in between,
     *          then the restored state will be LS_STATE_OFF!
     */
void PCA9532::turnOn() {

  writeReg(REG_LS0, _storedRegLs0);
  writeReg(REG_LS1, _storedRegLs1);
  writeReg(REG_LS2, _storedRegLs2);
  writeReg(REG_LS3, _storedRegLs3);
}

    /**
     * Turn off all LEDs. Saves current settings for turnOn()
     * For power saving, see sleep()
     *
     * WARNING: If you call turnOff() twice without calling turnOn() in between,
     *          then the restored state will be LS_STATE_OFF!
     */
void PCA9532::turnOff() {

  _storedRegLs0 = readReg(REG_LS0);
  writeReg(REG_LS0, LS_STATE_OFF);
  _storedRegLs1 = readReg(REG_LS1);
  writeReg(REG_LS1, LS_STATE_OFF);
  _storedRegLs2 = readReg(REG_LS2);
  writeReg(REG_LS2, LS_STATE_OFF);
  _storedRegLs3 = readReg(REG_LS3);
  writeReg(REG_LS3, LS_STATE_OFF);
}

    /**
     * Set PWM value for channels IO_0...IO_7 or IO_8...IO_15
     *
     * @param regPwm Register address for PWM channel
     * @param pwm    PWM value
     */
void PCA9532::setPwm(uint8_t regPwm, uint8_t pwm) {

  writeReg(regPwm, pwm);
}

    /**
     * Set PWM value for all channels
     *
     * @param pwm PWM value
     */
void PCA9532::setGrpPwm(uint8_t pwm) {

  writeReg(REG_PWM0, pwm);
  writeReg(REG_PWM1, pwm);
}

    /**
     * Set blinking period for channels IO_0...IO_7 or IO_8...IO_15
     *
     * @param blinkPeriod Period for one blink (turning off and on)
     */
void PCA9532::setBlinking(uint8_t regPsc, uint8_t blinkPeriod) {

  writeReg(regPsc, blinkPeriod);
}

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
void PCA9532::setLsState(uint8_t state, uint8_t regLs, uint8_t lsBit) {

  uint8_t prevReg = readReg(regLs);
  uint8_t newReg;

  newReg = prevReg & ~(0b11 << lsBit);

  newReg |= (state << lsBit);

  writeReg(regLs, newReg);
}

    /**
    * Set the LED output state for all channels. There are four states:
    *   - LS_STATE_OFF
    *   - LS_STATE_ON
    *   - LS_STATE_IND
    *   - LS_STATE_IND_GRP
    *
    * @param state One of the four possible states
    */
void PCA9532::setLsStateAll(uint8_t state) {

  uint8_t newReg = ( state << BIT_LS_LED3
                   | state << BIT_LS_LED2
                   | state << BIT_LS_LED1
                   | state << BIT_LS_LED0);

  writeReg(REG_LS0, newReg);

  newReg = ( state << BIT_LS_LED7
           | state << BIT_LS_LED6
           | state << BIT_LS_LED5
           | state << BIT_LS_LED4);

  writeReg(REG_LS1, newReg);

  newReg = ( state << BIT_LS_LED11
           | state << BIT_LS_LED10
           | state << BIT_LS_LED9
           | state << BIT_LS_LED8);

  writeReg(REG_LS2, newReg);

  newReg = ( state << BIT_LS_LED15
           | state << BIT_LS_LED14
           | state << BIT_LS_LED13
           | state << BIT_LS_LED12);

  writeReg(REG_LS3, newReg);
}

/****************************** PRIVATE METHODS *******************************/


    /**
    * Write data to a register
    *
    * @param registerAddress Register address to write to
    * @param data            Data to write
    */
void PCA9532::writeReg(uint8_t registerAddress, uint8_t data) {

  _wire->beginTransmission(_deviceAddress);
  _wire->write(registerAddress);
  _wire->write(data);
  _wire->endTransmission();
}

    /**
    * Read data from a register
    *
    * @param registerAddress Register address to read from
     *
     * @return byte read from given registerAddress
     * @return -1 if no byte was available to be read
    */
uint8_t PCA9532::readReg(uint8_t registerAddress) {

  _wire->beginTransmission(_deviceAddress);
  _wire->write(registerAddress);
  _wire->endTransmission();

  _wire->requestFrom(_deviceAddress, (uint8_t) 1);

  if (_wire->available() == 1) {
    return _wire->read();
  }

  return -1;
}
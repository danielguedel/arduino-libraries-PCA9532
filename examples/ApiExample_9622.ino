/*
   Copyright (C) 2021 Daniel Guedel

   This file is subject to the terms and conditions of the GNU Lesser
   General Public License v2.1. See the file LICENSE in the top level
   directory for more details.
*/

#define _DEBUG

#ifdef _DEBUG
  #define _CONSOLE
  #define BAUDRATE 115200
#else
  #undef _CONSOLE
#endif

#include "Arduino.h"
#include "PCA9532.h"
#include "PCA9622.h"

#define PCA9532_RESET_PIN 3 // PCA9532 reset on pin ~D3
#define PCA9532_1_ADDRESS 0x62  // device address of first PCA9532
#define PCA9532_2_ADDRESS 0x63  // device address of second PCA9532
#define PCA9622_ADDRESS 0x18 // device address of PCA9622

PCA9532 pca9532_1 = PCA9532(REG_PWM0, REG_PWM1); // instance of first PCA9532
PCA9532 pca9532_2 = PCA9532(REG_PWM0, REG_PWM1); // instance of second PCA9532
PCA9622 pca9622 = PCA9622(REG_PWM0, REG_PWM1, REG_PWM2); // instance of PCA9622 for one RGB LED channel

void setup() {
  // pin initialization
  pinMode(PCA9532_RESET_PIN, OUTPUT); // initialize the PCA9532 reset pin (as output)
  digitalWrite(PCA9532_RESET_PIN, LOW); // set PCA9532 reset pin to LOW (this resets both PCA9532s)
  delay(1); // PCA9532 reset pulse width 1ms
  digitalWrite(PCA9532_RESET_PIN, HIGH); // set PCA9532 reset pin to HIGH (this releases PCA9532s reset)

  // initialize serial communication
#ifdef _CONSOLE
  Serial.begin(BAUDRATE); // open serial communications with defined baudrate
  // loop not required for debug console only
  while (!Serial) {
   ; // wait until port is open (only necessary for native USB port)
  }
  delay(500);
#endif

  // initialize the first PCA9532
  pca9532_1.begin(PCA9532_1_ADDRESS, &Wire); // initialization of the PCA9532
  pca9532_1.setLsStateAll(LS_STATE_OFF); // PCA9532 output state all channels off
  pca9532_1.setGrpPwm(0); // set global brightness to 0%
  pca9532_1.turnOff(); // turn all LEDs off
  // initialize the second PCA9532
  pca9532_2.begin(PCA9532_2_ADDRESS, &Wire); // initialization of the PCA9532
  pca9532_2.setLsStateAll(LS_STATE_OFF); // PCA9532 output state all channels off
  pca9532_2.setGrpPwm(0); // set global brightness to 0%
  pca9532_2.turnOff(); // turn all LEDs off
  // initialize the PCA9622
  pca9622.begin(PCA9622_ADDRESS, &Wire); // initialization of the PCA9622
  pca9622.setLdrStateAll(LDR_STATE_IND_GRP); // PCA9622 output state all channels individual brightness and group dimming/blinking
  pca9622.setGroupControlMode(GROUP_CONTROL_MODE_DIMMING); // enable group dimming
  pca9622.setGrpPwm(0); // set global brightness to 0%
  pca9622.setRGB(0, 0, 0); // set RGB LED off

#ifdef _CONSOLE
  Serial.println("\nStarting application...");
  Serial.println("Application successfully started, all tasks are running");
#endif
}

void loop() {
  pca9532_1.setGrpPwm(13); // set global brightness to 5%
  /* pca9532_1.setLsState(LS_STATE_OFF, REG_LS0, BIT_LS_LED0); // (not connected)
  delay(500); // wait 500ms */
  pca9532_1.setLsState(LS_STATE_BLNK0, REG_LS0, BIT_LS_LED1); // set LEDs D1-1, D1-2, D1-5 and D1-6 on PWM0
  delay(500); // wait 500ms
  pca9532_1.setLsState(LS_STATE_BLNK0, REG_LS0, BIT_LS_LED2); // set LEDs D2-1 and D2-2 on PWM0
  delay(500); // wait 500ms
  pca9532_1.setLsState(LS_STATE_BLNK0, REG_LS0, BIT_LS_LED3); // set LEDs D3-1 and D3-2 on PWM0
  delay(500); // wait 500ms
  pca9532_1.setLsState(LS_STATE_BLNK0, REG_LS1, BIT_LS_LED4); // set LEDs D4-1 and D4-2 on PWM0
  delay(500); // wait 500ms
  pca9532_1.setLsState(LS_STATE_BLNK0, REG_LS1, BIT_LS_LED5); // set LEDs D5-1 and D5-2 on PWM0
  delay(500); // wait 500ms
  pca9532_1.setLsState(LS_STATE_BLNK0, REG_LS1, BIT_LS_LED6); // set LEDs D6-1 and D6-2 on PWM0
  delay(500); // wait 500ms
  pca9532_1.setLsState(LS_STATE_BLNK0, REG_LS1, BIT_LS_LED7); // set LEDs D7-1 and D7-2 on PWM0
  delay(500); // wait 500ms
  // pca9532_1.setLsState(LS_STATE_BLNK0, REG_LS2, BIT_LS_LED8); // set LEDs D8-1 and D8-2 on PWM0 (not assembled)
  // delay(500); // wait 500ms
  //pca9532_1.setLsState(LS_STATE_BLNK0, REG_LS2, BIT_LS_LED9); // set LEDs D9-1 and D9-2 on PWM0 (not assembled)
  // delay(500); // wait 500ms
  /* pca9532_1.setLsState(LS_STATE_OFF, REG_LS2, BIT_LS_LED10); // (not connected)
  delay(500); // wait 500ms
  pca9532_1.setLsState(LS_STATE_OFF, REG_LS2, BIT_LS_LED11); // (not connected)
  delay(500); // wait 500ms
  pca9532_1.setLsState(LS_STATE_OFF, REG_LS3, BIT_LS_LED12); // (not connected)
  delay(500); // wait 500ms
  pca9532_1.setLsState(LS_STATE_OFF, REG_LS3, BIT_LS_LED13); // (not connected)
  delay(500); // wait 500ms
  pca9532_1.setLsState(LS_STATE_OFF, REG_LS3, BIT_LS_LED14); // (not connected)
  delay(500); // wait 500ms
  pca9532_1.setLsState(LS_STATE_OFF, REG_LS3, BIT_LS_LED15); // (not connected)
  delay(500); // wait 500ms */

  // the folowing lines perform a single 20ms flash with full brightness due to avoid dazzle
  pca9532_1.setGrpPwm(255); // set global brightness to 100%
  delay(20); // wait 20ms
  pca9532_1.setLsStateAll(LS_STATE_OFF); // PCA9532 output state all channels off
  delay(500); // wait 500ms

  pca9532_2.setGrpPwm(13); // set global brightness to 5%
  /* pca9532_2.setLsState(LS_STATE_OFF, REG_LS0, BIT_LS_LED0); // (not connected)
  delay(500); // wait 500ms */
  pca9532_2.setLsState(LS_STATE_BLNK0, REG_LS0, BIT_LS_LED1); // set LEDs D1-3, D1-4, D1-7 and D1-8 on PWM0
  delay(500); // wait 500ms
  pca9532_2.setLsState(LS_STATE_BLNK0, REG_LS0, BIT_LS_LED2); // set LEDs D2-3 and D2-4 on PWM0
  delay(500); // wait 500ms
  pca9532_2.setLsState(LS_STATE_BLNK0, REG_LS0, BIT_LS_LED3); // set LEDs D3-3 and D3-4 on PWM0
  delay(500); // wait 500ms
  pca9532_2.setLsState(LS_STATE_BLNK0, REG_LS1, BIT_LS_LED4); // set LEDs D4-3 and D4-4 on PWM0
  delay(500); // wait 500ms
  pca9532_2.setLsState(LS_STATE_BLNK0, REG_LS1, BIT_LS_LED5); // set LEDs D5-3 and D5-4 on PWM0
  delay(500); // wait 500ms
  pca9532_2.setLsState(LS_STATE_BLNK0, REG_LS1, BIT_LS_LED6); // set LEDs D6-3 and D6-4 on PWM0
  delay(500); // wait 500ms
  pca9532_2.setLsState(LS_STATE_BLNK0, REG_LS1, BIT_LS_LED7); // set LEDs D7-3 and D7-4 on PWM0
  delay(500); // wait 500ms
  // pca9532_2.setLsState(LS_STATE_BLNK0, REG_LS2, BIT_LS_LED8); // set LEDs D8-3 and D8-4 on PWM0 (not assembled)
  // delay(500); // wait 500ms
  // pca9532_2.setLsState(LS_STATE_BLNK0, REG_LS2, BIT_LS_LED9); // set LEDs D9-3 and D9-4 on PWM0 (not assembled)
  // delay(500); // wait 500ms
  /* pca9532_2.setLsState(LS_STATE_OFF, REG_LS2, BIT_LS_LED10); // (not connected)
  delay(500); // wait 500ms
  pca9532_2.setLsState(LS_STATE_OFF, REG_LS2, BIT_LS_LED11); // (not connected)
  delay(500); // wait 500ms
  pca9532_2.setLsState(LS_STATE_OFF, REG_LS3, BIT_LS_LED12); // (not connected)
  delay(500); // wait 500ms
  pca9532_2.setLsState(LS_STATE_OFF, REG_LS3, BIT_LS_LED13); // (not connected)
  delay(500); // wait 500ms
  pca9532_2.setLsState(LS_STATE_OFF, REG_LS3, BIT_LS_LED14); // (not connected)
  delay(500); // wait 500ms
  pca9532_2.setLsState(LS_STATE_OFF, REG_LS3, BIT_LS_LED15); // (not connected)
  delay(500); // wait 500ms */

  // the folowing lines perform a single 20ms flash with full brightness due to avoid dazzle
  pca9532_2.setGrpPwm(255); // set global brightness to 100%
  delay(20); // wait 20ms
  pca9532_2.setLsStateAll(LS_STATE_OFF); // PCA9532 output state all channels off
  delay(500); // wait 500ms

  pca9622.setGrpPwm(13); // set global brightness to 5%
  pca9622.setRGB(255, 0, 0); // set RGB LED color red
  delay(500); // wait 500ms
  pca9622.setRGB(0, 255, 0); // set RGB LED color green
  delay(500); // wait 500ms
  pca9622.setRGB(0, 0, 255); // set RGB LED color blue
  delay(500); // wait 500ms
  pca9622.setRGB(255, 255, 255); // set RGB LED color white
  delay(500); // wait 500ms

  // the folowing lines perform a single 20ms flash with full brightness due to avoid dazzle
  pca9622.setGrpPwm(255); // set global brightness to 100%
  delay(20); // wait 20ms
  pca9622.setGrpPwm(0); // set global brightness to 0%
  delay(500); // wait 500ms
}
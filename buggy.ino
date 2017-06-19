/* 
 * httpRC webserver firmware for ESP8266 (ESP-01)for PiKoder/SSC wRX 
 *
 * Copyright 2017 Gregor Schlechtriem
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "buggy.h"

void setupBuggy()
{
  stopBuggy();
}

void stopBuggy()
{
  Serial.write(0xFF);
  Serial.write((byte) 0x00);
  Serial.write((byte) 0x7F);
  Serial.write((byte) 0xFF);
  Serial.write((byte) 0x01);
  Serial.write((byte) 0x7F);
}

/* Routine that updates 4 PWM motor drivers based on
 * position of a virtual joystick.
 * 
 * direction: -100 - 100
 * throttle: -100 - 100
 */
void moveBuggy(int direction, int throttle)
{
  int movingForward = (throttle > 0);

  if(!movingForward)
    throttle = - throttle; // we should go in reverse -> flip the sign
  if(throttle < THROTTLE_THRESHOLD)
  {
    stopBuggy();
    return;
  }

  int throttlePWM = map(throttle, THROTTLE_THRESHOLD, THROTTLE_MAX, PWM_MIN, PWM_MAX);
  if(movingForward)
  {
    throttlePWM = 0x7F + throttlePWM;
  }
  else {
    throttlePWM = 0x7F - throttlePWM;
  }       

  int correctSign = (direction > 0);
  if(!correctSign)
    direction = - direction; // we should go in reverse -> flip the sign
  if(direction < DIRECTION_THRESHOLD)
  {
    direction = DIRECTION_THRESHOLD;
  }
  int directionPWM = map(direction, DIRECTION_THRESHOLD, THROTTLE_MAX, PWM_MIN, PWM_MAX);
  if(correctSign)
  {
    directionPWM = 0x7F + directionPWM;
  }
  else {
    directionPWM = 0x7F - directionPWM;
  }       

  Serial.write((byte) 0xFF);
  Serial.write((byte) 0x00);
  Serial.write((byte) directionPWM);
  Serial.write((byte) 0xFF);
  Serial.write((byte) 0x01);
  Serial.write((byte) throttlePWM);
}


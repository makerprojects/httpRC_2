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

#define THROTTLE_THRESHOLD 30
#define THROTTLE_MAX 100
#define PWM_MIN 0     
#define PWM_MAX 0x7E  
#define DIRECTION_THRESHOLD 30
#define FULL_PWM_FACTOR 2.55

void setupBuggy();
void stopBuggy();
void moveBuggy(int direction, int throttle);

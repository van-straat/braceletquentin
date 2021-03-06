/*****************************************************************************
  BH1790GLC.ino

 Copyright (c) 2016 ROHM Co.,Ltd.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
******************************************************************************/
#include <Wire.h>
#include <BH1790GLC.h>
#include <FlexiTimer2.h>
#include <stdio.h>

BH1790GLC bh1790glc;
volatile bool timer_flg;
char str[100];

void timer_isr(void);

void setup() {
  byte rc;
  
  timer_flg = false;
  Serial.begin(115200);
  while (!Serial);

  Wire.begin();
  Wire.setClock(400000L);
  
  rc = bh1790glc.init();
  if (rc != 0) {
    Serial.println("BH1790 initialize failed");
  } else {
    Serial.println("LEDON Data, LEDOFF Data");
    FlexiTimer2::stop();
    FlexiTimer2::set(250, 1.0/8000, timer_isr);  // 32Hz timer
    FlexiTimer2::start();
  }
}

void loop() {
  byte rc;
  unsigned short val[2];
 
  if (timer_flg) {
    rc = bh1790glc.get_val(val);
    if (rc == 0) {
      sprintf(str, "%d, %d\n", val[1], val[0]);
      Serial.print(str);
    }
    timer_flg = false;
  }

}

void timer_isr(void) {
  timer_flg = true;
}

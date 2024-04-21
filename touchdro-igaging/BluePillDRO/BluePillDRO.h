#ifndef QUADRATUREBLUEPILL_H
#define QUADRATUREBLUEPILL_H

#include "Arduino.h"

/*
  Copyright 2018 Chris Albertson 
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License (LGPL) 
  as published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define QUAD_TIMER_1  1
#define QUAD_TIMER_3  3
#define QUAD_TIMER_4  4

#define PPR 65535

static HardwareTimer *timer[5];
static long ints[5] = {0, 0, 0, 0, 0};


class quadCounter {
  public:

    quadCounter(int timer_number) {

      if (timer_number_valid(timer_number)) {
        _timer_number = timer_number;
      }
      else {
        // We shoud signal an error somehow
        return;
      }
      

      timer[_timer_number] = new HardwareTimer(timer_number);

      //define the Timer channels as inputs.
      pinMode(pinA[timer_number], INPUT_PULLUP);  //channel A
      pinMode(pinB[timer_number], INPUT_PULLUP);  //channel B

      timer[_timer_number]->setMode(1, TIMER_ENCODER); //set mode, the channel is not used when in this mode.
      timer[_timer_number]->pause(); //stop...
      timer[_timer_number]->setPrescaleFactor(1); //normal for encoder to have the lowest or no prescaler.
      timer[_timer_number]->setOverflow(PPR);     //match the number of pulse per revolution of the encoder.
      timer[_timer_number]->setCount(0);          //reset the counter.

      // Set the number of channels used.
      //    TIMER_SMCR_SMS_ENCODER3 will process both chanles and give four counts per cycle
      //    TIMER_SMCR_SMS_ENCODER1 use channel 1 only, gives two counts per cycle
      //    TIMER_SMCR_SMS_ENCODER2 use channel 2 only, gives two counts per cycle
      timer[_timer_number]->setEdgeCounting(TIMER_SMCR_SMS_ENCODER3);
      switch(_timer_number) {
        case QUAD_TIMER_1:  timer_attach_interrupt(TIMER1, 0, handler_tim1);
                            break;
        case QUAD_TIMER_3:  timer_attach_interrupt(TIMER3, 0, handler_tim3);
                            break;
        case QUAD_TIMER_4:  timer_attach_interrupt(TIMER4, 0, handler_tim4);
                            break;
      }
      timer[_timer_number]->resume();                 //start the encoder...

    }

    int count() {
      return timer[_timer_number]->getCount();
    }

    void resetCount() {
      timer[_timer_number]->setCount(0);
    }

  private:

    int _timer_number;

    int timer_number_valid(int timer_number) {
      
      int rtnCode;
      
      if (timer_number == QUAD_TIMER_1 ||
          timer_number == QUAD_TIMER_3 ||
          timer_number == QUAD_TIMER_4   ) {
            
        rtnCode = 1;
      }
      else {
        rtnCode = 0;
      }
      return rtnCode; 
    }
    


    static void handler_tim1(void){
      if (timer[1]->getDirection()) {
        ints[1]--;
      } else {
        ints[1]++;
      }
    }
    static void handler_tim3(void) {
      if (timer[3]->getDirection()) {
        ints[3]--;
      } else {
        ints[3]++;
      }
    }
    static void handler_tim4(void) {
      if (timer[4]->getDirection()) {
        ints[4]--;
      } else {
        ints[4]++;
      }
    }
    


    /*
      Pins associated with each timer are
      HardwareTimer timer(1); PA8-9
      HardwareTimer timer(3); PA6-7
      HardwareTimer timer(4); PB6-7
    */

    // Arrary is set up so timmer_number is a direct index.  timmer_number
    // can equal only 1, 3 or 4.
    byte pinA[5] = {0, PA8, 0, PA6, PB6};
    byte pinB[5] = {0, PA9, 0, PA7, PB7};
};


#endif /* QUADRATUREBLUEPILL_H */

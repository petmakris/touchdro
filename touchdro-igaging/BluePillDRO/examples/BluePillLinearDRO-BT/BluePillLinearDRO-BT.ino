
#include "BluePillDRO.h"

/*
Pins associated with each timer are
HardwareTimer timer(1); PA8-9 
HardwareTimer timer(3); PA6-7 
HardwareTimer timer(4); PB6-7 
*/

/*
Pins associated with Serial2 UART
TX ; PA2 
RX ; PA3
*/

quadCounter  QC1(QUAD_TIMER_1);
quadCounter  QC3(QUAD_TIMER_3);
quadCounter  QC4(QUAD_TIMER_4);


void setup() {
  
  Serial2.begin(9600);
}


void loop() {

  Serial2.print("X");
  Serial2.print(ints[1]*65536+QC1.count());
  Serial2.print(";");
  Serial2.print("Y");
  Serial2.print(ints[3]*65536+QC3.count());
  Serial2.print(";");
  Serial2.print("Z");
  Serial2.print(ints[4]*65536+QC4.count());
  Serial2.print(";");

  delay(100);
  }

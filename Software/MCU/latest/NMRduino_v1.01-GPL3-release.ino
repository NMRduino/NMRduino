/*  NMRduino open core version 1.01
*   Compatible with Teensy 4.1 and NMRduino UI version(s) 3.34.3
*   Copyright (C) 2024, by Michael Tayler and Sven Bodenstedt
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

enum State {State_Idle, State_Init, State_Sequence, State_Acquire, State_Transfer, State_GPIO_change, State_acq_get, State_acq_rtn, State_dac_rtn, State_acq_point_init}; //, State_Flush};
volatile State state = State_Idle;
#include "serial.h"

void setup(){ 
  state=State_Idle;
  Serial_init();
  }
  
void loop(){
switch (state)
  {
  case State_Idle:
    while(readSerialData()){}
    state = State_Init;
  break;
      
  case State_Init: 
    seq_run();
  break;

  case State_Transfer:
    transfer_data(n_data_points);
    delay(1);
    state=State_Idle;
  break;

  default: 
  break;
  }
}

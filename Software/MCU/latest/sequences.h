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

#ifndef sequences_h
#define sequences_h

#define MAX_NUMBER_EVENTS  15000
unsigned int seq_sampleperiod;
uint8_t seq_acquisition_mode;
uint32_t gpio_time[MAX_NUMBER_EVENTS];   // Event time stamps
uint32_t gpio_clr[MAX_NUMBER_EVENTS];    // Event GPIO clear register bits
uint32_t gpio_set[MAX_NUMBER_EVENTS];    // Event GPIO set register bits
uint32_t *gpio_time_ptr, *gpio_clr_ptr, *gpio_set_ptr;
uint32_t n_events, n_data_points;
volatile uint32_t gpiocounter, global_data_counter, local_data_counter, n_local_data_points, n_us_acq;
volatile bool end_event_flag, end_data_flag, acq_pt_available;
volatile byte cword;

#include "MCP4822.h"
#include "LTC1859.h"
#include "QTM.h"

void seq_run(){  
  switch(seq_acquisition_mode) {  
    case 0x0A:                    // Spectrum analyzer.    
      while (Serial.available() > 0){Serial.read();} // read_all();
      ADC_init_5V();
      qtm_init_sa(); qtm_start_sa();
      while(!Serial.available()){
        if(acq_pt_available){
        acq_pt_available = false;
        Serial.write((uint8_t) ((data_point >> 8) & 0xFF));
        Serial.write((uint8_t) ((data_point     ) & 0xFF));
        }     
      }
      qtm_stop();
      acq_pt_available = false;
      Serial.write(0);  Serial.send_now();  Serial.flush();
      delay(100);
      state=State_Idle;
    break; 
  
    case 0x0C:                    // Pulse sequence event list
         gpio_time_ptr = gpio_time;    
         gpio_set_ptr  = gpio_set;      
         gpio_clr_ptr  = gpio_clr;
         ADC_init_5V();
         qtm_init(); qtm_start();
    break;
  }
}

#endif // sequences_h

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

#ifndef serial_h
#define serial_h

#include <SPI.h>
#include "sequences.h"

byte serialData[64];
const byte bits_per_event = 8;  
const byte overhead_bits = 8;

void Serial_init() {
  Serial.begin(115200);  //start Serial  
  SPI_init();
  delay(10);
  pinMode(23,OUTPUT);   digitalWriteFast(23,LOW); // D0
  pinMode(22,OUTPUT);   digitalWriteFast(22,LOW);
  pinMode(21,OUTPUT);   digitalWriteFast(21,LOW); // D1
  pinMode(20,OUTPUT);   digitalWriteFast(20,LOW); 
  pinMode(19,OUTPUT);   digitalWriteFast(19,LOW); // D2
  pinMode(18,OUTPUT);   digitalWriteFast(18,LOW);
  pinMode(15,OUTPUT);   digitalWriteFast(15,LOW); // A0
  pinMode(14,OUTPUT);   digitalWriteFast(14,LOW); 
  pinMode(41,OUTPUT);   digitalWriteFast(41,LOW); // A1
  pinMode(40,OUTPUT);   digitalWriteFast(40,LOW);  
  dac_init(); 
}

void print_error(byte err){
  Serial.write(err);
  Serial.send_now();
}

void read_all(){ while (Serial.available() > 0){Serial.read();}}
  
int readSerialData(){
  while (Serial.available() == 0){ }

  if (Serial.readBytes((char*)serialData, 1) != 1){print_error(1); return 1;}   

  else if (serialData[0] == 0x00){print_error(0); return 1;} 

  if (serialData[0] != 0xA5){read_all(); print_error(1); return 1;} 

  if (Serial.readBytes((char*)serialData+1, 2) != 2){read_all(); print_error(1); return 1;} 

  size_t serialDataLength;

  if (serialData[1] < 0x80){serialDataLength = 18;}  else {serialDataLength = serialData[2];}
  if (serialDataLength > 64){print_error(1); return 1;}
  
  if (Serial.readBytes((char*)(serialData+3), serialDataLength-3) != (serialDataLength-3)){read_all(); print_error(1); return 1;}
  else if(serialData[serialDataLength-1] != 0x5A){read_all(); print_error(1); return 1;} 

  if (serialData[1] >= 0x80){print_error(0);}
  if (serialData[1] <= 0x80){
    n_data_points   = serialData[2]*512; // number of data points times 512
    seq_sampleperiod        = (serialData[10] << 8) + serialData[11];
    seq_acquisition_mode    = serialData[1]; 
    return 0;
  }  
  if(serialData[1] == 0x81){
    n_events = (serialData[3] << 8) + serialData[4]; 
    unsigned int currentevent = (serialData[5] << 8) + serialData[6]; 
    unsigned int n_currentevent = (serialDataLength - overhead_bits) / bits_per_event; // number of events in current serial command
    unsigned int dataIndex, pulseIndex;

    if ((serialDataLength - overhead_bits) % bits_per_event){return 1;} 
    else if (n_events > MAX_NUMBER_EVENTS - 1) {return 1;} 
    else if((currentevent + n_currentevent) > n_events){return 1;}  
    for (unsigned int i=0; i < n_currentevent; i++){
      pulseIndex = currentevent + i;
      dataIndex = (overhead_bits - 1) + i*bits_per_event;
      
      gpio_time[pulseIndex] = (  (serialData[dataIndex + 0] << 24) +
                                 (serialData[dataIndex + 1] << 16) +
                                 (serialData[dataIndex + 2] <<  8) +
                                 (serialData[dataIndex + 3]      )  );
      gpio_set[pulseIndex] =  (  (serialData[dataIndex + 4] << 24) +
                                 (serialData[dataIndex + 5] << 16) +
                                 (serialData[dataIndex + 6] <<  8) +
                                 (serialData[dataIndex + 7]      )  );  
      gpio_clr[pulseIndex] =  (~((serialData[dataIndex + 4] << 24) +
                                 (serialData[dataIndex + 5] << 16) +
                                 (serialData[dataIndex + 6] <<  8)) );                                      
    }

    return 1;
  }

  return 1;
}

#endif // serial_h

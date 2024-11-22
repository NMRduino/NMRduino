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

#define DAC_SS 10
#define DAC_LDVAL 9
#define write_dac_A0 0xB000
#define write_dac_A1 0x3000

volatile uint16_t dac_value_A0, dac_value_A1;

void dac_init() {
  pinMode(DAC_LDVAL, OUTPUT);  digitalWriteFast(DAC_LDVAL, HIGH);
  pinMode(DAC_SS, OUTPUT);  digitalWriteFast(DAC_SS, HIGH);
}

void dac_write(uint16_t dac_data, uint16_t dac_channel){
  uint16_t dac_bytes = (dac_channel | (dac_data & 0xFFF));
  digitalWriteFast(DAC_SS, LOW);
  SPI.transfer16(dac_bytes); 
  digitalWriteFast(DAC_SS, HIGH);
}

void dac_ldval(){
  digitalWriteFast(DAC_LDVAL, LOW);
  asm volatile("nop\n nop\n nop\n nop");
  digitalWriteFast(DAC_LDVAL, HIGH);
}

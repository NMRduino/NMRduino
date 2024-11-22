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

#ifndef LTC1859_h
#define LTC1859_h

#define ADC_SS 8
#define CONVST 6
#define ADCBYTES_CH0_5V   (B10000000 << 8)  //Non-amplified input
#define ADCBYTES_CH0_10V  (B10000100 << 8)  //Non-amplified input
#define ADCBYTES_CH1_5V   (B10010000 << 8)  //Amplified input
#define ADCBYTES_CH1_10V  (B10010100 << 8)  //Amplified input

volatile uint16_t acq_data[2*65536];        // array for data storage
const uint16_t ADCBYTES_5V = ADCBYTES_CH0_5V;
const uint16_t ADCBYTES_10V = ADCBYTES_CH0_10V;
volatile uint16_t adc_byte, data_point;

void SPI_init(){
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
}

static inline uint16_t acq_pt(uint16_t) __attribute__((always_inline));
static inline uint16_t acq_pt(uint16_t adc_bytes){
  digitalWriteFast(ADC_SS, LOW);  
  data_point = SPI.transfer16(adc_bytes); 
  digitalWriteFast(ADC_SS,HIGH);
  return data_point;
}

static inline uint16_t acq_pt_10V() __attribute__((always_inline));
static inline uint16_t acq_pt_10V(){
  return acq_pt(ADCBYTES_10V);
}

static inline uint16_t acq_pt_5V() __attribute__((always_inline));
static inline uint16_t acq_pt_5V(){
  return acq_pt(ADCBYTES_5V);
}

void ADC_init(uint16_t adc_bytes){
  pinMode(ADC_SS,OUTPUT); digitalWriteFast(ADC_SS,HIGH);
  pinMode(CONVST,OUTPUT); digitalWriteFast(CONVST,LOW);
  delayMicroseconds(2);
  digitalWriteFast(CONVST, HIGH);
  delayMicroseconds(6);
  digitalWriteFast(CONVST, LOW);
  acq_pt(adc_bytes);
}

void ADC_init_10V() {ADC_init(ADCBYTES_10V);}
void ADC_init_5V() {ADC_init(ADCBYTES_5V);}

void transfer_data(unsigned int data_length){
  for(unsigned int i=0; i < data_length; i++){ 
    Serial.write((uint8_t) ((acq_data[i] >> 8) & 0xFF));
    Serial.write((uint8_t) ((acq_data[i]     ) & 0xFF));
  }
  Serial.flush();
}

#endif // LTC1859_h

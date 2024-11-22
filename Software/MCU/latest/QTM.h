/*  NMRduino open core version 1.01. 
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

#ifndef QTM_h
#define QTM_h
#define cwADC  0x0A
#define cwDAC  0x0D
#define cwGPIO 0x00

void qtm_stop() { 
    TMR4_CSCTRL0 = 0; TMR4_SCTRL0 = 0; TMR4_CTRL0 = 0; 
    TMR4_CSCTRL1 = 0; TMR4_SCTRL1 = 0; TMR4_CTRL1 = 0; 
    TMR4_CSCTRL2 = 0; TMR4_SCTRL2 = 0; TMR4_CTRL2 = 0; 
    NVIC_DISABLE_IRQ(IRQ_QTIMER4);
    detachInterrupt(IRQ_QTIMER4);
    asm volatile ("dsb");
}

FASTRUN void nmrduino_isr() {
  TMR4_CSCTRL2 &= ~(TMR_CSCTRL_TCF2);
   if(state==State_acq_get){
        local_data_counter++;
        acq_data[global_data_counter] = acq_pt_5V();
        global_data_counter++;
        if(local_data_counter < n_local_data_points){
          n_us_acq = ( (*gpio_set_ptr >> 4) & 0xFFF ); // dwell time between data points    
          TMR4_COMP12 = (750 * n_us_acq) - 1; 
          TMR4_COMP22 = (750 * n_us_acq) - 1;        
          asm volatile ("dsb"); return;}
        else{  
          if(global_data_counter==n_data_points){end_data_flag = true;}
          state = State_acq_rtn; asm volatile ("dsb"); return;}
   }
   else if(state==State_acq_rtn){
          gpiocounter+=((n_us_acq * 10 * n_local_data_points) + 4);
          state=State_Acquire;
          TMR4_COMP12 = 75 - 1;  
          TMR4_COMP22 = 75 - 1;
          gpio_time_ptr++; gpio_clr_ptr++; gpio_set_ptr++;     
   }
   else if(state==State_dac_rtn){
          GPIO6_DR_CLEAR |= 0x02000000;
          state=State_Acquire; 
          gpiocounter+= 6;
          TMR4_COMP12 = 75 - 1;  
          TMR4_COMP22 = 75 - 1;
          digitalWriteFast(DAC_LDVAL, HIGH);
          gpio_time_ptr++; gpio_clr_ptr++; gpio_set_ptr++;
          asm volatile ("dsb"); return;
   }
      
   else if(gpiocounter >= *gpio_time_ptr)  // New event
   { 
    cword = ( (uint8_t) (*gpio_set_ptr & 0x0000000F) );
    if(cword==cwADC) {
        n_local_data_points = ( ((*gpio_set_ptr >> 16)+1) & 0xFFFF );
        local_data_counter = 0;
        TMR4_COMP12 = 375 - 1; 
        TMR4_COMP22 = 375 - 1;
        state=State_acq_get;
    }
    else if(cword==cwDAC){
        GPIO6_DR_SET |= 0x02000000;
        TMR4_COMP12 = 375 - 1; 
        TMR4_COMP22 = 375 - 1; 
        dac_value_A1 = (write_dac_A1 | ( (*gpio_set_ptr >> 20) & 0x0FFF ));
        dac_value_A0 = (write_dac_A0 | ( (*gpio_set_ptr >>  8) & 0x0FFF ));
        digitalWriteFast(DAC_SS, LOW);
        SPI.transfer16(dac_value_A1); 
        digitalWriteFast(DAC_SS, HIGH);
        asm volatile("nop\n nop\n nop\n nop");
        digitalWriteFast(DAC_SS, LOW);
        SPI.transfer16(dac_value_A0); 
        digitalWriteFast(DAC_SS, HIGH);
        digitalWriteFast(DAC_LDVAL, LOW);
        state = State_dac_rtn;
        asm volatile ("dsb"); return;
    }
    else {
    GPIO6_DR_CLEAR |= *gpio_clr_ptr++; 
    GPIO6_DR_SET   |= *gpio_set_ptr++;   
    gpio_time_ptr++;
    }      
   }   

   else if(gpiocounter>=gpio_time[n_events-1]){end_event_flag=true;}
   
   gpiocounter++;
   if(end_event_flag && end_data_flag){state = State_Transfer; qtm_stop();}
   asm volatile ("dsb");
}

FASTRUN void isr_sa() {
  TMR4_CSCTRL1 &= ~(TMR_CSCTRL_TCF2 | TMR_CSCTRL_TCF1);
  acq_pt_5V(); 
  acq_pt_available = true; 
  asm volatile ("dsb");
}

void qtm_start(){
  TMR4_CNTR0 = 0;   TMR4_LOAD0 = 0;
  TMR4_CNTR1 = 899; TMR4_LOAD1 = 0;
  TMR4_CNTR2 = 0;   TMR4_LOAD2 = 0;
  end_data_flag=false;
  end_event_flag=false;
  gpiocounter=1;
  global_data_counter=0;
  state=State_Acquire;
  for(uint32_t i=0; i<n_data_points;i++){acq_data[i]=0;}
  TMR4_CSCTRL2 |= TMR_CSCTRL_TCF2EN | TMR_CSCTRL_TCF2; 
  NVIC_ENABLE_IRQ(IRQ_QTIMER4);
}

void qtm_start_sa(){
  TMR4_CNTR1 = 899; TMR4_LOAD1 = 0;
  acq_pt_available=false;
  state=State_Acquire;
  TMR4_CSCTRL1 |= TMR_CSCTRL_TCF2EN | TMR_CSCTRL_TCF2; 
  NVIC_ENABLE_IRQ(IRQ_QTIMER4);
}

void qtm_init() {
  gpiocounter=0;
  TMR4_CTRL0 = 0; TMR4_CNTR0 = 0; TMR4_LOAD0 = 0; 
  TMR4_CTRL1 = 0; TMR4_CNTR1 = 0; TMR4_LOAD1 = 0; 
  TMR4_CTRL2 = 0; TMR4_CNTR2 = 0; TMR4_LOAD2 = 0;
  TMR4_COMP10 = 75 - 1;    TMR4_COMP20 = 75 - 1;  
  TMR4_COMP11 = 900 - 1;   TMR4_COMP21 = 600 - 1;       
  TMR4_COMP12 = 75 - 1;    TMR4_COMP22 = 75 - 1;
  TMR4_SCTRL0 = TMR_SCTRL_OEN; 
  TMR4_SCTRL1 = TMR_SCTRL_OEN; 
  TMR4_SCTRL2 = TMR_SCTRL_OEN; 
  TMR4_CTRL0 = TMR_CTRL_CM(1) | TMR_CTRL_PCS(8) | TMR_CTRL_LENGTH | TMR_CTRL_OUTMODE(4); 
  TMR4_CTRL1 = TMR_CTRL_CM(1) | TMR_CTRL_PCS(8) | TMR_CTRL_LENGTH | TMR_CTRL_OUTMODE(4);
  *(portConfigRegister(6)) = 1;  
  TMR4_CTRL2 = TMR_CTRL_CM(1) | TMR_CTRL_PCS(8) | TMR_CTRL_LENGTH | TMR_CTRL_OUTMODE(4); 
  *(portConfigRegister(9)) = 1;    
  attachInterruptVector(IRQ_QTIMER4, nmrduino_isr);
  NVIC_SET_PRIORITY(IRQ_QTIMER4, 0);
}

void qtm_init_sa() {  
  TMR4_CTRL1 = 0; TMR4_CNTR1 = 0; TMR4_LOAD1 = 0;
  TMR4_COMP11 = seq_sampleperiod - 600 - 1;   TMR4_COMP21 = 600 - 1;     
  TMR4_SCTRL1 = TMR_SCTRL_OEN; 
  TMR4_CTRL1 = TMR_CTRL_CM(1) | TMR_CTRL_PCS(8) | TMR_CTRL_LENGTH | TMR_CTRL_OUTMODE(4);
  *(portConfigRegister(6)) = 1;  
  attachInterruptVector(IRQ_QTIMER4, isr_sa);
  NVIC_SET_PRIORITY(IRQ_QTIMER4, 0);  
}

#endif // QTM_h

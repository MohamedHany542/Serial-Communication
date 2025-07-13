#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/resets.h"
#include <string.h>
#include "LCD.h"

char received_message[16]; 
char current_byte = 0;
int byte_index = 0, array_index = 0;
bool parity = false, update = false;

void receive(uint, uint32_t);
void shift_reg(int data);
int calc_checksum();

LCD lcd(13, 14, 12, 8, 9, 10, 11, false);

int main(){
 stdio_init_all();
 gpio_init(16);
 gpio_set_dir(16, false);
 gpio_init(18);
 gpio_set_dir(18, true);
 gpio_init(20);
 gpio_set_dir(20, true);
 gpio_init(22);
 gpio_set_dir(22, true);
 gpio_init(21);
 gpio_set_dir(21, false);
 gpio_init(26);
 gpio_set_dir(26, true);
 lcd.init();
 gpio_set_irq_enabled_with_callback(16, GPIO_IRQ_EDGE_RISE, true, &receive);
 while (true)
 {
  if (update){
  lcd.nocursor();
  lcd.setaddress(0,0);
  lcd.print(received_message);
  lcd.setaddress(1,0);
  for(int i=0; i<8; i++){
    if(i<byte_index){
       lcd.print((char*)((current_byte & (0x80>>i))? "1" : "0"));
    }
    else{
       lcd.print(" ");
    }
  }
  lcd.setaddress(0, strlen(received_message));
  lcd.cursor();
  shift_reg(calc_checksum());
  update = false;
 }
}
}

void receive(uint irq_source, uint32_t){
bool rx_bit = gpio_get(21);
  if(byte_index == 8){
    array_index++;
    byte_index = 0;
    current_byte = 0;
   }
  if(byte_index < 8){
    current_byte |= rx_bit << (7-byte_index);
    byte_index++;
   }
  if(byte_index == 8){
    received_message[array_index] = current_byte;
   }
   update = true;
}

int calc_checksum (){
  int checksum = 0;
  for(int i=0; i<strlen(received_message); i++){
      checksum += received_message[i];
  }
  const int carry = checksum>>8;
  checksum = checksum&0xff;
  return ~(checksum+ carry);
}

void shift_reg(int data){
for(int i=0; i<8; i++){  
  gpio_put(18, (data & (0x01<<i)));
  gpio_put(20, true);
  busy_wait_us(2);
  gpio_put(20, false);
  busy_wait_us(2);
 }
}
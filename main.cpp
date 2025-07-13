#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include <string.h>
#include "LCD.h"



char received_message[16]; 
char current_byte = 0;
int byte_index = 0, array_index = 0;
bool parity = false;

void receive(uint, uint32_t);


LCD lcd(13, 14, 12, 8, 9, 10, 11, false);

int main(){
 stdio_init_all();
 gpio_init(16);
 gpio_set_dir(16, false);
 gpio_init(21);
 gpio_set_dir(21, false);
 gpio_init(26);
 gpio_set_dir(26, true);
 lcd.init();
 gpio_set_irq_enabled_with_callback(16, GPIO_IRQ_EDGE_RISE, true, &receive);
 while (true)
 {
 }
 
}

void receive(uint, uint32_t){
   if(byte_index == 8){
    array_index++;
    byte_index = 0;
    current_byte = 0;
   }
   if(byte_index<8){
    current_byte |=  gpio_get(21) << (7-byte_index);
    byte_index++;
   }
  if(byte_index == 8){
    received_message[array_index] = current_byte;
   }

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
  
}

/*Parity code*/
/*   
 if(gpio_get(21)){
      parity = parity ^ gpio_get(21);
    }
  gpio_put(26, parity);
*/
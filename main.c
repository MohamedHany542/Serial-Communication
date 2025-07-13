#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>

int RS   = 0b000000000000000001000000000000;
int RW   = 0b000000000000000000100000000000;
int E    = 0b000000000000000010000000000000;
int data = 0b000000000000000000001111000000;

int main() {
 stdio_init_all();
 gpio_init_mask(RS|RW|E|data);
 gpio_set_dir_out_masked(RS|RW|E|data);
 
 char message[20] = "hello, world!"; 
 
 LCDinit();
 //printlcd(message);
 while(true){
    printlcd(message);
 }
}

void writelcd(){
  gpio_put(13, 1);
  gpio_put(13, 0);
}

void printlcd(char pr[]){
 for (int i=0; i<strlen(pr); i++){
  //chkbf();
  gpio_put_masked(RS|RW|data, RS|((pr[i]>>4)<<6));
  writelcd();
  gpio_put_masked(RS|RW|data, RS|((pr[i]&(0x0f))<<6));
  writelcd();
 }

}
void LCDinit(){
 /*LCD Function Set*/
  gpio_put_masked(RS|RW|data, ((0b001000)<<4));
  writelcd();
  gpio_put_masked(RS|RW|data, ((0b001000)<<4));
  writelcd();
  gpio_put_masked(RS|RW|data, ((0b100000)<<4));
  writelcd();

  /*Display On/Off Control*/
  gpio_put_masked(RS|RW|data, ((0b000000)<<4));
  writelcd();
  gpio_put_masked(RS|RW|data, ((0b110000)<<4));
  writelcd();
  
  /*Entry Mode Set*/
  gpio_put_masked(RS|RW|data, ((0b000000)<<4));
  writelcd();
  gpio_put_masked(RS|RW|data, ((0b011000)<<4));
  writelcd();
 
  /*CLear Display*/
  gpio_put_masked(RS|RW|data, ((0b000000)<<4));
  writelcd();
  gpio_put_masked(RS|RW|data, ((0b000100)<<4));
  writelcd();

  delay_ms(1000);
}

void chkbf(){
 gpio_put_masked(RS|RW,RW);
 writelcd();
 gpio_set_dir(9, false);
 bool bflag = gpio_get(9);
 int n = 0;
 while(bflag){
  writelcd();
  n = n + 1;
  if ((n%2)==1){
    bflag = gpio_get(9);
  }  
 }
  writelcd();
  gpio_set_dir(9,true);
  gpio_put_masked(RS|RW,0);
}

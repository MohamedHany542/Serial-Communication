#include <iostream>
#include <string.h>
#include "pico/stdlib.h"
#include "LCD.h"

using namespace std;

#define fset1     0b000010
#define fset2     0b001000
#define display1  0b000000
#define display2  0b001100
#define entry1    0b000000
#define entry2    0b000110
#define clr1      0b000000
#define clr2      0b000001

#define cursoron  0b001110
#define cursoroff 0b001100



LCD:: LCD(int RS_user, int RW_user, int E_user, int d7_user, int d6_user, int d5_user, int d4_user, bool mode_user) 
{
 RS = (0b1 << RS_user);  RS_u = RS_user;
 RW = (0b1 << RW_user);  RW_u = RW_user;
 E  = (0b1 << E_user);   E_u  = E_user;
 d7 = (0b1 << d7_user);  d7_u = d7_user;
 d6 = (0b1 << d6_user);  d6_u = d6_user;
 d5 = (0b1 << d5_user);  d5_u = d5_user;
 d4 = (0b1 << d4_user);  d4_u = d4_user; 
 mode = mode_user;
}

void LCD:: writelcd(){
  gpio_put(E_u, mode);
  busy_wait_ms(1);
  gpio_put(E_u, !mode);
  busy_wait_ms(1);
}

void LCD:: chkbf(){
 gpio_put_masked(RS|RW, mode? RW : RS);
 writelcd();
 gpio_set_dir(d7_u, false);
 bool bflag = mode? gpio_get(d7_u): !gpio_get(d7_u);
 int n = 0;
 while(bflag){
  writelcd();
  n = n + 1;
  if ((n%2)==1){
    bflag = mode? gpio_get(d7_u): !gpio_get(d7_u);
  }  
 }
  writelcd();
  gpio_set_dir(d7_u,true);
  gpio_put_masked(RS|RW, mode? 0 : RW|RS);
}

int  LCD:: bmap(int add_d){
  add_d = mode? add_d : ~add_d;
   return (add_d & 0x20) << (RS_u-5) | (add_d & 0x10) << (RW_u-4) | (add_d & 0x08) << (d7_u-3) | (add_d & 0x04) << (d6_u-2) | (add_d & 0x02) << (d5_u-1) | (add_d & 0x01) << (d4_u); 
}

void LCD:: init(){
  /*LCD pins set dir*/
  gpio_init_mask(RS|RW|E|d7|d6|d5|d4);
  gpio_set_dir_out_masked(RS|RW|E|d7|d6|d5|d4);

  /*LCD Function Set*/
  gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(fset1));    // 00 0010 .. data length is 4bit >>>(!(RS|RW) | (!d7 | !d6 | d5 | !d4))
  writelcd();
  gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(fset1));    // 1st nibble for 2nd time
  writelcd();
  gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(fset2));    // 00 1000 .. 2-line display, dot font
  writelcd();

  /*Display On/Off Control*/
  gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(display1)); // 00 0000
  writelcd();
  gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(display2)); // 00 1100 .. display on, cursor off, blinking off
  writelcd();
  
  /*Entry Mode Set*/
  gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(entry1));   // 00 0000
  writelcd();
  gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(entry2));   // 00 0110
  writelcd();
 
  /*CLear Display*/
  gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(clr1));    // 00 0000
  writelcd();
  gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(clr2));    // 00 0001
  writelcd();
}

void LCD:: setaddress(int row, int column){
 //chkbf();
 int address = 0x80 | (row<<6) | column;
 gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(0x00 | address >> 4));  
 writelcd();
 gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(0x00 | address));
 writelcd();
}

void LCD:: print(char message[]){
  for (int i=0; i<strlen(message); i++){
  //chkbf();
  gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(0x20 | message[i]>>4));
  writelcd();
  gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(0x20| (message[i] & 0x0F)));
  writelcd();
 }
}

void LCD:: cursor(){
 gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(display1));   // 00 0000
 writelcd();
 gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(cursoron));   // 00 1110 .. display on, cursor on, blinking off
 writelcd();
}

void LCD:: nocursor(){
 gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(display1));    // 00 0000
 writelcd();
 gpio_put_masked(RS|RW|d7|d6|d5|d4, bmap(cursoroff));   // 00 1100 .. display on, cursor off, blinking off
 writelcd();
}

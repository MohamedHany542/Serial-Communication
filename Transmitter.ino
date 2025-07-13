#include <LiquidCrystal.h>

#define RS 2
#define EN 3
#define DL4 4
#define DL5 5
#define DL6 6
#define DL7 7
#define TX_DATA 11
#define TX_CLK 12

const char *message = "hello, World!";
int checksum =0;
void setup() {
  pinMode(TX_DATA, OUTPUT);
  pinMode(TX_CLK, OUTPUT);

  LiquidCrystal lcd(RS, EN, DL4, DL5, DL6, DL7);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(message);
  for(int byte_idx = 0; byte_idx < strlen(message); ++byte_idx){
    char tx_byte = message[byte_idx];

    lcd.noCursor();
    lcd.setCursor(0, 1);
    lcd.print("        ");
    lcd.setCursor(byte_idx, 0);
    lcd.cursor();

   for(int bit_idx = 0; bit_idx < 8; ++bit_idx){
      bool tx_bit = tx_byte & (0x80 >> bit_idx);

      digitalWrite(TX_DATA, tx_bit);
      delay(100);

      lcd.noCursor();
      lcd.setCursor(bit_idx, 1);
      lcd.print(tx_bit ? "1" : "0");
      lcd.setCursor(byte_idx, 0);
      lcd.cursor();

      digitalWrite(TX_CLK, HIGH);
      delay(100);
      digitalWrite(TX_CLK, LOW);
    }
  }
  for(int i=0; i<strlen(message); i++){
    checksum += message[i];
  }
  checksum = ~(checksum%255) & 0xff;
  for (int i=0; i<8; i++){
    bool tx_bit = checksum & (0x80 >> i);
    digitalWrite(TX_DATA, tx_bit);
    delay(100);
    digitalWrite(TX_CLK, HIGH);
    delay(100);
    digitalWrite(TX_CLK, LOW);
  }
  digitalWrite(TX_DATA, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

}

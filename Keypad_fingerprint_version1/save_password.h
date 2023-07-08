#include <EEPROM.h>
#define EEPROM_SIZE 512
#define PASSWORD_ADDRESS 0
#define MAX_PASSWORD 99999999

int savePassword(int password){
  uint8_t saveDigit_12 = 0;
  uint8_t saveDigit_34 = 0;
  uint8_t saveDigit_56 = 0;
  uint8_t saveDigit_78 = 0;
  if(password > MAX_PASSWORD){
    return;
  }
  saveDigit_12 = (password / 1000000);
  saveDigit_34 = ((password - saveDigit_12 * 1000000) / 10000);
  saveDigit_56 = (password - saveDigit_12 * 1000000 - saveDigit_34 * 10000) / 100;
  saveDigit_78 = password - saveDigit_12 * 1000000 - saveDigit_34 * 10000 - saveDigit_56 * 100;

  EEPROM.put(PASSWORD_ADDRESS,     saveDigit_12);
  EEPROM.put(PASSWORD_ADDRESS + 1, saveDigit_34);
  EEPROM.put(PASSWORD_ADDRESS + 2, saveDigit_56);
  EEPROM.put(PASSWORD_ADDRESS + 3, saveDigit_78);
  return EEPROM.commit();
}

int getPassword(){
  uint8_t getDigit_12 = 0;
  uint8_t getDigit_34 = 0;
  uint8_t getDigit_56 = 0;
  uint8_t getDigit_78 = 0;
  int password;
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(PASSWORD_ADDRESS,     getDigit_12);
  EEPROM.get(PASSWORD_ADDRESS + 1, getDigit_34);
  EEPROM.get(PASSWORD_ADDRESS + 2, getDigit_56);
  EEPROM.get(PASSWORD_ADDRESS + 3, getDigit_78);
  password = getDigit_12 * 1000000 + getDigit_34 * 10000 + getDigit_56 * 100 + getDigit_78;
  return password;
}

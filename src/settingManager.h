

#ifndef SettingManager_h
#define SettingManager_h
#include "main.h"
#include <EEPROM.h>
#include <BaseManager.h>
#include <BaseSettingManager.h>

#define BUTTON_NOTFOUND 255
#define BUTTON_NBER 10


#define BUT_X 0
#define BUT_Y 1
#define BUT_A 2   
#define BUT_B 3
#define BUT_L1 4   
#define BUT_R1 5   
#define BUT_L2 6
#define BUT_R2 7
#define BUT_SELECT 8
#define BUT_START 9

#define BUT_HOTKEY BUT_R1



class btCfg {
  public:
  uint8_t gpio;
  uint8_t but;
  uint8_t action;
};

class SettingManager : public BaseSettingManager
{
public:

  SettingManager(unsigned char pinLed);


  virtual uint16_t readData();
  virtual uint16_t writeData();
  String getClassName() { return "SettingManager"; }
  String toString(boolean bJson);
  //uint16_t writeData();

  uint8_t getGPIO(uint8_t iButton) {
    return m_buttons[iButton].gpio;
  }


  uint8_t getActionFromButton(uint8_t iButton) {
    return m_buttons[iButton].action;
  }


  void setGPIO(uint8_t iButton,uint8_t iGPIO) {
    m_buttons[iButton].gpio = iGPIO;
  }
  
  void setAction(uint8_t iButton,uint8_t action) {
    m_buttons[iButton].action = action;
  }

  void set(uint8_t iButton,uint8_t iGPIO, uint8_t action) {
    m_buttons[iButton].action = action;
    m_buttons[iButton].gpio   = iGPIO;
    m_buttons[iButton].but    = iButton;
  }


  uint8_t getMaxButton() {
    return BUTTON_NBER;
  }
  
  btCfg *get(uint8_t iGPIO) {
    for (uint8_t i=0;i<BUTTON_NBER;i++){
      if (m_buttons[i].gpio == iGPIO ) return &m_buttons[i];
    }
    return NULL;
  }
  
  uint8_t getButton(uint8_t iGPIO) {
    btCfg *bt =  get( iGPIO);
    if (bt!=NULL) 
      return bt->but; 
    return BUTTON_NOTFOUND;
  }

  uint8_t getAction(uint8_t iGPIO) {
    btCfg *bt =  get( iGPIO);
    if (bt!=NULL) 
      return bt->action; 
    return BUTTON_NOTFOUND;
  }

private:
  btCfg m_buttons[BUTTON_NBER];//0..15
};

#endif

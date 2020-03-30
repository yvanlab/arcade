

#include "main.h"



SettingManager::SettingManager(unsigned char pinLed) : BaseSettingManager(pinLed)
{
   DEBUGLOG("Init  Data");
    for (uint8_t i=0;i<BUTTON_NBER;i++) {
      m_buttons[i].gpio=i;
      m_buttons[i].action=BUT_X;
    }
}

String SettingManager::toString(boolean bJson = false)
{
  String jSon;
  if (bJson == STD_TEXT)
    return BaseSettingManager::toString(bJson);
  else {
    
    for (uint8_t i=0;i<BUTTON_NBER;i++) {
      jSon += "\"" + String(i)+ "\":\"" + String(m_buttons[i].gpio) +"," + String(m_buttons[i].action) + "\""; 
      if (i<BUTTON_NBER-1) {
        jSon += ",";
      }
    }
  }
  return jSon;
 }


uint16_t SettingManager::readData()
{
  DEBUGLOGF("Read Data version [%d]\n",m_version );
  BaseSettingManager::readData();
  switchOn();
   
  if (m_version == EEPROM_VERSION)
  {  
    DEBUGLOG("Read Data");
    for (uint8_t i=0;i<BUTTON_NBER;i++) {
      m_buttons[i].but = i;
      m_buttons[i].gpio = (uint8_t)readEEPROM();
      m_buttons[i].action =(uint8_t)readEEPROM();
    } 
  } else {
    DEBUGLOGF("Read Data version [%d]\n",m_version );
    for (uint8_t i=0;i<BUTTON_NBER;i++) {
      m_buttons[i].but = i;
      m_buttons[i].gpio = (uint8_t)readEEPROM();
      m_buttons[i].action = (uint8_t)BUT_X;
    } 

  }
  switchOff();
  return m_iEEprom;
}
uint16_t SettingManager::writeData()
{
  DEBUGLOG("Write Data");
  BaseSettingManager::writeData();
  for (uint8_t i=0; i<BUTTON_NBER ; i++) {
      writeEEPROM(m_buttons[i].gpio);
      writeEEPROM(m_buttons[i].action);
    }
  EEPROM.commit();
  switchOff();
  return m_iEEprom;
}

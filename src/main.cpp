#include <Arduino.h>



#include "main.h"


Adafruit_MCP23017   *lightOut;
Adafruit_MCP23017   *buttonIn;

SettingManager      *configurationMgr;
WifiManager         *wfManager;
LightManager        *lightMgr;
ButtonManager       *buttonMgr;
SpiffFileManager    *spiffFileMgr;
CommandHelper       commandHpr;


#ifdef MCPOC_TELNET
RemoteDebug Debug;
#endif

#ifdef MCPOC_TELNET // Not in PRODUCTION
void processCmdRemoteDebug()
{
  String lastCmd = Debug.getLastCommand();
   if (lastCmd == "b")
  {
   buttonMgr->buttonEvent();
  }
  else
  if (lastCmd == "r")
  {
   for (uint8_t i = 0; i < BUTTON_MAX_ELT; i++)
        {
            uint8_t b = buttonIn->digitalRead(i);
            DEBUGLOGF("read bouton [%d][%d]\n",i,b);
        }
        DEBUGLOGF("read Interrupt[%d]\n",digitalRead(D6));
  }
  else if (lastCmd == "restart")
  {
    ESP.restart();
  }
  else if (lastCmd == "reset")
  {
    WiFi.disconnect();
  }
}
#endif

void startWiFiserver()
{
  if (wfManager->begin(IPAddress(MODULE_IP), MODULE_NAME, MODULE_MDNS, MODULE_MDNS_AP) == WL_CONNECTED)
  {
    wfManager->getServer()->on("/", dataPage);
    wfManager->getServer()->onNotFound(dataPage);
  }
  wfManager->getServer()->on("/status", dataJson);
  wfManager->getServer()->on("/setting", dataPage);
  wfManager->getServer()->on("/setData", setData);
  wfManager->getServer()->on("/setData", setData);
  wfManager->getServer()->on("/favicon.ico", HTTP_GET, [](){
    wfManager->getServer()->send(200, "text/plain", "Favicon");
  });
  Serial.println(wfManager->toString(STD_TEXT));
}

void setup(void)
{
  // ESP.wdtDisable() ;
  Serial.begin(115200);
  delay(500);
  Serial.println("start debuging");

  lightOut          = new Adafruit_MCP23017();
  buttonIn          = new Adafruit_MCP23017();
  
  lightOut->begin(0);
  buttonIn->begin(4);

  configurationMgr  = new SettingManager(m_pinLed);
  wfManager         = new WifiManager (m_pinLed, configurationMgr);
  lightMgr          = new LightManager(); 
  buttonMgr         = new ButtonManager();
  spiffFileMgr      = new SpiffFileManager();
  
  //delay(5000);
  configurationMgr->readData();
  DEBUGLOG("");
  DEBUGLOG(configurationMgr->toString(STD_TEXT));
  
  
  
  
  //SPIFFS.begin(true);

#ifdef MCPOC_TELNET
  Debug.begin(MODULE_NAME);
  String helpCmd = "l1\n\rl2\n\rl3\n\rrestart\n\rreset\n\r";
  Debug.setHelpProjectsCmds(helpCmd);
  Debug.setCallBackProjectCmds(&processCmdRemoteDebug);
#endif

#ifdef MCPOC_MOCK
  randomSeed(analogRead(0));
#endif

  lightMgr->loadSequences("/test.txt");
  while(!lightMgr->handle()) delay(1);
  //lightMgr->resetSequence();
  startWiFiserver();
   for (uint8_t i = 0; i < BUTTON_MAX_ELT; i++)
        {
            uint8_t b = buttonIn->digitalRead(i);
            DEBUGLOGF("read bouton [%d][%d]\n",i,b);
        }
        DEBUGLOGF("read Interrupt[%d]\n",digitalRead(D6));
  
}


void loop(void)
{
  wfManager->handleClient();
  buttonMgr->handle();
  bool isSequenceFinished = lightMgr->handle();
  

  /*if (commandHpr.isRecaleBoxStarting()){
    DEBUGLOG("Recal is starting");
    lightMgr->loadSequences("/start.txt");
  } 

  if (commandHpr.isRecaleBoxStoping()){
    DEBUGLOG("Recal is stoping");  
    lightMgr->loadSequences("/stop.txt");
  } 
*/
 /* if (!buttonMgr->isDemoMode() & !isSequenceFinished) {
    lightMgr->stopAllLights();
  }*/
  //Start  + A → Reset Game
  if (buttonMgr->isActionPressed(BUT_A) & buttonMgr->isActionPressed(BUT_HOTKEY)) {
    DEBUGLOG("Start  + A → Reset Game");
    lightMgr->loadSequences("/stopjeu.txt"); 
  }
  
  //Start  + Y → Save State
  if (buttonMgr->isActionPressed(BUT_Y) & buttonMgr->isActionPressed(BUT_HOTKEY)) {
    DEBUGLOG("Start  + Y → Save State");
    lightMgr->loadSequences("/flash.txt"); 
  }
  
  //Start  + X → load State
  if (buttonMgr->isActionPressed(BUT_X) & buttonMgr->isActionPressed(BUT_HOTKEY)) {
    DEBUGLOG("Start  + X → load State");
    lightMgr->loadSequences("/2flash.txt"); 
  }


if (buttonMgr->isActionPressed(BUT_R1) & buttonMgr->isActionPressed(BUT_R2)) {
    DEBUGLOG("R1+R2");
    lightMgr->loadSequences("/"+spiffFileMgr->getNextFile()); 
  }



  if (buttonMgr->isDemoMode() & isSequenceFinished) {
    DEBUGLOG("DemoMode");
    lightMgr->loadSequences("/"+spiffFileMgr->getNextFile()); 
  }
 
  if (Serial.available())
  {
    char c = Serial.read();
    switch (c)
    {
    case 'h':
    {
      lightMgr->playOneShote("L0:R1,H|1000,L|1000");
    /*  for (uint8_t i=0;i<configurationMgr->getMaxButton();i++) {
        delay(250);
        buttonMgr->playOneShote("L0:R1,H|1000,L|1000"));
        buttonOut.getPin(configurationMgr->getGPIO(i)).setValue(HIGH);
      }*/
      break;
    }
    case 'b':
    {
      buttonMgr->buttonEvent();
      break;
    }
    case 'r':
    {
       for (uint8_t i = 0; i < BUTTON_MAX_ELT; i++)
        {
            uint8_t b = buttonIn->digitalRead(i);
            DEBUGLOGF("read bouton [%d][%d]\n",i,b);
        }
        DEBUGLOGF("read Interrupt[%d]\n",digitalRead(D6));
      break;
    }
    case 's':
    {
      
      break;
    }
    }
  }
}


#ifndef main_h
#define main_h

#include <Arduino.h>




//#include <Wire.h>
#include "Adafruit_MCP23017.h"


#include <WifiManagerV2.h>
#include <myTimer.h>
#include <DelayHelper.h>
#include "SettingManager.h"
#include "networkUI.h"
#include "LightManager.h"
#include "ButtonManager.h"
#include "SpiffFileManager.h"
#include "commandHelper.h"

extern Adafruit_MCP23017    *lightOut;
extern Adafruit_MCP23017    *buttonIn;

extern SettingManager       *configurationMgr;
extern WifiManager          *wfManager;
extern LightManager         *lightMgr;
extern ButtonManager        *buttonMgr;
extern SpiffFileManager     *spiffFileMgr;


#ifdef ESP32
//#include <SPIFFS.h>
#include <analogWrite.h>
#else
//#include <FS.h>
#define INPUT_PULLDOWN INPUT_PULLDOWN_16 
#endif
#define MCP_BUTTON_INPUT_ADR 1

#ifdef ESP32
static const uint8_t m_pinLed = LED_BUILTIN;
#else
static const uint8_t m_pinLed = LED_BUILTIN;
#endif


#define MODULE_NAME ARCADE_NAME
#define MODULE_MDNS ARCADE_MDNS
#define MODULE_MDNS_AP ARCADE_MDNS_AP
#define MODULE_IP  ARCADE_IP


#ifdef MCPOC_TELNET
//extern RemoteDebug          Debug;
#endif

#endif

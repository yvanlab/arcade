#ifndef ButtonManager_h
#define ButtonManager_h

//https://www.youtube.com/watch?v=6eh1v0FtlVQ

//#include "main.h"
//#include "Button.h"
#include <Arduino.h>
#include "FunctionalInterrupt.h"

#define BUTTON_MAX_ELT 10

#define ESP_INTERRUPT D6




#define MAX_DELAY_BEFORE_DEMO 1*60*1000


extern Adafruit_MCP23017  *buttonIn;
/*extern Adafruit_MCP23017  *lightOut;*/
extern LightManager         *lightMgr;

class ButtonManager
{
public:

    ButtonManager()
    {
        for (uint8_t i = 0; i < BUTTON_MAX_ELT; i++)
        {
            buttonIn->pinMode(i, INPUT);
            buttonIn->pullUp(i, LOW);
            buttonIn->setupInterruptPin(i, CHANGE);
            actionPressed[i]  = false;
        }
        buttonIn->setupInterrupts(true, false, LOW);

        pinMode(ESP_INTERRUPT, INPUT);
        attachInterrupt(ESP_INTERRUPT, std::bind(&ButtonManager::buttonEvent, this), CHANGE);
        //attachInterrupt(ESP_INTERRUPT_B,std::bind(&ButtonManager::buttonEventB, this),HIGH);
        // configuration for a button on port A
        // interrupt will triger when the pin is taken to ground by a pushbutton
    };

    void buttonEvent()
    {
        interrupt = true;
    }

    void handle()
    {
       // if (interrupt)
        //{
            uint16_t read = buttonIn->readGPIOAB(); 
            //Serial.println(read, BIN);
            //DEBUGLOGF("read Interrupt[%d]\n",digitalRead(D6));
            interrupt = false;
            uint16_t buttonToChange = (read | previousRed) & 0b0000001111111111;
            uint16_t buttonToChangeMask = 1;
            for (uint8_t pin = 0 ;pin<BUTTON_MAX_ELT; pin++ ) {
                if (buttonToChangeMask & buttonToChange) {
                    btCfg *bt = configurationMgr->get(pin);
                    DEBUGLOGF("ButtonManager::handle Interrupt gpio[%d] bt[%d] action[%d] val[%d]\n", pin, bt->but, bt->action, buttonToChangeMask & read);
                    
                    if (isDemoMode()) {
                        lightMgr->stopAllLights();
                    }

                    if (buttonToChangeMask & read) {
                        lightMgr->forceLight(bt->gpio, bt->but, LIGHT_ON);
                        actionPressed[bt->action]  = true;
                    }else{
                        lightMgr->forceLight(bt->gpio, bt->but, LIGHT_OFF);
                        actionPressed[bt->action]  = false;
                    }
                    m_delayForDemo.startDelay(MAX_DELAY_BEFORE_DEMO);
                }
                buttonToChangeMask = buttonToChangeMask<<1;

//            }       
          
            
            previousRed = read;
 /*           
            uint8_t pin = buttonIn->getLastInterruptPin();
            uint8_t val = buttonIn->getLastInterruptPinValue();
            if (pin == MCP23017_INT_ERR || val == MCP23017_INT_ERR ) {
                DEBUGLOGF("ButtonManager::handle NOT FOUND Interrupt gpio[%d] val[%d]\n", pin,  val);
                return;
            }
            uint8_t nBut = configurationMgr->getButton(pin);
            DEBUGLOGF("ButtonManager::handle Interrupt gpio[%d] bt[%d] val[%d]\n", pin, nBut, val);
            if (val) {
                lightMgr->forceLight(nBut, LIGHT_OFF);actionPressed[nBut]  = false;
            }else{
                lightMgr->forceLight(nBut, LIGHT_ON);actionPressed[nBut]  = true;
            }
            */
           
        }
    };


    bool isActionPressed(uint8_t nAction)
    {
        return actionPressed[nAction];
    };


    bool isDemoMode() {
        return m_delayForDemo.isDone();
    }

    String toString(boolean bJson = false)
    {
        String jSon;
        if (bJson == STD_TEXT)
            return "";
        else
        {
            ;
        }
        return jSon;

    }

    boolean interrupt = false;
    bool actionPressed[BUTTON_MAX_ELT];
    DelayHelper m_delayForDemo;
    uint16_t previousRed = 0;

}
;

#endif
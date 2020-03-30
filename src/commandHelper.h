#ifndef CommandHelper_h
#define CommandHelper_h

//https://www.youtube.com/watch?v=6eh1v0FtlVQ

//#include "main.h"
//#include "Button.h"
#include <Arduino.h>
#include "FunctionalInterrupt.h"

#define ESP_COMMAND D7

#define SWITCH_ONOFF_DELAY 5*1000

class CommandHelper
{
public:

    CommandHelper()
    {
        pinMode(ESP_COMMAND, INPUT);
        attachInterrupt(ESP_COMMAND, std::bind(&CommandHelper::buttonEvent, this), CHANGE);
    };

   void buttonEvent()
    {
        isJustChanged = true;
    }


    void handle()
    {
        if (isJustChanged) {
            isJustChanged=false;
            if (m_delay.isDone()) {
                m_delay.startDelay(SWITCH_ONOFF_DELAY);
                isRecalBoxActive = digitalRead(ESP_COMMAND);
            }
        }
    };


    bool isRecalBoxSwitchedOn()
    {
        return isRecalBoxActive;
    };
    
    bool  isRecaleBoxStarting() {
        if (isRecalBoxActive & !m_delay.isDone()) {
            return true;
        }
        return false;
    }

    bool  isRecaleBoxStoping() {
        if (!isRecalBoxActive & !m_delay.isDone()) {
            return true;
        }
        return false;
    }

    
    boolean isRecalBoxActive = true;
    boolean isJustChanged = false;
    DelayHelper m_delay;

}
;

#endif
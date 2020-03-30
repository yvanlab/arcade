#ifndef LightManager_h
#define LightManager_h

//#include "main.h"
#include "Light.h"

#define LIGHT_MAX_ELT 10
#define LIGHT_MAX 10
#define LIGHT_ONE_SHOT_ELT LIGHT_MAX_ELT-1


class LightManager
{
public:
    LightManager()
    {
        
        maxNberLights=LIGHT_MAX_ELT;
    
    };

    void loadSequences(String fileName)
    {
        if (SPIFFS.exists(fileName.c_str()))
        {
            File dataFile = SPIFFS.open(fileName.c_str(), "r");
            //uint8_t id = 0;
            String line;
            line = dataFile.readStringUntil('\n');
            m_title = line.substring(0,line.length()-1);
            while (dataFile.available()) {
                line = dataFile.readStringUntil('\n');
                int16_t indexLast = line.indexOf(':');
                
                uint8_t butID = line.substring(1, indexLast).toInt();
                //DEBUGLOGF("LightMger.m_nButton %d\n",butID);
                m_Light[butID].setSequence(line);
            }
            //maxNberLights = id;
            dataFile.close();
           
        }
        else
        {
            DEBUGLOGF("loadFromSpiffs[%s] does not exist\n", fileName.c_str());
        }
    };

    bool handle()
    {
        bool isSequencesFinished = true;
        for (uint8_t i=0;i<maxNberLights;i++) {
            isSequencesFinished = isSequencesFinished & m_Light[i].handle();
        }
        /*bActif = bActif &*/ m_OneShotSequence.handle();
        return isSequencesFinished;
    };

    void updateGPIO() {
        for (uint8_t i=0;i<maxNberLights;i++) {
            m_Light[i].updateGPIO();
        }

    }

    //
    void resetSequence() {
        for (uint8_t i=0;i<maxNberLights;i++) {
            m_Light[i].setButton(i);
            m_Light[i].forceLight(LIGHT_OFF);
        } 
    }

    void stopAllLights() {
        for (uint8_t i=0;i<maxNberLights;i++) {
            forceLight(i,LIGHT_OFF);
        }
    }

    void forceLight(uint8_t nLight, uint8_t action) {
        //DEBUGLOGF("Force lihjt bouton [%d] \n", nLight);
        m_Light[nLight].forceLight(action);
       
    }


    void forceLight(uint8_t nGpio, uint8_t nLight, uint8_t action) {
        //DEBUGLOGF("Force lihjt bouton [%d] \n", nLight);
        lightOut->digitalWrite(nGpio,action == LIGHT_ON);
    }

    String toString(boolean bJson = false)
    {
        String jSon;
        if (bJson == STD_TEXT)
            return "";
        else {
            jSon += "\"title\":\"" + m_title + "\","; 
            for (uint8_t i=0;i<maxNberLights;i++) {
                jSon += m_Light[i].toString(bJson);
                if (i<maxNberLights-1) jSon += ",";
            }
        }
         return jSon;
    }

    void playOneShote(String sequence){
        m_OneShotSequence.setSequence(sequence);
    }
   
    String m_title;
    Light m_Light[LIGHT_MAX_ELT];
    Light m_OneShotSequence;

    uint8_t maxNberLights = 0;
};

#endif
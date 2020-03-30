#ifndef Light_h
#define Light_h

//#define MCP_LIGHT_OUTPUT_ADR 0x0
//#include "main.h"




extern SettingManager   *configurationMgr;
extern Adafruit_MCP23017  *lightOut;



#define LIGHT_MAX_SEQ 10
#define LIGHT_ON 'H'
#define LIGHT_OFF 'L'

class LightElt
{
 public:
    uint16_t duree;
    char action;
    void set(String sElt)
    {
        //DEBUGLOGF("LightElt:%s\n",sElt.c_str());
        //250|L
        uint8_t index = sElt.indexOf('|');
        action = sElt.substring(0, index).charAt(0);
        //DEBUGLOGF("LightElt.action%s\n",action.c_str());
        //DEBUGLOGF("LightElt.duree%s\n",sElt.substring(index + 1).c_str());
        duree = sElt.substring(index + 1).toInt();
    }
    String toString(boolean bJson = false)
    {
        String jSon;
        if (bJson == STD_TEXT)
            return "";
        else {
            jSon = "\"duree\":\"" + String(duree) +"\",";
            jSon += "\"action\":\"" + String(action) +"\""; 
            
        }
        return jSon;
    }
    

};

class Light
{
public:
    //MCP23017  lightOut((uint8_t)0);
    

    /*void Light()
    { 
        ;  
    }*/

    //L0:Rn,H|250,L|500
    void setSequence(String sSquence)
    {
        DEBUGLOGF("Light.sSquence%s\n",sSquence.c_str());
        //Extract Light num
        int16_t indexFirst = 1;
        int16_t indexLast = sSquence.indexOf(':');
        //DEBUGLOGF("Light.m_nButton%s\n",sSquence.substring(indexFirst, indexLast).c_str());
        m_nButton = sSquence.substring(indexFirst, indexLast).toInt();
        indexFirst = indexLast + 1;
        m_nGpio = configurationMgr->getGPIO(m_nButton);

        //Extract Repetion
        indexLast = sSquence.indexOf(',', indexFirst );
        
        //DEBUGLOGF("Light.m_MaxRepetition%s\n",sSquence.substring(indexFirst+1, indexLast).c_str());
        m_MaxRepetition = sSquence.substring(indexFirst+1, indexLast).toInt();
        //indexFirst = indexLast + 1;

        //Extract Sequence
        //L0:R10,H|1000,L|1000
        uint8_t i = 0;
        indexFirst = indexLast + 1;
        indexLast = sSquence.indexOf(',',indexFirst );
        while (indexLast > 0)
        {
            //DEBUGLOGF("[%d..%d]",indexFirst,indexLast );
            m_Sequence[i].set(sSquence.substring(indexFirst, indexLast));
            i++;
            indexFirst = indexLast + 1;
            indexLast = sSquence.indexOf(',',indexFirst);
        }
        m_Sequence[i].set(sSquence.substring(indexFirst));
        m_nbElts = i + 1;
        m_ID = -1;
 
        lightOut->pinMode(m_nGpio,OUTPUT);

    }

    void setAction(LightElt elt) {
        if (m_nButton==0)
            DEBUGLOGF("setAction ID[%d] B[%d], G[%d], D[%d] A[%c]\n",m_ID,m_nButton, m_nGpio, m_Sequence[m_ID].duree , m_Sequence[m_ID].action );
        m_delay.startDelay(m_Sequence[m_ID].duree);
        //switch light*
        lightOut->digitalWrite(m_nGpio,m_Sequence[m_ID].action == LIGHT_ON);
        
    }

    
    void setButton(uint8_t nButton) {
        m_nButton = nButton;
        updateGPIO();
    }

    void forceLight(uint8_t action) {
        m_ID = m_nbElts;
        m_MaxRepetition =0;
        m_delay.startDelay(0);
        lightOut->digitalWrite(m_nGpio,action == LIGHT_ON);
        // DEBUGLOGF("Force lihjt m_nGpio [%d] \n", m_nGpio);
    }

    void updateGPIO() {
        m_nGpio = configurationMgr->getGPIO(m_nButton);
        lightOut->pinMode(m_nGpio,OUTPUT);
    }


    bool handle()
    {
        bool isSequenceFinished = m_delay.isDone();
        if (isSequenceFinished)
        {
            if (m_ID < m_nbElts-1)
            {
                m_ID++;
                setAction(m_Sequence[m_ID]);
                isSequenceFinished = false;
            }
            else
            {
                if (m_MaxRepetition != 0)
                {
                    m_MaxRepetition--;
                    m_ID = -1;
                    isSequenceFinished = false;
                }
            }
        }
        return isSequenceFinished;
    }
    
    
    String toString(boolean bJson = false)
    {
        String jSon;
        if (bJson == STD_TEXT)
            return "";
        else {
            jSon  = "\"L"+String(m_nButton) + "\":{";
            jSon += "\"GPIO\":\""+String(m_nGpio) + "\",";
            jSon += "\"repeat\":\""+String(m_MaxRepetition) + "\",";
            jSon += "\"seq\":[";
            for (uint8_t i=0;i<m_nbElts;i++) {
                jSon += "{";
                jSon += m_Sequence[i].toString(bJson);
                jSon += "}";
                if (i<m_nbElts-1) jSon += ",";
            }
            jSon += "]}";
        }
        return jSon;
    }


    LightElt m_Sequence[LIGHT_MAX_SEQ];
    uint8_t m_nbElts;
    int8_t m_ID;
    uint8_t m_MaxRepetition;
    uint8_t m_nButton;
    uint8_t m_nGpio;
    DelayHelper m_delay;
};

#endif
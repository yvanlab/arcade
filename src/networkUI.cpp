
#include "main.h"

#ifdef ESP32
extern "C"
{
  uint8_t temprature_sens_read();
}
#endif
#ifdef ESP8266
uint8_t temprature_sens_read()
{
  return 0;
}
#endif

/*String getSPIFFFilesJson()
{
  String res;
  boolean bFirst = true;

  Dir dir = SPIFFS.openDir("/");
  String filename;
  while (dir.next())
  {
    //Serial.println(dir.fileName());
    filename = dir.fileName();
    if (filename.indexOf(".txt") > 0)
    {
      if (bFirst)
      {
        res += "\"" + dir.fileName().substring(1) + "\"";
        bFirst = false;
      }
      else
        res += ",\"" + dir.fileName().substring(1) + "\"";
    }
  }
  return res;
}*/

String getJson()
{
  String tt("{\"module\":{");
  tt += "\"nom\":\"" + String(MODULE_NAME) + "\",";
  tt += "\"version\":\"" + String(CURRENT_VERSION) + "\",";
  tt += "\"status\":\"" + String(STATUS_PERIPHERIC_WORKING) + "\",";
  tt += "\"uptime\":\"" + NTP.getUptimeString() + "\",";
  tt += "\"build_date\":\"" + String(__DATE__ " " __TIME__) + "\",";
  tt += "\"temp\":\"" + String((temprature_sens_read() - 32) / 1.8) + "\"},";

  tt += "\"setting\":{" + configurationMgr->toString(JSON_TEXT) + "},";
  tt += "\"sequence\":{" + lightMgr->toString(JSON_TEXT) + "},";
  //tt += "\"files\":[" + getSPIFFFilesJson() + "],";
  tt += "\"files\":[" + spiffFileMgr->toString() + "],";

  tt += "\"LOG\":[" + wfManager->log(JSON_TEXT) + "," +
        configurationMgr->log(JSON_TEXT) + "],";
  tt += "\"datetime\":{" + wfManager->getHourManager()->toDTString(JSON_TEXT) + "}}";
  return tt;
}

void dataJson()
{
  digitalWrite(m_pinLed, HIGH);
  DEBUGLOG("dataJson");
  wfManager->getServer()->send(200, "text/json", getJson());
  digitalWrite(m_pinLed, LOW);
}

void setData()
{
  digitalWrite(m_pinLed, HIGH);
  DEBUGLOG("SetData");
  String param;
  String value;
  for (uint8_t i = 0; i < wfManager->getServer()->args(); i++)
  {
    param = wfManager->getServer()->argName(i);
    value = wfManager->getServer()->arg(i);
    DEBUGLOGF("[%s,%s]", param.c_str(), value.c_str());
  }

  if (wfManager->getServer()->hasArg("load"))
  {
    lightMgr->loadSequences("/" + wfManager->getServer()->arg("load"));
  }
  else
  {
    boolean bSave = false;
    for (uint8_t i = 0; i < wfManager->getServer()->args(); i++)
    {
      
      param = wfManager->getServer()->argName(i);
      value = wfManager->getServer()->arg(i);
      DEBUGLOGF("[%s,%s]", param.c_str(), value.c_str());  
      if (param == "save")
      {
        bSave = true;
      }
      else
      {
        char Category = param[0];
        String Bid = param.substring(1);
        if (Category == 'L') {
          configurationMgr->setGPIO(Bid.toInt(), value.toInt());
          lightMgr->updateGPIO();
        } else if (Category == 'A')
          configurationMgr->setAction(Bid.toInt(), value.toInt());
      }
    }

    if (bSave)
    {
      configurationMgr->writeData();
    }
    else
    {
      String sequence = "L%s:R1,L|25,H|25";
      sequence.replace("%s", param.substring(1));
      lightMgr->playOneShote(sequence);
    }
  }
  digitalWrite(m_pinLed, LOW);
}

void dataPage()
{
  digitalWrite(m_pinLed, HIGH);

  wfManager->loadFromSpiffs("/index.html");
  //wfManager->getServer()->sendHeader("Location", "/robot.html",true);   //Redirige vers page index.html sur SPIFFS
  wfManager->getServer()->send(200, "text/plane", "");
  digitalWrite(m_pinLed, LOW);
}

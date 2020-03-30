#ifndef SpiffFileManager_h
#define SpiffFileManager_h

#include <Arduino.h>
#include <fs.h>

class SpiffFileManager
{
public:
    String sequenceFile[20];
    uint8_t maxNbFile=0;
    uint8_t idFile=0;
    SpiffFileManager()
    {
        if(!SPIFFS.begin()){
            DEBUGLOG("SPIFFS Mount Failed");
           
        } else {
            DEBUGLOG("SPIFFS Mount OK");
            loadFiles();
        }
    };

    void loadFiles()
    {
        String res;
        idFile  = 0;

        Dir dir = SPIFFS.openDir("/");
        String filename;
        while (dir.next())  {
            filename = dir.fileName();
            if (filename.indexOf(".txt") > 0)  {
                sequenceFile[idFile]=dir.fileName().substring(1);
                idFile++;
            }
        }
        maxNbFile = idFile;
    }
    
    String getFile(uint8_t id)
    {
        return sequenceFile[id];
    };

    String getNextFile() {
        idFile++;
        if (idFile >= maxNbFile) 
            idFile = 0;
        return getFile(idFile);
        
    }

    //
    String toString() {
        String res;
        uint8_t id=0;
        String filename;
        while (id<maxNbFile)   {
            if (id==0)   {
                res += "\"" + sequenceFile[id] + "\"";
            } else 
                res += ",\"" + sequenceFile[id] + "\"";

            id++;
        }
        return res;
    }
};

#endif
#define DEBUG_WRITE_SERIAL
#define DEBUG_WRITE_LOG

#include "httpServer.h"

void setup()
{
  serverBegin();
  /*
  blnprint("---------------");
  blnprint((String)sizeof(Task));
  blnprint((String)sizeof(http));
  blnprint((String)sizeof(wifiAuto));
  blnprint((String)sizeof(Blynk));
  blnprint("-----------------");
  blnprint(String(sizeof(getStringESP)));
  blnprint(String(sizeof(getStringFile)));
  blnprint(String(sizeof(getStringAP)));
  blnprint(String(sizeof(getStringSTA)));
  */
}

void loop()
{
  serverRun();
}

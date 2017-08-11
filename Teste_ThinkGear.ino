#include "thinkgear.h"

char *comPortName = NULL;
    int   dllVersion = 0;
    int   connectionId = 0;
    int   packetsRead = 0;
    int   errCode = 0;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  dllVersion = TG_GetDriverVersion();
  connectionId = TG_GetNewConnectionId();
}

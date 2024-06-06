
#include"NIDAQmx.h"
#include<stdbool.h>

bool createDigitalInput(uInt32 port);
bool createDigitalOutput(uInt32 port);
uInt8 readDigitalU8(uInt32 port);
void writeDigitalU8(uInt32, uInt8 data);
bool close_channels();



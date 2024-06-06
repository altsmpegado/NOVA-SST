#pragma once
#include <interface.h>

void initializeHardwarePorts();
void setBitValue(uInt8* variable, int n_bit, int new_value_bit);
int getBitValue(uInt8 value, uInt8 n_bit);
int readPort(int port);
void writePort(int port, int value);
int getCylinder1Position();
int getCylinder2Position();
int getCylinderStartPosition();
void cylinder1MoveForward();
void cylinder1MoveBackward();
void cylinder1Stop();
void cylinder2MoveForward();
void cylinder2MoveBackward();
void cylinder2Stop();
void cylinderStartMoveForward();
void cylinderStartMoveBackward();
void cylinderStartStop();
void conveyorMove();
void conveyorStop();
int getIdentificationSensors();
bool isCylinder1MovingForward();
bool isCylinder1MovingBackward();
bool isCylinder2MovingForward();
bool isCylinder2MovingBackward();
bool isCylinderStartMovingForward();
bool isCylinderStartMovingBackward();
bool isPartAtDock1();
bool isPartAtDock2();
bool isDock1Active();
bool isDock2Active();
bool isDockEndActive();
bool isLEDActive();
void flashLED(int value);


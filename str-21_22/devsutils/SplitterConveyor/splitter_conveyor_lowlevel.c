#include <stdio.h>
#include <conio.h>
#include <stdio.h>
#include <interface.h>

void initializeHardwarePorts() {
	createDigitalInput(0);
	createDigitalInput(1);
	createDigitalOutput(2);
}

void setBitValue(uInt8* variable, int n_bit, int new_value_bit)
// given a byte value, set the n bit to value
{
	uInt8 mask_on = (uInt8)(1 << n_bit);
	uInt8 mask_off = ~mask_on;
	if (new_value_bit) *variable |= mask_on;
	else *variable &= mask_off;
}

int getBitValue(uInt8 value, uInt8 n_bit)
// given a byte value, returns the value of bit n
{
	return(value & (1 << n_bit));
}

int readPort(int port) {
	return readDigitalU8(port);
}

void writePort(int port, int value) {
	writeDigitalU8(value, port);
}

int getCylinder1Position()
{
	int v = readDigitalU8(0);
	if (!getBitValue(v, 4))
		return 0;
	else if (!getBitValue(v, 3))
		return 1;
	return(-1);
}
int getCylinder2Position()
{
	int v = readDigitalU8(0);
	if (!getBitValue(v, 2))
		return 0;
	else if (!getBitValue(v, 1))
		return 1;
	return(-1);
}

int getCylinderStartPosition()
{
	int v = readDigitalU8(0);
	if (getBitValue(v, 6))
		return 0;
	else if (getBitValue(v, 5))
		return 1;
	return(-1);
}

void cylinder1MoveForward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 4, 1);
	setBitValue(&p, 3, 0);
	writeDigitalU8(2, p);
}

void cylinder1MoveBackward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 4, 0);
	setBitValue(&p, 3, 1);
	writeDigitalU8(2, p);
}

void cylinder1Stop()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 4, 0);
	setBitValue(&p, 3, 0);
	writeDigitalU8(2, p);
}

void cylinder2MoveForward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 5, 0);
	setBitValue(&p, 6, 1);
	writeDigitalU8(2, p);
}

void cylinder2MoveBackward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 5, 1);
	setBitValue(&p, 6, 0);
	writeDigitalU8(2, p);
}

void cylinder2Stop()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 5, 0);
	setBitValue(&p, 6, 0);
	writeDigitalU8(2, p);
}

void cylinderStartMoveForward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 1, 1);
	setBitValue(&p, 0, 0);
	writeDigitalU8(2, p);
}
void cylinderStartMoveBackward()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 1, 0);
	setBitValue(&p, 0, 1);
	writeDigitalU8(2, p);
}
void cylinderStartStop()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 1, 0);
	setBitValue(&p, 0, 0);
	writeDigitalU8(2, p);
}

void conveyorMove()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 2, 1);
	writeDigitalU8(2, p);
}
void conveyorStop()
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 2, 0);
	writeDigitalU8(2, p);
}

//Identifies the type of package entering to be processed
int getIdentificationSensors() {
	uInt8 p = readDigitalU8(1);
	if (getBitValue(p, 5))
		return 1;
	if (getBitValue(p, 6))
		return 2;
	return 0;
}

bool isCylinder1MovingForward() {
	uInt8 p = readDigitalU8(2);
	if (getBitValue(p, 4))
		return true;
	else return false;
}

bool isCylinder1MovingBackward() {
	uInt8 p = readDigitalU8(2);
	if (getBitValue(p, 3))
		return true;
	else return false;
}

bool isCylinder2MovingForward() {
	uInt8 p = readDigitalU8(2);
	if (getBitValue(p, 6))
		return true;
	else return false;
}

bool isCylinder2MovingBackward() {
	uInt8 p = readDigitalU8(2);
	if (getBitValue(p, 5))
		return true;
	else return false;
}

bool isCylinderStartMovingForward() {
	uInt8 p = readDigitalU8(2);
	if (getBitValue(p, 1))
		return true;
	else return false;
}

bool isCylinderStartMovingBackward() {
	uInt8 p = readDigitalU8(2);
	if (getBitValue(p, 0))
		return true;
	else return false;
}

bool isPartAtDock1()
{
	uInt8 p = readDigitalU8(0);
	if (getBitValue(p, 0))
		return true;
	else return false;
}

bool isPartAtDock2()
{
	uInt8 p = readDigitalU8(1);
	if (getBitValue(p, 7))
		return true;
	else return false;
}

bool isDock1Active()
{
	uInt8 p = readDigitalU8(1);
	if (getBitValue(p, 4))
		return true;
	else return false;
}

bool isDock2Active()
{
	uInt8 p = readDigitalU8(1);
	if (getBitValue(p, 3))
		return true;
	else return false;
}

bool isDockEndActive()
{
	uInt8 p = readDigitalU8(1);
	if (getBitValue(p, 2))
		return true;
	else return false;
}

bool isLEDActive()
{
	uInt8 p = readDigitalU8(2);
	if (getBitValue(p, 7))
		return true;
	else return false;
}

void flashLED(int value) 
{
	uInt8 p = readDigitalU8(2);
	setBitValue(&p, 7, value); //turn ON or OFF the LED
	writeDigitalU8(2, p);
}
#include <conio.h>
#include <cstdio>
#include <stdlib.h>
#include <windows.h> //for Sleep function
#include <time.h>
#include <list>
#include <stdbool.h>
#include <my_interaction_functions.h>

extern "C" {
#include <interface.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
}

int getBitValue(uInt8 value, uInt8 bit_n)
// given a byte value, returns the value of its bit n
{
    return(value & (1 << bit_n));
}

void setBitValue(uInt8* variable, int n_bit, int new_value_bit)
// given a byte value, set the n bit to value
{
    uInt8 mask_on = (uInt8)(1 << n_bit);
    uInt8 mask_off = ~mask_on;
    if (new_value_bit) *variable |= mask_on;
    else *variable &= mask_off;
}

// put here all function's implementations
void moveXLeft() 
{
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 6, 1); // set bit 6 to high level
    setBitValue(&p, 7, 0); // set bit 7 to low level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();

}

void moveZUp() 
{
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 3, 1); // set bit 6 to high level
    setBitValue(&p, 2, 0); // set bit 7 to low level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}

void moveZDown() 
{
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 3, 0); // set bit 6 to high level
    setBitValue(&p, 2, 1); // set bit 7 to low level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}

void moveXRight() 
{
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 6, 0); // set bit 6 to low level
    setBitValue(&p, 7, 1); // set bit 7 to high level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
   
}

void moveYInside() 
{
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 5, 1); // set bit 6 to high level
    setBitValue(&p, 4, 0); // set bit 7 to low level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}

void moveYOutside() 
{
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 5, 0); // set bit 6 to high level
    setBitValue(&p, 4, 1); // set bit 7 to low level
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}

void stopX()
{
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 6, 0); // set bit 6 to low level
    setBitValue(&p, 7, 0); // set bit 7 to low level
    writeDigitalU8(2, p); // update port 2
}

void stopY() 
{
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 5, 0); // set bit 5 to low level
    setBitValue(&p, 4, 0); // set bit 4 to low level
    writeDigitalU8(2, p); // update port 2
}

void stopZ() 
{
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 3, 0); // set bit 3 to low level
    setBitValue(&p, 2, 0); // set bit 2 to low level
    writeDigitalU8(2, p); // update port 2
}

int getXPos() 
{
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(0);
    taskEXIT_CRITICAL();
    if (!getBitValue(p, 2))
        return 1;
    if (!getBitValue(p, 1))
        return 2;
    if (!getBitValue(p, 0))
        return 3;
    return(-1);
}

int getYPos()
{
    taskENTER_CRITICAL();
    uInt8 p0 = readDigitalU8(0);
    taskEXIT_CRITICAL();
    if (!getBitValue(p0, 5))
        return 1;
    if (!getBitValue(p0, 4))
        return 2;
    if (!getBitValue(p0, 3))
        return 3;
    return(-1);
}

int getZPos()
{
    taskENTER_CRITICAL();
    uInt8 p0 = readDigitalU8(0);
    uInt8 p1 = readDigitalU8(1);
    taskEXIT_CRITICAL();
    if (!getBitValue(p1, 3))
        return 1;
    if (!getBitValue(p1, 1))
        return 2;
    if (!getBitValue(p0, 7))
        return 3;
    return(-1);
}

void gotoX(int x)
{
    if (x < getXPos())
        moveXLeft();
    else if (x > getXPos())
        moveXRight();
    while (x != getXPos()) {
        Sleep(1);
    }
    stopX();
}

void gotoY(int y)
{
    if (y < getYPos())
        moveYOutside();
    else if (y > getYPos())
        moveYInside();
    while (y != getYPos()) {
        Sleep(1);
    }
    stopY();
}

void gotoZ(int z)
{
    if (z < getZPos())
        moveZDown();
    else if (z > getZPos())
        moveZUp();
    while (z != getZPos()) {
        Sleep(1);
    }
    stopZ();
}

void gotoZUp()
{
    moveZUp();
    while (1) {
        taskENTER_CRITICAL();
        uInt8 p0 = readDigitalU8(0);
        uInt8 p1 = readDigitalU8(1);
        taskEXIT_CRITICAL();
        if (!getBitValue(p1, 2))
            break;
        if (!getBitValue(p1, 0))
            break;
        if (!getBitValue(p0, 6))
            break;
    }
    stopZ();

 }

void gotoZDown()
{
    moveZDown();
    while (1) {
        if (getZPos() == 3)
            break;
        if (getZPos() == 2)
            break;
        if (getZPos() == 1)
            break;
    }
    stopZ();
}

void putPartInCell() {

    gotoZUp();
    gotoY(3);
    gotoZDown();
    gotoY(2);

}

void takePartFromCell() {

    gotoY(3);
    gotoZUp();
    gotoY(2);
    gotoZDown();
}

void menu() {
    printf("\033[0;32m");
    puts("\nPHARMACY STORAGE SYSTEM");
    printf("\033[0m");
    puts("------------------------------------MENU-------------------------------------------");
    puts("(1) : Store a product.");
    puts("(2) : Retrive a product with specified (x,z) position.");
    puts("(3) : Retrieve a product with specified national drug code.");
    puts("(4) : Search for product national drug code.");
    puts("(5) : Show list of stored products.");
    puts("(6) : Show record of removed drugs due to expiry date.");
    puts("(sw1) :Press switch 1 to remove all expired drugs from storage");
    puts("(sw1 & sw2) : EMERGENCY STOP");
    puts("(x) : Exit program.");
}

void flashLED(bool stop) {

        int delay=500;
    
        taskENTER_CRITICAL();
        uInt8 p = readDigitalU8(2);
        taskEXIT_CRITICAL();
        setBitValue(&p, 0, 1);
        if (stop) {
            setBitValue(&p, 1, 1);
            delay = 250;
        }
        writeDigitalU8(2, p);
        vTaskDelay(delay);
        taskENTER_CRITICAL();
        p = readDigitalU8(2);
        taskEXIT_CRITICAL();
        setBitValue(&p, 0, 0);
        if(stop)
            setBitValue(&p, 1, 0);
        writeDigitalU8(2, p);
        vTaskDelay(delay);  
}
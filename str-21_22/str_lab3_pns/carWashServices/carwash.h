#pragma once
#include"pch.h"
#include<interface_simulator.h>

void initializeCarWash();
void setBitValue(uInt8* variable, int n_bit, int new_value_bit);
int  getBitValue(uInt8 value, uInt8 bit_n);

void moveBrushRight();
void moveBrushLeft();
void stopHorizMovement();
void moveBrushUp();
void moveBrushDown();

void ceilingBrushRotate();
void ceilingBrushStop();
void lateralBrushRotate();
void lateralBrushStop();

void stopVerticalMovement();
bool isBrushUp();
bool isBrushDown();
bool isBrushLeft();
bool isBrushRight();

bool isLeftInfraRedOn();
bool isRightInfraRedOn();
uInt8 save_state();
void resume_state(uInt8 state);

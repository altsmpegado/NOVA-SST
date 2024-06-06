// SplitterConveyor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <conio.h>
#include <stdio.h>
extern "C" {
#include <interface.h>
#include <SplitterConveyor.h>
}

int main()
{
	printf("Welcome to the Splitter Conveyor Application\n");
	initializeHardwarePorts();
	int t = -1;

	printf("\n\nPress: q, a, z or 0");
	while (t != '0') {
		t= _getch();
		switch (t) {
		case 'q': cylinder2MoveForward(); break;
		case 'a': cylinder2MoveBackward(); break;
		case 'z': cylinder2Stop(); break;
		}
	}
	
	writeDigitalU8(2, 0x00);
	closeChannels();
	return 0;
}

#include <jni.h>
#include <SplitterConveyor.h>

JNIEXPORT void JNICALL Java_SplitterConveyor_initializeHardwarePorts(JNIEnv* ignoreEnv, jclass ignorejClass) {
	initializeHardwarePorts();
}

JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder1MoveForward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	cylinder1MoveForward();
}

JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder1MoveBackward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	cylinder1MoveBackward();
}

JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder1Stop(JNIEnv* ignoreEnv, jclass ignorejClass) {
	cylinder1Stop();
}

JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder2MoveForward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	cylinder2MoveForward();
}

JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder2MoveBackward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	cylinder2MoveBackward();
}

JNIEXPORT void JNICALL Java_SplitterConveyor_cylinder2Stop(JNIEnv* ignoreEnv, jclass ignorejClass) {
	cylinder2Stop();
}
  
JNIEXPORT void JNICALL Java_SplitterConveyor_cylinderStartMoveForward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	cylinderStartMoveForward();
}
  
JNIEXPORT void JNICALL Java_SplitterConveyor_cylinderStartMoveBackward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	cylinderStartMoveBackward();
}

JNIEXPORT void JNICALL Java_SplitterConveyor_cylinderStartStop(JNIEnv* ignoreEnv, jclass ignorejClass) {
	cylinderStartStop();
}
 
JNIEXPORT void JNICALL Java_SplitterConveyor_conveyorMove(JNIEnv* ignoreEnv, jclass ignorejClass) {
	conveyorMove();
}

JNIEXPORT void JNICALL Java_SplitterConveyor_conveyorStop(JNIEnv* ignoreEnv, jclass ignorejClass) {
	conveyorStop();
}

JNIEXPORT jint JNICALL Java_SplitterConveyor_getCylinder1Position(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return getCylinder1Position();
}

JNIEXPORT jint JNICALL Java_SplitterConveyor_getCylinder2Position(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return getCylinder2Position();
}
 
JNIEXPORT jint JNICALL Java_SplitterConveyor_getCylinderStartPosition(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return getCylinderStartPosition();
}
  
JNIEXPORT jint JNICALL Java_SplitterConveyor_getIdentificationSensors(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return getIdentificationSensors();
}

JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isCylinder1MovingForward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isCylinder1MovingForward();
}

JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isCylinder1MovingBackward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isCylinder1MovingBackward();
}

JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isCylinder2MovingForward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isCylinder2MovingForward();
}

JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isCylinder2MovingBackward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isCylinder2MovingBackward();
}

JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isCylinderStartMovingForward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isCylinderStartMovingForward();
}

JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isCylinderStartMovingBackward(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isCylinderStartMovingBackward();
}

JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isPartAtDock1(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isPartAtDock1();
}
  
JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isPartAtDock2(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isPartAtDock2();
}
  
JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isDock1Active(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isDock1Active();
}
  
JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isDock2Active(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isDock2Active();
}
  
JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isDockEndActive(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isDockEndActive();
}
  
JNIEXPORT jboolean JNICALL Java_SplitterConveyor_isLEDActive(JNIEnv* ignoreEnv, jclass ignorejClass) {
	return isLEDActive();
}

JNIEXPORT jint JNICALL Java_SplitterConveyor_readPort(JNIEnv* ignoreEnv, jclass ignorejClass, jint ignorejint) {
	return readPort(ignorejint);
}

JNIEXPORT void JNICALL Java_SplitterConveyor_writePort(JNIEnv* ignoreEnv, jclass ignorejClass, jint ignorejint, jint ignorejint2) {
	writePort(ignorejint, ignorejint2);
}

JNIEXPORT void JNICALL Java_SplitterConveyor_setBitValue(JNIEnv* ignoreEnv, jclass ignorejClass, jint ignorejint, jint ignorejint2, jint ignorejint3) {
	setBitValue(ignorejint, ignorejint2, ignorejint3);
}

JNIEXPORT void JNICALL Java_SplitterConveyor_flashLED(JNIEnv* ignoreEnv, jclass ignorejClass, jint ignorejint) {
	flashLED(ignorejint);
}

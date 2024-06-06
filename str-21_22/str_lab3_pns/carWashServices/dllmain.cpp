// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <stdio.h>

#include<hpSimHandlers.h>
#include<carwash.h>

#include"resource.h"

HINSTANCE g_hInst = NULL;
extern "C" bool dialogLaunch();

char logHpSim[512] = "";

bool initializationFinished = false;
int  selectedWash = 0;
int  manualCommand = 0;
int stopresume = 0;
uInt8 mov_state = 0;

LOG_HANDLER getLogString(void)
{
	return logHpSim;
}

void sprintfLog(const char* message) {
	strcpy_s(logHpSim, sizeof(logHpSim), message);
}

TRANSITION_HANDLER washIsInitialized() {
	return initializationFinished;
}

TRANSITION_HANDLER arrivedBelow() {

	return isBrushDown();
}

TRANSITION_HANDLER arrivedAbove() {
	return isBrushUp();
}

TRANSITION_HANDLER arrivedAtLeft() {

	return isBrushLeft();	
}

TRANSITION_HANDLER arrivedAtRight() {

	return isBrushRight();
}

TRANSITION_HANDLER  contourDetected() {
	return isLeftInfraRedOn() || isRightInfraRedOn();
}

TRANSITION_HANDLER  contourNotDetected() {
	return !contourDetected() && !isBrushDown();
}

TRANSITION_HANDLER edgeCar() {
	return !contourDetected() && isBrushDown();
}

TRANSITION_HANDLER simpleSelected() {
	return selectedWash == IDC_SIMPLE;
}

TRANSITION_HANDLER professionalSelected() {
	return selectedWash == IDC_PRO;
}

TRANSITION_HANDLER buttonRightClicked() {
	return manualCommand == IDC_BUTTON_RIGHT;
}

TRANSITION_HANDLER buttonLeftClicked() {
	return manualCommand == IDC_BUTTON_LEFT;
}

TRANSITION_HANDLER buttonUpClicked() {
	return manualCommand == IDC_BUTTON_UP;
}

TRANSITION_HANDLER buttonDownClicked() {
	return manualCommand == IDC_BUTTON_DOWN;
}

TRANSITION_HANDLER buttonStopClicked() {
	return manualCommand == IDC_BUTTON_STOP;
}

TRANSITION_HANDLER buttonEmergencyStop() {
	return stopresume == IDC_EMRG_STOP;
}

TRANSITION_HANDLER buttonResume() {
	return stopresume == IDC_EMERG_RESUME;
}

//extern "C" __declspec(dllexport) void __cdecl
PLACE_HANDLER  initializingKit(long tokens)
{

	initializationFinished = false;
	initializeCarWash();
	initializationFinished = true;
	// if returning false, 
	// visits HANDLER at every step 
	dialogLaunch();
	return true;
}


//PLACE_HANDLER moveRight(long tokens)
PLACE_HANDLER movingRight(long tokens) {
	sprintf_s(logHpSim, sizeof(logHpSim), "doing movingRight:  %ld", tokens);

	stopVerticalMovement();
	moveBrushRight();
	return true;
}

PLACE_HANDLER movingLeft(long tokens)
{
	stopVerticalMovement();
	moveBrushLeft();
	return true;
}

PLACE_HANDLER movingUp(long tokens)
{
	stopHorizMovement();
	moveBrushUp();

	return true;

}

PLACE_HANDLER movingDown(long tokens)
{
	// ensures moveDown has got the right signature
	CHECK_SIGNATURE_PLACE_HANDLER(movingDown);

	stopHorizMovement();
	moveBrushDown();

	return true;
}

PLACE_HANDLER rightRight(long tokens)
{
	moveBrushRight();
	return true;
}

PLACE_HANDLER leftLeft(long tokens)
{
	moveBrushLeft();
	return true;
}

PLACE_HANDLER upUp(long tokens)
{
	moveBrushUp();
	return true;
}

PLACE_HANDLER downDown(long tokens)
{
	moveBrushDown();
	return true;
}

PLACE_HANDLER manualRight(long tokens) {
	moveBrushRight();
	manualCommand = 0;
	return true;
}

PLACE_HANDLER manualLeft(long tokens) {
	moveBrushLeft();
	manualCommand = 0;
	return true;
}

PLACE_HANDLER manualUp(long tokens) {
	moveBrushUp();
	manualCommand = 0;
	return true;
}

PLACE_HANDLER manualDown(long tokens) {
	moveBrushDown();
	manualCommand = 0;
	return true;
}

PLACE_HANDLER manualStop(long tokens) {
	stopHorizMovement();
	stopVerticalMovement();
	return true;
}

PLACE_HANDLER stopVertical(long tokens) {
	stopVerticalMovement();
	return true;
}

PLACE_HANDLER endingSimple(long tokens) {
	selectedWash = 0;
	return true;
}

PLACE_HANDLER endingProfessional(long tokens) {
	selectedWash = 0;
	return true;
}

PLACE_HANDLER rotateCeiling(long tokens) {
	ceilingBrushRotate();
	return true;
}

PLACE_HANDLER rotateLateral(long tokens) {
	lateralBrushRotate();
	return true;
}

PLACE_HANDLER stopCeiling(long tokens) {
	ceilingBrushStop();
	return true;
}

PLACE_HANDLER stopLateral(long tokens) {
	lateralBrushStop();
	return true;
}

PLACE_HANDLER saveState(long tokens) {
	mov_state = save_state();
	stopHorizMovement();
	stopVerticalMovement();
	ceilingBrushStop();
	lateralBrushStop();
	stopresume = 0;
	return true;
}

PLACE_HANDLER resumeState(long tokens) {
	resume_state(mov_state);
	stopresume = 0;
	return true;
}

LOG_ALERT_HANDLER alertMissingTransitions(void) {
	//returning false prevents the annoying messages
	return false;
}

LOG_ALERT_HANDLER alertMissingPlaces(void) {
	//returning false prevents the annoying messages
	return false;
}

TOKEN_CHANGED_HANDLER onTokenEnterPlace_moveDown(long tokensBefore, long tokensNow) {
	if (tokensBefore == 0 && tokensNow > 0) {
		//do Something here		
	}
}

TOKEN_CHANGED_HANDLER onTokenLeavePlace_moveDown(long tokensBefore, long tokensNow)
{
	if (tokensBefore >= 0 && tokensNow == 0) {
		//do Something here		
	}

}

HANDLE myHandle; // these declarations should be placed at the begining
HWND hwndGoto;

extern "C" INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int x = 0;
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			if (IsDlgButtonChecked(hDlg, IDC_SIMPLE))
				selectedWash = IDC_SIMPLE;
			if (IsDlgButtonChecked(hDlg, IDC_PRO))
				selectedWash = IDC_PRO;
			EndDialog(hDlg, IDOK);
			break;
		case IDCANCEL:
			selectedWash = 0;
			manualCommand = 0;
			stopresume = 0;
			EndDialog(hDlg, IDCANCEL);
			break;
		case IDC_BUTTON_DOWN:
		case IDC_BUTTON_LEFT:
		case IDC_BUTTON_UP:
		case IDC_BUTTON_RIGHT:
		case IDC_BUTTON_STOP:
			manualCommand = LOWORD(wParam);
			break;
		case IDC_SIMPLE:
		case IDC_PRO:
			selectedWash = LOWORD(wParam);
			break;
		case IDC_EMRG_STOP:
		case IDC_EMERG_RESUME:
			stopresume = LOWORD(wParam);
		}
		break;
	case WM_CLOSE:
		::EndDialog(hDlg, IDOK);
		return TRUE;
	case WM_DESTROY:
		DestroyWindow(hDlg);
		PostQuitMessage(0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

DWORD WINAPI myThread(LPVOID lpParameter) {
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DialogProc);
	return 0;
}

extern "C" bool dialogLaunch() {
	DWORD myThreadID;
	myHandle = CreateThread(0, 0, myThread, NULL, 0, &myThreadID);
	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hInst = hModule;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


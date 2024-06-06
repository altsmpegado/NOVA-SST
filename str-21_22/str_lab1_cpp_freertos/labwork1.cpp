// labwork1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <conio.h>
#include <cstdio>
#include <stdlib.h>
#include <windows.h> //for Sleep function
#include <time.h>
#include <list>
#include <stdbool.h>
#include <my_interaction_functions.h>
#include <string.h>
#include <math.h>

extern "C" {
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <interface.h>	
#include <interrupts.h>
}

#define mainREGION_1_SIZE   8201
#define mainREGION_2_SIZE   29905
#define mainREGION_3_SIZE   7607


/*******************STRUCTURES******************/
typedef struct {
	int x;
	int z;
} Pos;

typedef struct {
	bool xcal;
	bool zcal;
} calibrated;

struct _drug {
	char name[50];
	char labname[50];
	char natcode[9];
	int expiry; //in seconds
	TimerHandle_t xTimer;
	short int in_out; // 1 for in, 0 for out, maybe bool
	Pos position;
};

typedef struct _drug* drug; // TAD

struct {
	drug matriz[3][3];
	int occupied;
	drug expired[50];
	bool led1state;
	uInt8 movestate;
	bool STOP;
	bool IDLE;
	calibrated c;
} w; /*w stands for warehouse*/
/***********************************************/

/*************************PROTOTYPES************************/
void initialization();
void shutdown();
drug createDrug(char* name, char* labname, char* natcode, int expiry, short int io, int xpos, int zpos);
void deletesDrug(drug d);
drug returnsDrug(Pos mp);
void insertsDrug(drug d);
void removesDrug(drug d);
bool verifyNationalDrugCode(char* natdrugcode);
bool verifyCellState(Pos mp);
bool verifyIfWarehouseFull();
void storedDrugsList();
void showsDrugInfo();
void expiredDrugsList();
void showsEmptyCells();
void putPartInCellRequest();
void takePartFromCellxz();
void takePartFromCellRequest();
Pos closestPosition(char* natdrugcode);
Pos lowestPosition(int zpos);
void drugHasExpired(TimerHandle_t xTimer);
int getLocalTime_s();
int getexpirydate(int seconds);
SYSTEMTIME getexpirydate_hms(int expiry); /*function to convert drug expiry date into hh:mm:ss format*/
/***********************************************/

/*****************Semaphores********************/
xSemaphoreHandle semaphoreX;
xSemaphoreHandle semaphoreZ;
xSemaphoreHandle semaphoreY;
xSemaphoreHandle semaphoreXZ;
xSemaphoreHandle sem_put_req;
xSemaphoreHandle sem_take_req;
xSemaphoreHandle sem_put_done;
xSemaphoreHandle sem_take_done;
xSemaphoreHandle sem_exp_req;
xSemaphoreHandle sem_exp_done;
xSemaphoreHandle sem_deliver_done;
xSemaphoreHandle sem_in_plat;
xSemaphoreHandle sem_idle_req;
xSemaphoreHandle sem_idle_done;
/***********************************************/

/*********************Mailboxes*********************/
xQueueHandle mb_xcal; //mailbox x calibration
xQueueHandle mb_zcal; //mailbox z calibration
xQueueHandle mbx; //mailbox x position
xQueueHandle mby; // mailbox y position
xQueueHandle mbz; //mailbox z position
xQueueHandle mbxz; //mailbox gotoxz 
xQueueHandle mb_input; //mailbox input
xQueueHandle mb_pt_req; //mailbox put take request
xQueueHandle mb_del_rec; //mailbox deliver receive request
/***********************************************/

/**************Task Handlers****************/
TaskHandle_t xHandler_input;
TaskHandle_t xHandler_xcal;
TaskHandle_t xHandler_zcal;
TaskHandle_t xHandler_stop;
TaskHandle_t xHandler_gtx;
TaskHandle_t xHandler_IDLE;
TaskHandle_t xHandler_gty;
TaskHandle_t xHandler_gtz;
TaskHandle_t xHandler_gtxz;
TaskHandle_t xHandler_put;
TaskHandle_t xHandler_take;
TaskHandle_t xHandler_storage;
TaskHandle_t xHandler_cells;
TaskHandle_t xHandler_expired;
TaskHandle_t xHandler_flash;
TaskHandle_t xHandler_resume;
TaskHandle_t xHandler_reset;
TaskHandle_t xHandler_deliver;
/***********************************************/

/***********************************CRITICAL FUNCTIONS freeRTOS*********************************************/

void vAssertCalled(unsigned long ulLine, const char* const pcFileName)
{
	static BaseType_t xPrinted = pdFALSE;
	volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;
	/* Called if an assertion passed to configASSERT() fails.  See
	http://www.freertos.org/a00110.html#configASSERT for more information. */
	/* Parameters are not used. */
	(void)ulLine;
	(void)pcFileName;
	printf("ASSERT! Line %ld, file %s, GetLastError() %ld\r\n", ulLine, pcFileName, GetLastError());

	taskENTER_CRITICAL();
	{
		/* Cause debugger break point if being debugged. */
		__debugbreak();
		/* You can step out of this function to debug the assertion by using
		   the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		   value. */
		while (ulSetToNonZeroInDebuggerToContinue == 0)
		{
			__asm { NOP };
			__asm { NOP };
		}
	}
	taskEXIT_CRITICAL();
}

static void  initialiseHeap(void)
{
	static uint8_t ucHeap[configTOTAL_HEAP_SIZE];
	/* Just to prevent 'condition is always true' warnings in configASSERT(). */
	volatile uint32_t ulAdditionalOffset = 19;
	const HeapRegion_t xHeapRegions[] =
	{
		/* Start address with dummy offsetsSize */
		{ ucHeap + 1,mainREGION_1_SIZE },
		{ ucHeap + 15 + mainREGION_1_SIZE,mainREGION_2_SIZE },
		{ ucHeap + 19 + mainREGION_1_SIZE +
				mainREGION_2_SIZE,mainREGION_3_SIZE },
		{ NULL, 0 }
	};


	configASSERT((ulAdditionalOffset +
		mainREGION_1_SIZE +
		mainREGION_2_SIZE +
		mainREGION_3_SIZE) < configTOTAL_HEAP_SIZE);
	/* Prevent compiler warnings when configASSERT() is not defined. */
	(void)ulAdditionalOffset;
	vPortDefineHeapRegions(xHeapRegions);
}
/****************************************************************************************************************/

/***************************************************Drug related functions**********************************************************************/

/******************************************************************************
* Function name: createDrug
* Description: creates a drug
* Type: drug
* Parameters: drug name, lab name, national drug code, expiry time, in and out state, x position, z position
* Return: created drug
*******************************************************************************/
drug createDrug(char* name, char* labname, char* natcode, int expiry, short int io, int xpos, int zpos) {

	drug d = (drug)malloc(sizeof(struct _drug));
	if (d == NULL)
		return NULL;
	strcpy_s(d->name, sizeof(d->name), name);
	strcpy_s(d->labname, sizeof(d->labname), labname);
	strcpy_s(d->natcode, sizeof(d->natcode), natcode);
	d->expiry = getexpirydate(expiry);
	d->in_out = io;
	d->position.x = xpos;
	d->position.z = zpos;
	d->xTimer = xTimerCreate(natcode, expiry * 600, pdFALSE, (void*)0, drugHasExpired);

	return d;
}

/******************************************************************************
* Function name: deletesDrug
* Description: deletes a drug, freeing the memory allocated to it
* Type: void
* Parameters: drug
* Return:
*******************************************************************************/
void deletesDrug(drug d) {
	xTimerDelete(d->xTimer, 0);
	free(d);
}

/******************************************************************************
* Function name: returnsDrug
* Description: returns a drug from the warehouse
* Type: drug
* Parameters: drug position
* Return: drug
*******************************************************************************/
drug returnsDrug(Pos mp) {
	return w.matriz[mp.x - 1][mp.z - 1];
}

/******************************************************************************
* Function name: insertsDrug
* Description: inserts a drug in the warehouse
* Type: void
* Parameters: drug
* Return:
*******************************************************************************/
void insertsDrug(drug d) {
	w.matriz[d->position.x - 1][d->position.z - 1] = d;
	w.occupied++;
}

/******************************************************************************
* Function name: removesDrug
* Description: removes a drug from the warehouse
* Type: void
* Parameters: drug
* Return:
*******************************************************************************/
void removesDrug(drug d) {
	w.matriz[d->position.x - 1][d->position.z - 1] = NULL;
	w.occupied--;
}
/**************************************************************************************************************************************************/

/*******************Complementary functions (not tasks)********************************************************************************************/

/******************************************************************************
* Function name: initialization
* Description: function called on startup to initialize the matrix and the other warehouse variables
* Type:
* Parameters:
* Return:
*******************************************************************************/
void initialization() {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			w.matriz[i][j] = NULL;
	}
	for (int j = 0; j < 3; j++)
		w.expired[j] = NULL;

	w.led1state = false;
	w.occupied = 0;
	w.STOP = false;
	w.IDLE = false;
	w.c = { false, false };
}

/******************************************************************************
* Function name: shutdown
* Description: function that deletes all the allocated drugs in memory and their timers
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void shutdown() {

	int k = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			drug d = returnsDrug({ i + 1, j + 1 });
			if (d != NULL)
				deletesDrug(d);
		}
	}
	while (w.expired[k] != NULL) {
		deletesDrug(w.expired[k]);
		k++;
	}
}

/******************************************************************************
* Function name: storedDrugsList
* Description: shows the list of all of the stored drugs in the warehouse
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void storedDrugsList() { /*needs algorith to only print the repeated drugs once*/

	if (w.occupied != 0) {
		puts("\nHere's the list of all the stored drugs: ");
		puts("---------------------------------------------");
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (w.matriz[i][j] != NULL) {
					printf("\nDrug at position (x,z) = %d,%d\n", i + 1, j + 1);
					printf("Name: %s\n", w.matriz[i][j]->name);
					printf("Labname: %s\n", w.matriz[i][j]->labname);
					printf("National Drug Code: %s\n", w.matriz[i][j]->natcode);
					SYSTEMTIME st = getexpirydate_hms(w.matriz[i][j]->expiry);
					printf("Expiry date: %02d:%02d:%02d\n", st.wHour, st.wMinute, st.wSecond);
					puts("---------------------------------------------");
				}
			}
		}
	}
	else puts("\nThere are no drugs in the warehouse!");
}

/******************************************************************************
* Function name: showsDrugInfo
* Description: shows all of the information of a drug with a given national drug code
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void showsDrugInfo() {

	char natcode[9];

	if (w.occupied != 0) {
		vTaskSuspend(xHandler_input);
		puts("\nInsert national drug code: ");
		fgets(natcode, sizeof(natcode), stdin);
		vTaskResume(xHandler_input);
		if (verifyNationalDrugCode(natcode) == true) {
			puts("\nHere are all the ocurrences of the specified drug:");
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (w.matriz[i][j] != NULL) {
						if (strcmp(w.matriz[i][j]->natcode, natcode) == 0) {
							SYSTEMTIME st = getexpirydate_hms(w.matriz[i][j]->expiry);
							printf("In position (%d,%d) , with expiration date %02d:%02d:%02d\n", w.matriz[i][j]->position.x, w.matriz[i][j]->position.z, st.wHour, st.wMinute, st.wSecond);
						}
					}
				}
			}
		}
		else puts("\nDrug not found!");
	}
	else puts("There are no drugs in the warehouse!");
}

/******************************************************************************
* Function name: expiredDrugsList
* Description: shows the list of all expired drugs untill then
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void expiredDrugsList() {

	int i = 0;
	if (w.expired[0] != NULL) {
		puts("\nHere's the list of all the expired drugs untill now!");
		while (w.expired[i] != NULL) {
			puts(w.expired[i]->name);
			puts(w.expired[i]->labname);
			puts(w.expired[i]->natcode);
			SYSTEMTIME st = getexpirydate_hms(w.expired[i]->expiry);
			printf("Expired at %02d:%02d:%02d\n", st.wHour, st.wMinute, st.wSecond);
			i++;
		}
	}
	else puts("\nThere are no expired drugs yet!");

}

/******************************************************************************
* Function name: putPartInCellRequest
* Description: function that sends a request via mailbox to insert a new drug in the warehouse
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void putPartInCellRequest() {

	char cx, cz;
	char drugname[50], labname[50], natcode[9], expiry[10];
	int expirydate;

	if (verifyIfWarehouseFull() == false) {
		vTaskSuspend(xHandler_input); /*Temporarly suspends the input task*/
		puts("\nInsert drug name: ");
		fgets(drugname, sizeof(drugname), stdin);
		puts("Insert laboratory name : ");
		fgets(labname, sizeof(labname), stdin);
		puts("Insert national drug code: ");
		fgets(natcode, sizeof(natcode), stdin);
		puts("Insert expiry date (in seconds): ");
		fgets(expiry, sizeof(expiry), stdin);
		sscanf_s(expiry, "%d", &expirydate);
		vTaskResume(xHandler_input); /*Resumes the input task*/
		showsEmptyCells();
		puts("Insert x value: ");
		xQueueReceive(mb_input, &cx, portMAX_DELAY);
		puts("Insert z value: ");
		xQueueReceive(mb_input, &cz, portMAX_DELAY);
		Pos mp = { cx - '0' ,cz - '0' };
		if ((mp.x >= 1 && mp.x <= 3) && (mp.z >= 1 && mp.z <= 3)) {
			if (verifyCellState(mp) == false) {
				drug d = createDrug(drugname, labname, natcode, expirydate, 1, mp.x, mp.z);
				xQueueSend(mb_pt_req, &d, portMAX_DELAY);
			}
			else puts("\nCell currently in use!");
		}
		else puts("\nInvalid input!");
	}
	else puts("\nThere are no cells avaiable for drug storage!");
}

/******************************************************************************
* Function name: takePartFromCellxz
* Description: function that sends a request via mailbox to remove a drug with a given position
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void takePartFromCellxz() {

	char cx, cz;
	if (w.occupied != 0) {
		puts("Insert x value: ");
		xQueueReceive(mb_input, &cx, portMAX_DELAY);
		puts("Insert z value: ");
		xQueueReceive(mb_input, &cz, portMAX_DELAY);
		Pos mp = { cx - '0' ,cz - '0' };
		if ((mp.x >= 1 && mp.x <= 3) && (mp.z >= 1 && mp.z <= 3)) {
			drug d = returnsDrug(mp);
			if (d != NULL) {
				d->in_out = 0;
				xQueueSend(mb_pt_req, &d, portMAX_DELAY);
			}
			else puts("\nThis cell is empty!");
		}
		else puts("\nInvalid input!");
	}
	else puts("\nThere are no drugs in the warehouse!");
}

/******************************************************************************
* Function name: takePartFromCellRequest
* Description: function that sends a request via mailbox to remove a drug with a certain national drug code
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void takePartFromCellRequest() {

	char natcode[9];

	if (w.occupied != 0) {
		vTaskSuspend(xHandler_input);
		puts("\nInsert national drug code: ");
		fgets(natcode, sizeof(natcode), stdin);
		vTaskResume(xHandler_input);
		if (verifyNationalDrugCode(natcode) == true) {
			Pos mp = closestPosition(natcode);
			drug d = returnsDrug(mp);
			d->in_out = 0;
			xQueueSend(mb_pt_req, &d, portMAX_DELAY);
		}
		else puts("\nDrug not found!");
	}
	else puts("\nThere are no drugs in the warehouse!");
}

/******************************************************************************
* Function name: closestPosition
* Description: given a national drug code, it returns the position of the one who's closer to 1,1
* Type: MovPos
* Parameters: national drug code
* Return: position of the drug closer to 1,1
*******************************************************************************/
Pos closestPosition(char* natdrugcode) { /*the closest position from (1,1) where a given drug is stored and soon will be freed --> for take purposes only*/

	Pos mp = { 2, 2 };

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (w.matriz[i][j] != NULL) {
				if (strcmp(w.matriz[i][j]->natcode, natdrugcode) == 0) {
					int modulus1 = (i * i) + (j * j);
					int modulus2 = (mp.x * mp.x) + (mp.z * mp.z);
					if (sqrt((double)modulus1) <= sqrt((double)modulus2))
						mp = { i, j };
				}
			}
		}
	}
	mp.x += 1;
	mp.z += 1;
	return mp;
}

/******************************************************************************
* Function Name: lowestPosition
* Description: function that finds lowest available position, used in IDLE mode organize drugs in storage
* Type: Pos (position strcuture .x.z)
* Parameters:
* Return: returns the lowest available position (x,z)
*******************************************************************************/
Pos lowestPosition(int zpos) {
	for (int j = 0; j < zpos - 1; j++) {
		for (int i = 0; i < 3; i++) {
			if (!verifyCellState({ i + 1, j + 1 }))
				return { i + 1, j + 1 };
		}
	}
	return { 0, 0 };
}

/******************************************************************************
* Function name: verifyNationalDrugCode
* Description: tells the user if a drug with given national code exists
* Type: bool
* Parameters: national drug code
* Return: true = it exists, false = it doesn't exist
*******************************************************************************/
bool verifyNationalDrugCode(char* natdrugcode) {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (w.matriz[i][j] != NULL) {
				if (strcmp(w.matriz[i][j]->natcode, natdrugcode) == 0)
					return true;
			}
		}
	}
	return false;
}

/******************************************************************************
* Function name: verifyCellState
* Description: tells the user if the wanted cell is free
* Type: bool
* Parameters: MovPos data type which holds the x and z coordinates
* Return: true = it is full, false = it is free
*******************************************************************************/
bool verifyCellState(Pos mp) {

	if (w.matriz[(mp.x - 1)][(mp.z - 1)] != NULL)
		return true;
	return false;
}

/******************************************************************************
* Function name: verifyIfWarehouseFull
* Description: tells the user if the warehouse is full
* Type: bool
* Parameters:
* Return: true = it is full, false = it is not full
*******************************************************************************/
bool verifyIfWarehouseFull() {

	if (w.occupied < 9)
		return false;
	return true;
}

/******************************************************************************
* Function name: showsEmptyCells
* Description: shows the user how many cells are free
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void showsEmptyCells() {

	printf("The cells ");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (w.matriz[i][j] == NULL)
				printf("(%d,%d) ", i + 1, j + 1);
		}
	}
	puts("are empty!");
}

/******************************************************************************
* Function name: drugHasExpired
* Description: callback function invoked by a timer when it expires
* Type: void
* Parameters: timer handler
*******************************************************************************/
void drugHasExpired(TimerHandle_t xTimer) {

	puts("\nA drug has expired!");
	w.led1state = true;
}

/******************************************************************************
* Function name: getexpirydate
* Description: returns the expiry date of a drug in seconds
* Type: int
* Parameters: expiry time
* Return: expiry date in seconds
*******************************************************************************/
int getexpirydate(int seconds) {

	SYSTEMTIME st;
	GetLocalTime(&st);
	return (st.wHour * 3600) + (st.wMinute * 60) + st.wSecond + seconds;

}

/******************************************************************************
* Function name: getLocalTime_s
* Description: returns the local time in an int (holds it in seconds)
* Type: int
* Parameters:
* Return: local time in seconds
*******************************************************************************/
int getLocalTime_s() {

	SYSTEMTIME st;
	GetLocalTime(&st);
	return(st.wHour * 3600) + (st.wMinute * 60) + st.wSecond;
}

/******************************************************************************
* Function name: getexpirydate_hms
* Description: converts the expiry time of a drug into a SYSTEMTIME data type
* Type: SYSTEMTIME
* Parameters: expiry time of a drug
* Return: the expiry time in SYSTEMTIME
*******************************************************************************/
SYSTEMTIME getexpirydate_hms(int expiry) {

	SYSTEMTIME st;
	st.wHour = (expiry / 3600);
	st.wMinute = (expiry - (3600 * st.wHour)) / 60;
	st.wSecond = (expiry - (3600 * st.wHour) - (st.wMinute * 60));
	return st;
}

/***********************************************************************************************************************/


/*********************************************************TASKS*********************************************************/
/******************************************************************************
* Function Name: task_manualXCalibration
* Description:  task that alows the cage to be controlled and calibrated with 'a' and 'd' keys
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_manualXCalibration(void* param) {

	char key;
	while (1) {
		if (xQueueReceive(mb_xcal, &key, portMAX_DELAY)) {
			if (getYPos() == 2) {
				if (key == 'a') {
					if (getXPos() != 1)
						moveXLeft();
					else continue;
				}
				else if (key == 'd') {
					if (getXPos() != 3)
						moveXRight();
					else continue;
				}
				while (getXPos() != -1) // para sair do sensor
					Sleep(1);
				while (getXPos() == -1)// até chegar ao sensor
					Sleep(1);
				stopX();
				w.c.xcal = true;
			}
			else puts("\nClose cage to enable movement!");
		}
		else puts("\nFailed to receive x axis key!");
	}
}

/******************************************************************************
* Function Name: task_manualZCalibration
* Description: task that alows the cage to be controlled and calibrated with 'w' and 's' keys
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_manualZCalibration(void* param) {

	char key;
	while (1) {
		if (xQueueReceive(mb_zcal, &key, portMAX_DELAY)) {
			if (getYPos() == 2) {
				if (key == 's') {
					if (getZPos() != 1)
						moveZDown();
					else continue;
				}
				else if (key == 'w') {
					if (getZPos() != 3)
						moveZUp();
					else continue;
				}
				while (getZPos() != -1) // para sair do sensor
					Sleep(1);
				while (getZPos() == -1)// até chegar ao sensor
					Sleep(1);
				stopZ();
				w.c.zcal = true;
			}
			else puts("\nClose cage to enable movement!");
		}
		else puts("\nFailed to receive z axis key!");
	}
}

/******************************************************************************
* Function Name: task_gotoX
* Description: performs movement to a sensor in the x axis
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_gotoX(void* param) {

	while (1) {
		int x;
		if (xQueueReceive(mbx, &x, portMAX_DELAY)) {
			gotoX(x);
			xSemaphoreGive(semaphoreX);
		}
	}
}

/******************************************************************************
* Function Name: task_gotoY
* Description: moves plate inside or outside
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_gotoY(void* param) {

	while (1) {
		int y;
		if (xQueueReceive(mby, &y, portMAX_DELAY)) {
			gotoY(y);
			xSemaphoreGive(semaphoreY);
		}
	}
}

/******************************************************************************
* Function Name: task_gotoZ
* Description: performs movement to a sensor in the z axis
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_gotoZ(void* param) {

	while (1) {
		int z;
		if (xQueueReceive(mbz, &z, portMAX_DELAY)) {
			gotoZ(z);
			xSemaphoreGive(semaphoreZ);
		}
	}
}

/******************************************************************************
* Function Name: task_gotoXZ
* Description: movement in both x and z axis task handler
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_gotoXZ(void* param) {

	while (1) {
		Pos position;
		if (xQueueReceive(mbxz, &position, portMAX_DELAY)) {
			xQueueSend(mbx, &position.x, portMAX_DELAY);
			xQueueSend(mbz, &position.z, portMAX_DELAY);

			if (xSemaphoreTake(semaphoreX, portMAX_DELAY))
				puts("\nFinished GoToX task!");
			if (xSemaphoreTake(semaphoreZ, portMAX_DELAY))
				puts("\nFInished GoToZ task!");
			xSemaphoreGive(semaphoreXZ);
		}
		else puts("\nFailed to receive desired position!");
	}
}

/******************************************************************************
* Function Name: task_deliverANDreceive
* Description: task that controls all the delivery and retrieval of drugs
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_deliverANDreceive(void* param) {

	drug d;
	Pos mp = { 1, 1 };
	while (1) {
		if (xQueueReceive(mb_del_rec, &d, portMAX_DELAY)) {
			xQueueSend(mbxz, &mp, portMAX_DELAY);
			if (xSemaphoreTake(semaphoreXZ, portMAX_DELAY)) {
				while (1) {
					int y = 1;
					xQueueSend(mby, &y, portMAX_DELAY);
					xSemaphoreTake(semaphoreY, portMAX_DELAY);
					puts("\nInsert/Remove the drug from the platform. You have 5 seconds.");
					vTaskDelay(5000);
					y = 2;
					xQueueSend(mby, &y, portMAX_DELAY);
					xSemaphoreTake(semaphoreY, portMAX_DELAY);
					if (d->in_out) {
						if (xSemaphoreTake(sem_in_plat, 2000)) {
							xSemaphoreGive(sem_deliver_done);
							break;
						}
					}
					else if (!xSemaphoreTake(sem_in_plat, 2000)) {
						xSemaphoreGive(sem_deliver_done);
						break;
					}
				}
			}
			else puts("\nFailed to receive signal!");
		}
		else puts("\nFailed to receive signal!");
	}
}

/******************************************************************************
* Function Name: task_putPartInCell
* Description: puts part in cell task
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_putPartInCell(void* param) {

	while (1) {
		if (xSemaphoreTake(sem_put_req, portMAX_DELAY)) {
			putPartInCell();
			xSemaphoreGive(sem_put_done);
		}
		else puts("Failed to receive signal!");
	}
}

/******************************************************************************
* Function Name: task_takePartFromCell
* Description: takes part from cell task
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_takePartFromCell(void* param) {

	while (1) {
		if (xSemaphoreTake(sem_take_req, portMAX_DELAY)) {
			takePartFromCell();
			xSemaphoreGive(sem_take_done);
		}
		else puts("Failed to receive signal!");
	}

}

/******************************************************************************
* Function Name: task_storageCellsHandler
* Description: task that handles put and take requests and receives/delivers drugs
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_storageCellsHandler(void* param) {

	drug d;

	while (1) {
		if (xQueueReceive(mb_pt_req, &d, portMAX_DELAY)) {
			switch (d->in_out) {
			case 1:
				if (w.IDLE) goto skipreceiving;
				xQueueSend(mb_del_rec, &d, portMAX_DELAY);
				xSemaphoreTake(sem_deliver_done, portMAX_DELAY);
				puts("\nDrug was received successfully!");
			skipreceiving:
				xQueueSend(mbxz, &(d->position), portMAX_DELAY);
				if (xSemaphoreTake(semaphoreXZ, portMAX_DELAY)) {
					xSemaphoreGive(sem_put_req);
					if (xSemaphoreTake(sem_put_done, portMAX_DELAY)) {
						insertsDrug(d);
						if (!w.IDLE) {
							xTimerStart(d->xTimer, 0);
							puts("\nDrug was stored successfully!");
						}
						else xSemaphoreGive(sem_idle_done);
					}
				}
				break;
			case 0:
				xQueueSend(mbxz, &(d->position), portMAX_DELAY);
				if (xSemaphoreTake(semaphoreXZ, portMAX_DELAY)) {
					xSemaphoreGive(sem_take_req);
					if (xSemaphoreTake(sem_take_done, portMAX_DELAY)) {
						puts("\nDrug was removed successfully!");
						if (w.IDLE) goto skipdelivery;
						xQueueSend(mb_del_rec, &d, portMAX_DELAY);
						xSemaphoreTake(sem_deliver_done, portMAX_DELAY);
					skipdelivery:
						removesDrug(d);
						if (w.IDLE) xSemaphoreGive(sem_idle_done);
						else {
							if (xTimerIsTimerActive(d->xTimer) == pdTRUE) deletesDrug(d);
							else xSemaphoreGive(sem_exp_done);
							puts("\nDrug was delivered successfully!");
						}
					}
				}
				break;
			}
		}
		else puts("\nFailed to receive request!");
	}
}

/******************************************************************************
* Function Name: task_storageSystem
* Description: this task handles inputs from the user and redirects them to their tasks/functions
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_storageSystem(void* param) {

	char key;
	while (1) {
		menu();
		puts("\nChoose a task: ");
		if (xQueueReceive(mb_input, &key, 20000)) {
			switch (key) {
			case 'a':
				xQueueSend(mb_xcal, &key, portMAX_DELAY); /*goes to X calibration task*/
				break;
			case 'd':
				xQueueSend(mb_xcal, &key, portMAX_DELAY); /* goes to X calibration task*/
				break;
			case 'w':
				xQueueSend(mb_zcal, &key, portMAX_DELAY); /* goes to Z calibration task*/
				break;
			case 's':
				xQueueSend(mb_zcal, &key, portMAX_DELAY); /* goes to Z calibration task*/
				break;
			case '1':
				if (w.c.xcal && w.c.zcal)
					putPartInCellRequest();
				else puts("\nCage not yet calibrated!");
				break;
			case '2':
				if (w.c.xcal && w.c.zcal)
					takePartFromCellxz();
				else puts("\nCage not yet calibrated!");
				break;
			case '3':
				if (w.c.xcal && w.c.zcal)
					takePartFromCellRequest();
				else puts("\nCage not yet calibrated!");
				break;
			case '5':
				storedDrugsList();
				break;
			case '4':
				showsDrugInfo();
				break;
			case '6':
				expiredDrugsList();
				break;
			case 'x':
				taskENTER_CRITICAL();
				writeDigitalU8(2, 0);
				taskEXIT_CRITICAL();
				shutdown();
				vTaskEndScheduler();
				break;
			default:
				break;
			}
		}
		else vTaskResume(xHandler_IDLE);
	}
}

/******************************************************************************
* Function Name: task_receiveInput
* Description: task responsible for all menu inputs, one character inputs
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_receiveInput(void* param) {

	while (1) {
		char c = _getch();
		putchar(c);
		printf("\n");
		if (!xQueueSend(mb_input, &c, portMAX_DELAY))
			puts("\nFailed to send the selected task!");
		vTaskDelay(200);
	}
}

/******************************************************************************
* Function Name: task_emergencyStop
* Description: EMERGENCY STOP activated when both switch 1 and switch 2 are pressed, all tasks are suspended
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_emergencyStop(void* param) {

	while (1) {
		vTaskSuspend(NULL);
		taskENTER_CRITICAL();
		w.movestate = readDigitalU8(2);
		taskEXIT_CRITICAL();
		stopX(); stopY(); stopZ();
		vTaskSuspend(xHandler_xcal);
		vTaskSuspend(xHandler_zcal);
		vTaskSuspend(xHandler_gtx);
		vTaskSuspend(xHandler_gty);
		vTaskSuspend(xHandler_gtz);
		vTaskSuspend(xHandler_gtxz);
		vTaskSuspend(xHandler_deliver);
		vTaskSuspend(xHandler_put);
		vTaskSuspend(xHandler_take);
		vTaskSuspend(xHandler_storage);
		vTaskSuspend(xHandler_cells);
		vTaskSuspend(xHandler_input);
		vTaskSuspend(xHandler_expired);
		vTaskSuspend(xHandler_IDLE);

		w.STOP = true;
		puts("EMERGENCY STOP!");
		puts("S1: Resume     S2: Reset");

	}
}

/******************************************************************************
* Function Name: task_resumeTasks
* Description: RESUME when switch 1 is pressed after EMERGENCY STOP
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_resumeTasks(void* param) {

	while (1) {
		vTaskSuspend(NULL);
		puts("Resuming tasks...");
		taskENTER_CRITICAL();
		writeDigitalU8(2, w.movestate);
		taskEXIT_CRITICAL();
		vTaskResume(xHandler_xcal);
		vTaskResume(xHandler_zcal);
		vTaskResume(xHandler_gtx);
		vTaskResume(xHandler_gty);
		vTaskResume(xHandler_gtz);
		vTaskResume(xHandler_gtxz);
		vTaskResume(xHandler_put);
		vTaskResume(xHandler_take);
		vTaskResume(xHandler_deliver);
		vTaskResume(xHandler_storage);
		vTaskResume(xHandler_cells);
		vTaskResume(xHandler_input);
		vTaskResume(xHandler_expired);
		w.STOP = false;
	}
}

/******************************************************************************
* Function Name: task_resetSystem
* Description: task RESET activated when switch 2 is pressed after EMERGANCY STOP, recalibrates system
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_resetSystem(void* param) {

	char key;
	while (1) {
		vTaskSuspend(xHandler_reset);
		taskENTER_CRITICAL();
		w.movestate = readDigitalU8(2);
		taskEXIT_CRITICAL();
		puts("Perform semi-automatic calibration.");
		w.c = { false, false };
		vTaskResume(xHandler_input);
		vTaskResume(xHandler_storage);
		vTaskResume(xHandler_xcal);
		vTaskResume(xHandler_zcal);
		while (!w.c.xcal || !w.c.zcal)
			vTaskDelay(1);
		xQueueReset(mb_pt_req);
		xQueueReset(mb_del_rec);
		vTaskResume(xHandler_xcal);
		vTaskResume(xHandler_zcal);
		vTaskResume(xHandler_gtx);
		vTaskResume(xHandler_gty);
		vTaskResume(xHandler_gtz);
		vTaskResume(xHandler_gtxz);
		vTaskResume(xHandler_put);
		vTaskResume(xHandler_take);
		vTaskResume(xHandler_deliver);
		vTaskResume(xHandler_storage);
		vTaskResume(xHandler_cells);
		vTaskResume(xHandler_input);
		vTaskResume(xHandler_expired);
		if (xSemaphoreTake(sem_in_plat, 2000))

		w.STOP = false;
	}
}

/******************************************************************************
* Function Name: task_removeExpiredDrugs
* Description: task responsible for removing expired drugs
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_removeExpiredDrugs(void* param) {

	int k = 0;

	while (1) {
		if (xSemaphoreTake(sem_exp_req, portMAX_DELAY)) {
			if (w.occupied != 0) {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if (w.matriz[i][j] != NULL) {
							if (xTimerIsTimerActive(w.matriz[i][j]->xTimer) == pdFALSE) {
								w.expired[k] = w.matriz[i][j];
								k++;
								w.matriz[i][j]->in_out = 0;
								xQueueSend(mb_pt_req, &w.matriz[i][j], portMAX_DELAY);
								xSemaphoreTake(sem_exp_done, portMAX_DELAY);
							}
						}
					}
				}
			}
			else puts("\nThere are no drugs in the warehouse!");
		} w.led1state = false;
	}
}

/******************************************************************************
* Function Name: task_flash_LEDS
* Description: task responsible for flashing both LEDS, calls flashLED in my_inteaction_functions.cpp
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_flashLEDS(void* param) {
	while (true) {
		if (w.led1state || w.STOP)
			flashLED(w.STOP);

	}
}

/******************************************************************************
* Function Name: switch1_rising_isr
* Description: interrupt that controls if switch 1 was pressed
* Type: void
* Parameters: ULONGLONG lastTime
* Return:
*******************************************************************************/
void switch1_rising_isr(ULONGLONG lastTime) {

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
	uInt8 p = readDigitalU8(1);
	taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
	if (getBitValue(p, 6) == 0) {
		if (w.STOP == true)
			BaseType_t xYieldRequired = xTaskResumeFromISR(xHandler_resume);
		else xSemaphoreGiveFromISR(sem_exp_req, &xHigherPriorityTaskWoken);
	}
	if (xHigherPriorityTaskWoken)
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/******************************************************************************
* Function Name: switch2_rising_isr
* Description: interrupt that controls if switch 2 was pressed
* Type: void
* Parameters: ULONGLONG lastTime
* Return:
*******************************************************************************/
void switch2_rising_isr(ULONGLONG lastTime) {

	UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
	uInt8 p = readDigitalU8(1);
	taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
	if (getBitValue(p, 5))
		BaseType_t xYieldRequired = xTaskResumeFromISR(xHandler_stop);
	else if (w.STOP == true)
		BaseType_t xYieldRequired2 = xTaskResumeFromISR(xHandler_reset);

}

/******************************************************************************
* Function Name: pallete_rising_isr
* Description: interrupt to check if cage is full
* Type: void
* Parameters: ULONGLONG lastTime
* Return:
*******************************************************************************/
void pallete_rising_isr(ULONGLONG lastTime) {

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if (getXPos() == 1 && getZPos() == 1) {
		xSemaphoreGiveFromISR(sem_in_plat, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken)
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}

}

/******************************************************************************
* Function Name: task_IDLEstate
* Description: enters IDLE MODE, drugs with a close expiry date are moved to lower positions
* Type: void
* Parameters:
* Return:
*******************************************************************************/
void task_IDLEstate(void* param) {

	Pos lmp; //stands for lowest movpos
	while (1) {
		vTaskSuspend(NULL);
		vTaskSuspend(xHandler_input);
		vTaskSuspend(xHandler_storage);
		puts("\nStorage entering IDLE MODE! Moving nearly expired drug to the lower cells...");
		w.IDLE = true;

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (w.matriz[i][j] != NULL) {
					drug d = returnsDrug({ i + 1, j + 1 });
					if (xTimerIsTimerActive(d->xTimer) && (d->expiry <= ((getLocalTime_s() + 30)))) {
						if (d->position.z != 1) {
							lmp = lowestPosition(d->position.z);
							if (lmp.x && lmp.z) {
								d->in_out = 0;
								xQueueSend(mb_pt_req, &d, portMAX_DELAY);
								xSemaphoreTake(sem_idle_done, portMAX_DELAY);
								d->position = lmp;
								d->in_out = 1;
								xQueueSend(mb_pt_req, &d, portMAX_DELAY);
								xSemaphoreTake(sem_idle_done, portMAX_DELAY);
							}
							else puts("\nThere is no empty cell in a lower row so the drug won't be moved..."); //print not necessary
						}
						else puts("\nDrug already in the lowest row of the warehouse so it won't be moved..."); //print not necessary	
					}
				}
			}
		}
		puts("\nAll nearly expired drugs have been organized. Leaving IDLE mode...");
		vTaskResume(xHandler_input);
		vTaskResume(xHandler_storage);
		w.IDLE = false;
	}
}
/****************************************************************************************************************************/

void myDaemonTaskStartupHook(void)
{
	initialization();
	semaphoreX = xSemaphoreCreateCounting(1000, 0);
	semaphoreZ = xSemaphoreCreateCounting(1000, 0);
	semaphoreY = xSemaphoreCreateCounting(1000, 0);
	semaphoreXZ = xSemaphoreCreateCounting(1000, 0);
	sem_put_req = xSemaphoreCreateCounting(1000, 0);
	sem_take_req = xSemaphoreCreateCounting(1000, 0);
	sem_put_done = xSemaphoreCreateCounting(1000, 0);
	sem_take_done = xSemaphoreCreateCounting(1000, 0);
	sem_exp_req = xSemaphoreCreateBinary();
	sem_exp_done = xSemaphoreCreateCounting(1000, 0);
	sem_deliver_done = xSemaphoreCreateCounting(1000, 0);
	sem_in_plat = xSemaphoreCreateBinary();
	sem_idle_req = xSemaphoreCreateCounting(1000, 0);
	sem_idle_done = xSemaphoreCreateCounting(1000, 0);

	mb_xcal = xQueueCreate(10, sizeof(int));
	mb_zcal = xQueueCreate(10, sizeof(int));
	mbx = xQueueCreate(10, sizeof(int));
	mby = xQueueCreate(10, sizeof(int));
	mbz = xQueueCreate(10, sizeof(int));
	mbxz = xQueueCreate(10, sizeof(Pos));
	mb_input = xQueueCreate(10, sizeof(char));
	mb_pt_req = xQueueCreate(10, sizeof(struct _drug));
	mb_del_rec = xQueueCreate(10, sizeof(struct _drug));

	xTaskCreate(task_manualXCalibration, "task_manualXCalibration", 100, NULL, 1, &xHandler_xcal);
	xTaskCreate(task_manualZCalibration, "task_manualZCalibration", 100, NULL, 1, &xHandler_zcal);
	xTaskCreate(task_gotoX, "task_gotoX", 100, NULL, 1, &xHandler_gtx);
	xTaskCreate(task_gotoZ, "task_gotoZ", 100, NULL, 1, &xHandler_gtz);
	xTaskCreate(task_gotoY, "task_gotoY", 100, NULL, 1, &xHandler_gty);
	xTaskCreate(task_gotoXZ, "task_gotoXZ", 100, NULL, 1, &xHandler_gtxz);
	xTaskCreate(task_deliverANDreceive, "task_deliverANDreceive", 100, NULL, 1, &xHandler_deliver);
	xTaskCreate(task_putPartInCell, "task_putPartInCell", 100, NULL, 1, &xHandler_put);
	xTaskCreate(task_takePartFromCell, "task_takePartFromCell", 100, NULL, 1, &xHandler_take);
	xTaskCreate(task_storageSystem, "task_storageSystem", 100, NULL, 1, &xHandler_storage);
	xTaskCreate(task_storageCellsHandler, "task_storageCellsHandler", 100, NULL, 1, &xHandler_cells);
	xTaskCreate(task_receiveInput, "task_receiveInput", 100, NULL, 1, &xHandler_input);
	xTaskCreate(task_removeExpiredDrugs, "task_removeExpiredDrugs", 100, NULL, 1, &xHandler_expired);
	xTaskCreate(task_flashLEDS, "task_flashLEDS", 100, NULL, 1, &xHandler_flash);
	xTaskCreate(task_emergencyStop, "task_emergencyStop", 100, NULL, 1, &xHandler_stop);
	xTaskCreate(task_resumeTasks, "task_resumeTasks", 100, NULL, 1, &xHandler_resume);
	xTaskCreate(task_resetSystem, "task_resetSystem", 100, NULL, 1, &xHandler_reset);
	xTaskCreate(task_IDLEstate, "task_IDLEstate", 100, NULL, 1, &xHandler_IDLE);

	attachInterrupt(1, 5, switch1_rising_isr, RISING);
	attachInterrupt(1, 6, switch2_rising_isr, RISING);
	attachInterrupt(1, 4, pallete_rising_isr, RISING);
}


int main(int argc, char** argv) {

	initialiseHeap();

	createDigitalInput(0);
	createDigitalInput(1);
	createDigitalOutput(2);

	puts("PLEASE calibrate the cage before proceeding");

	vApplicationDaemonTaskStartupHook = &myDaemonTaskStartupHook;
	vTaskStartScheduler();

}

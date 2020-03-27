#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
//#include "cmsis_os.h"
#include "main.h"
#include "init.h"
#include "tasks.h"

//void TimerTasks_Callback(void const *arg); 

osThreadId tid_firstTask;
osThreadId tid_secondTask;
//osTimerId  tid_TimerTasks; 

osThreadDef(THREADtask_GreenRedLight, osPriorityNormal,1,0);
osThreadDef(THREADtask_BlueLight, osPriorityNormal,1,0);
//osTimerDef(TimerTasks, TimerTasks_Callback);

int main (void) {
//	int tim=1;
	LEDInit();
	osKernelInitialize();                    // initialize CMSIS-RTOS

//	tid_TimerTasks = osTimerCreate(osTimer(TimerTasks), osTimerPeriodic, &tim);
//	if (tid_TimerTasks != NULL) {
//		if(osTimerStart (tid_TimerTasks, 10) != osOK) { }  
//	} 
//	
	tid_firstTask = osThreadCreate(osThread(THREADtask_GreenRedLight), NULL);
	tid_secondTask = osThreadCreate(osThread(THREADtask_BlueLight), NULL);
	
  osKernelStart();                         // start thread execution 
}
 
//void TimerTasks_Callback(void const *arg){
//	if(LEDCounter % 1000 == 0){ 
//		osSignalSet(tid_secondTask, 1); 
//	} else if (LEDCounter==0) {
//		osSignalClear(tid_secondTask, 1);
//	}
//}
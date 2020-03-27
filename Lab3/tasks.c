#include "tasks.h"
#include "lightutils.h"
#include "osObjects.h"
#include "main.h"

short LEDCounter=0;

void THREADtask_GreenRedLight(void const *argument) {
	
	while(1) {
		
		if(LEDCounter>= 10000){
			LEDCounter=0;
		}
		if(LEDCounter % 1000 == 0){ 
			osSignalSet(tid_secondTask, 1); 
		} 
		if (LEDCounter==0) {
			osSignalClear(tid_secondTask, 1);
		}

		lightsOn(RED);
		lightsOn(GREEN);
		osDelay(1000);
		lightsOff(RED);
		lightsOff(GREEN);
		osDelay(1000);
		
		LEDCounter++;
	}
}

void THREADtask_BlueLight(void const *argument) {
	osEvent event;
	while(1) {
		event = osSignalWait(0x0001, osWaitForever);
		if(event.status == osEventSignal){
			lightsOn(BLUE);
			osDelay(1000);
			lightsOff(BLUE);
			osDelay(1000);
		}
		osThreadYield();
	}
}

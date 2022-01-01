/*
 * Internalvalues.h
 *
 *  Created on: 24-Nov-2021
 */
#include<time.h>
#ifndef INTERNALVALUES_H_
#define INTERNALVALUES_H_
#define period1 12
#define period2 5
#define period3 6
#define priority1 1
#define priority2 3
#define priority3 2


void vtaskpressure();
void vtasktemperature();
void vtaskheight();
int findleastdeadline(TickType_t xLastWakeTime);
void vScheduleEDF();
void createtasks();
int findleastdeadline();
int changepriority(int x);

int getTemperature(){
	int upper=100,lower=20;
	int num = (rand() % (upper - lower + 1)) + lower;
	return num;
}

int getPressure(){
	int upper=10,lower=1;
	int num = (rand() % (upper - lower + 1)) + lower;
	return num;
}

int getHeight(){
	int upper=1000,lower=0;
	int num = (rand() % (upper - lower + 1)) + lower;
	return num;
}

#endif /* INTERNALVALUES_H_ */

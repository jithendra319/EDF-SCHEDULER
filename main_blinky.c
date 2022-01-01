#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include "Internalvalues.h"
#include "queue.h"

TaskHandle_t taskHandl[4];

struct info {
  TickType_t period, deadline, execution;
  int priority;
} taskinfo[3];

static QueueHandle_t xQueue = NULL;
int buffer;
void vtasktemperature() {
  xQueueReceive(xQueue, & buffer, portMAX_DELAY);
  TickType_t xLastWakeTime = 0;
  while (1) {
    printf("Temperature of the System is :%d Celsius\n", getTemperature());
    printf("Task:1(Temperature) is executing and Number of ticks is %ld\n\r", (long)xLastWakeTime);
    fflush(stdout);
    vTaskDelayUntil( & xLastWakeTime, pdMS_TO_TICKS(taskinfo[0].period));
    taskinfo[0].deadline += taskinfo[0].period;
    vTaskPrioritySet(taskHandl[3], 3);
    xQueueReceive(xQueue, & buffer, portMAX_DELAY);
  }

}

void vtaskpressure() {
  xQueueReceive(xQueue, & buffer, portMAX_DELAY);
  TickType_t xLastWakeTime = 0;
  while (1) {
    printf("Pressure of the System is :%d Bar\n", getPressure());
    printf("Task:2(Pressure) is executing and Number of ticks is %ld\n\r", (long)xLastWakeTime);
    fflush(stdout);
    vTaskDelayUntil( & xLastWakeTime, pdMS_TO_TICKS(taskinfo[1].period));
    taskinfo[1].deadline += taskinfo[1].period;
    vTaskPrioritySet(taskHandl[3], 3);
    xQueueReceive(xQueue, & buffer, portMAX_DELAY);
  }

}

void vtaskheight() {
  xQueueReceive(xQueue, & buffer, portMAX_DELAY);
  TickType_t xLastWakeTime = 0;
  while (1) {
    printf("Height Obtained is :%d meters\n", getHeight());
    printf("Task:3(Height) is executing and Number of ticks is %ld\n\r", (long)xLastWakeTime);
    fflush(stdout);
    vTaskDelayUntil( & xLastWakeTime, pdMS_TO_TICKS(taskinfo[2].period));
    taskinfo[2].deadline += taskinfo[2].period;
    vTaskPrioritySet(taskHandl[3], 3);
    xQueueReceive(xQueue, & buffer, portMAX_DELAY);
  }

}

void vScheduleEDF() {

  TickType_t Last_wake_time = xTaskGetTickCount();
  int x;
  while (1) {
    int y;
    x = findleastdeadline(Last_wake_time);
    y = changepriority(x);
    xQueueSendToBack(xQueue, & y, (TickType_t) 10);
    vTaskPrioritySet(taskHandl[3], 0);

  }

}

int findleastdeadline(TickType_t xLastWakeTime) {

  int arr[3], cnt = 0;
  for (int i = 0; i < 3; i++) {
    if (xLastWakeTime >= taskinfo[i].deadline - taskinfo[i].period)
      arr[cnt++] = i;
  }

  TickType_t min = taskinfo[cnt - 1].deadline, j = arr[0];

  for (int i = 0; i < cnt; i++) {
    if (taskinfo[arr[i]].deadline <= min) {
      min = taskinfo[arr[i]].deadline;
      j = arr[i];
    }
  }
  return j;
}

void createtasks() {
  for (int i = 0; i < 4; i++) {
    taskHandl[i] = NULL;
  }
  taskinfo[0].priority = priority1;
  taskinfo[1].priority = priority2;
  taskinfo[2].priority = priority3;

  taskinfo[0].period = pdMS_TO_TICKS(12);
  taskinfo[1].period = pdMS_TO_TICKS(5);
  taskinfo[2].period = pdMS_TO_TICKS(6);

  taskinfo[0].deadline = taskinfo[0].period;
  taskinfo[1].deadline = taskinfo[1].period;
  taskinfo[2].deadline = taskinfo[2].period;

  xTaskCreate(vtasktemperature, "T1", 200, NULL, taskinfo[0].priority, & taskHandl[0]);
  xTaskCreate(vtaskpressure, "T2", 200, NULL, taskinfo[1].priority, & taskHandl[1]);
  xTaskCreate(vtaskheight, "T3", 200, NULL, taskinfo[2].priority, & taskHandl[2]);
  xTaskCreate(vScheduleEDF, "T4", 200, NULL, 3, & taskHandl[3]);
  xQueue = xQueueCreate(10, sizeof(unsigned long));
}

void main_blinky() {

  createtasks();
  vTaskStartScheduler();

}

int changepriority(int x) {
  int b = x;
  for (int i = 0; i < 3; i++) {
    if (taskinfo[x].deadline == taskinfo[i].deadline && i != x) {
      if (taskinfo[x].priority < taskinfo[i].priority) {
        b = i;
        vTaskPrioritySet(taskHandl[i], 2);
        vTaskPrioritySet(taskHandl[x], 1);

        break;
      }
    }
  }
  return b;

}

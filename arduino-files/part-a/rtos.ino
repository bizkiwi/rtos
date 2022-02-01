// MG7013 Embedded Systems
// Assignment 2 Part A
// by Stephen Julian
// Unitec ID: 1041664
// Note: The following tutorial was found helpful for completing this work:
// https://create.arduino.cc/projecthub/feilipu/using-freertos-multi-tasking-in-arduino-ebc3cc

#include <Arduino_FreeRTOS.h>

void task_B0( void *pvParameters );
void task_B1( void *pvParameters );
void task_B2( void *pvParameters );
void task_B3( void *pvParameters );

void setup() {
  // we put our setup code here, to run once:
  
  xTaskCreate(
    task_B0
    , (const portCHAR *)"task_B0" // A name just for humans
    , 128 // Stack size
    , NULL
    , 2 // priority
    , NULL );
  
  xTaskCreate(
  task_B1
    , (const portCHAR *)"task_B1" // A name just for humans
    , 128 // Stack size
    , NULL
    , 2 // priority
    , NULL );
  
  xTaskCreate(
  task_B2
    , (const portCHAR *)"task_B2" // A name just for humans
    , 128 // Stack size
    , NULL
    , 2 // priority
    , NULL );
	
  xTaskCreate(
  task_B3
    , (const portCHAR *)"task_B3" // A name just for humans
    , 128 // Stack size
    , NULL
    , 2 // priority
    , NULL );
}


void loop() {
  // main code goes here, to run repeatedly...
  // however after setup all our code executes within tasks.

}


// This task flashes the LED connected to pin D11 at a rate of 250ms ON then 250ms OFF repeated.
void task_B0(void *pvParameters) 
{
  (void) pvParameters;
  pinMode(11, OUTPUT); // initialize digital pin D11 as an OUTPUT.
  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(11, HIGH); // turn LED ON
    vTaskDelay( 250 / portTICK_PERIOD_MS ); // wait for 250ms
    digitalWrite(11, LOW); // turn LED OFF
    vTaskDelay( 250 / portTICK_PERIOD_MS ); // wait for 250ms
  }
}


// This task flashes the LED connected to pin D10 at a rate of 500ms ON then 500ms OFF repeated.
void task_B1(void *pvParameters) 
{
  (void) pvParameters;
  pinMode(10, OUTPUT); // initialize digital pin D10 as an OUTPUT.
  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(10, HIGH); // turn LED ON
    vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for 500ms
    digitalWrite(10, LOW); // turn LED OFF
    vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for 500ms
  }
}


// This task flashes the LED connected to pin D9 at a rate of 1000ms ON then 1000ms OFF repeated.
void task_B2(void *pvParameters) 
{
  (void) pvParameters;
  pinMode(9, OUTPUT); // initialize digital pin D9 as an OUTPUT.
  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(9, HIGH); // turn LED ON
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for 1000ms
    digitalWrite(9, LOW); // turn LED OFF
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for 1000ms
  }
}


// This task flashes the LED connected to pin D8 at a rate of 2000ms ON then 2000ms OFF repeated.
void task_B3(void *pvParameters) 
{
  (void) pvParameters;
  pinMode(8, OUTPUT); // initialize digital pin D8 as an OUTPUT.
  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(8, HIGH); // turn LED ON
    vTaskDelay( 2000 / portTICK_PERIOD_MS ); // wait for 2000ms
    digitalWrite(8, LOW); // turn LED OFF
    vTaskDelay( 2000 / portTICK_PERIOD_MS ); // wait for 2000ms
  }
}

// MG7013 Embedded Systems
// Assignment 2 Part B
// by Stephen Julian
// Unitec ID: 1041664
// Note: The following tutorials were found helpful for completing this work:
// https://create.arduino.cc/projecthub/feilipu/using-freertos-multi-tasking-in-arduino-ebc3cc
// https://create.arduino.cc/projecthub/feilipu/using-freertos-semaphores-in-arduino-ide-b3cd6c
// https://www.electroschematics.com/9809/arduino-8-bit-binary-led/

#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).

SemaphoreHandle_t xMySemaphore;
byte countPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; // binary counter pins from LSB to MSB
String binCount = "00000000"; // at first run all LED's will be OFF.

void task_Live( void *pvParameters );
void task_Generator( void *pvParameters );
void task_Display( void *pvParameters );

void setup() {
  // we put our setup code here, to run once:
  // Serial port can be helpful for debugging, however...
  // ...best to write to Serial from only one task at a time...
  // ...or be sure to use semaphores to check-in and check-out access to the port.
  Serial.begin(115200);
  //Serial.println("Beginning Setup...");

  if( xMySemaphore == NULL ) {
	  xMySemaphore = xSemaphoreCreateMutex();
	  if( (xMySemaphore ) != NULL ) {
		  xSemaphoreGive( ( xMySemaphore ) );
	  }
  }
  
  xTaskCreate(
    task_Live
    , (const portCHAR *) "task_Live" // A name just for humans
    , 128 // Stack size
    , NULL
    , 1 // priority
    , NULL );
  
  xTaskCreate(
  task_Generator
    , (const portCHAR *) "task_Generator" // A name just for humans
    , 128 // Stack size
    , NULL
    , 1 // priority
    , NULL );
  
  xTaskCreate(
  task_Display
    , (const portCHAR *) "task_Display" // A name just for humans
    , 128 // Stack size
    , NULL
    , 1 // priority
    , NULL );
  //Serial.println("Finished Setup...");
}

void loop() {
  // after setup all of our code is executed in tasks.

}


// This task flashes the LED connected to pin D4 at a rate of 200ms ON then 200ms OFF repeated.
void task_Live(void *pvParameters __attribute__((unused))) 
{
  uint8_t statusLED = 4; // status LED is connected to pin D4...
  pinMode(statusLED, OUTPUT); // as an OUTPUT.
  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(statusLED, HIGH); // turn LED ON
    vTaskDelay( 200 / portTICK_PERIOD_MS ); // wait for 200ms
    digitalWrite(statusLED, LOW); // turn LED OFF
    vTaskDelay( 200 / portTICK_PERIOD_MS ); // wait for 200ms
  }
}

// This task increments a variable from 0 to 255 continuously and checks the status of the push-button switch. 
// When the normally open push-button switch is pressed, the value of the current count is sent to task "task_Display" using a messaging Queue.
void task_Generator(void *pvParameters __attribute__((unused))) 
{
  int count = 0;
  uint8_t pushButton = 5; // normally open momentary pushbutton is connected to pin D5...
  pinMode(pushButton, INPUT); // ...as an INPUT.
  for (;;) // A Task shall never return or exit.
  {
    // we are ready to check-in the semaphore to allow safe manipulation of the binCount global variable.
    if ( xSemaphoreTake( xMySemaphore, ( TickType_t ) 5 ) == pdTRUE ) {
      // buttonState will be...
      // LOW (FALSE) when button is NOT pressed, and...
      // HIGH (TRUE) when button is pressed
      int buttonState = digitalRead(pushButton);
      if(!buttonState) {
        // convert the integer counter into a binary value...
        // and save the result as a String in the global variable binCount.
        // (Difficult to overstate how helpful the next line is!)
        binCount = String(count, BIN);
        // output copy of current binary clock value to Serial Monitor just for testing purposes.
        Serial.println(binCount); 
      } else {
        // when the momentary button is pressed (held down)...
        // the binary number writing to the LED's will pause...
        // i.e. the LED output states will stop being written...
        // until the button is released again.
      }
      // ready to check-out the semaphore to allow other tasks to check-in.
  		xSemaphoreGive ( xMySemaphore );
    }
  	if(count == 255) { // when count fills 8 bits we rollover back to 0 again
  		count = 0;
  	} else {
  		count++; // increment count
  	}
  	vTaskDelay( 1 / portTICK_PERIOD_MS ); // wait for 1ms
  }
}


// This task receives the value of count and displays it on 8 x connected LED's.
void task_Display(void *pvParameters __attribute__((unused))) 
{
  // initialize LED pins for binary clock counter as OUTPUTS.
  pinMode(6, OUTPUT); // LED0 = LSB (Least Significant Bit)
  pinMode(7, OUTPUT); // LED1
  pinMode(8, OUTPUT); // LED2
  pinMode(9, OUTPUT); // LED3
  pinMode(10, OUTPUT); // LED4
  pinMode(11, OUTPUT); // LED5
  pinMode(12, OUTPUT); // LED6
  pinMode(13, OUTPUT); // LED7 = MSB (Most Significant Bit)
  for (;;) // A Task shall never return or exit.
  {
    // ready to check-in the semaphore.
	  if ( xSemaphoreTake( xMySemaphore, ( TickType_t ) 5 ) == pdTRUE ) {
      // the binary number is currently represented as a String of digits.
      // determine length of the String and thus also the number of digits.
      int binLength = binCount.length();
      // the clock counter has only 8 LED's so we only write up to 8 bits at a time. 
      for(int i=0;i<binLength;i++) {
        if(i <= 7) { 
          if(binCount[i] == '1') {
            digitalWrite(countPins[i], HIGH);
          } else {
            digitalWrite(countPins[i], LOW);
          }
        } else {
          break;  // we only write up to 8 bits at a time since the clock counter has only 8 LED's.
        }
      }
      // if the binary number that was written has less than 8 bits then...
      // ...next we pad the missing MSB digits with zeros as we write to the LED's.
      for(int i=binLength;i<8;i++) {
          digitalWrite(countPins[i], LOW);
      }
      // ready to check-out the semaphore to allow other tasks to check-in.
  		xSemaphoreGive ( xMySemaphore );
	  }
	  vTaskDelay( 10 / portTICK_PERIOD_MS ); // wait for 10ms
  }
}

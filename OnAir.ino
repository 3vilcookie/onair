/*
 * @file:      OnAir.ino
 * @brief:     Push-to-talk keyboard
 * @date:      Spring 2020
 * @author:    Raphael Pour <dev@raphaelpour.de>
 * @copyright: (C) 2020 Raphael Pour licensed under the GPLv3
 */
#include <Keyboard.h>

/* Hotkey which should be pressed*/
char key_combination[] = 
{
  KEY_LEFT_CTRL,
  KEY_LEFT_ALT
};

/*
 * Pin mapping
 */

/* Input pin connected to the pushbutton */
#define PUSH_TO_TALK_INPUT 2

/* Input pin connected to the switch */
#define SWITCH_TO_TALK_INPUT 9

/* Output pin connected to the Red LED */
#define LED_PIN 10

/*
 * Blink State
 */

bool blinkState = 0;
unsigned long nextBlink = 0;

/* 
 *  Remember if we went live or silent in order to
 *  trigger the keyboard press/release only once.
 */
int onAir = false;

/**
 * setup:
 * Prepares the Pins and starts the initializes the Keyboard.
 * After the setup, the connected PC should detect the
 * Arduino as Keyboard/HID (Human Interface Device).
 */
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUSH_TO_TALK_INPUT, INPUT);
  Keyboard.begin();
}

/**
 * goLive:
 * Activate push-to-talk once by sending the defined hotkey as
 * 'key press' to the connected PC.
 *
 * Once means, that the key press won't be sent multiple times
 * to not confuse the key-handling of the program that will
 * receive the keys at the PC.
 */
void goLive()
{
  if(!onAir)
  {
    int i;
    for(i=0; i<sizeof(key_combination);i++)
      Keyboard.press(key_combination[i]);
  }
  onAir = true;
}

/**
 * goSilent:
 * Deactivates push-to-talk by sending a release as 
 * 'key release all' to the connected PC.
 *
 * It also cares about turning off the LED.
 */
void goSilent()
{
  if(onAir) Keyboard.releaseAll();  
  digitalWrite(LED_PIN, LOW);
  onAir = false;
}

/**
 * blinkEvent:
 * Let the LED blink. This function should be called periodically
 * when push-to-talk is activated. It automatically checks when
 * it is time to turn the LED on/off.
 */
void blinkEvent()
{
  if(millis() < nextBlink) return;
  
  digitalWrite(LED_PIN, blinkState);
      
  nextBlink = millis() + 500;

  blinkState = !blinkState;
}

/**
 * loop:
 * Main loop where all Inputs will be evaluated and checked if
 * push-to-talk should be enabled/disabled.
 * It also calls the blinkEvent periodically when push-to-talk
 * is enabled.
 */
void loop() {
  
  if(digitalRead(SWITCH_TO_TALK_INPUT) == HIGH ||
     digitalRead(PUSH_TO_TALK_INPUT) == HIGH)
    goLive(); 
  else
    goSilent();

  if(onAir) blinkEvent();
}

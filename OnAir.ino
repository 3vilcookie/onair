/*
 * On Air - Voice Chat Push to talk keyboard
 */
#include <Keyboard.h>

/* Push to talk key combination which should be fired */
char key_combination[] = 
{
  KEY_LEFT_CTRL,
  KEY_LEFT_ALT
};

/*
 * Pin mapping
 */
#define PUSH_TO_TALK_INPUT 2
#define SWITCH_TO_TALK_INPUT 9
#define LED_PIN 10

bool blinkState = 0;
unsigned long nextBlink = 0;

/* 
 *  Remember if we went live or silent in order to
 *  trigger the keyboard press/release only once.
 */
int onAir = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUSH_TO_TALK_INPUT, INPUT);
  Keyboard.begin();
}


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

void goSilent()
{
  if(onAir) Keyboard.releaseAll();  
  digitalWrite(LED_PIN, LOW);
  onAir = false;
}

void blinkEvent()
{
  if(millis() < nextBlink) return;
  
  digitalWrite(LED_PIN, blinkState);
      
  nextBlink = millis() + 500;

  blinkState = !blinkState;
}
void loop() {
  
  if(digitalRead(SWITCH_TO_TALK_INPUT) == HIGH ||
     digitalRead(PUSH_TO_TALK_INPUT) == HIGH)
    goLive(); 
  else
    goSilent();

  if(onAir) blinkEvent();
}

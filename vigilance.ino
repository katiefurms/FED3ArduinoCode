#include <FED3.h>
String sketch = "Vigilance";
FED3 fed3 (sketch);

const unsigned long initialTimeout = 3000;      // 3 second timeout at the beginning of each trial
unsigned long trialStartTime;                   // time that has passed at the start of the current trial
unsigned long timeout;                          // full timeout period in which mouse shouldn't poke  
unsigned long trialPeriod = 3000;
unsigned long cuePeriod = 1000;
const int failStateTimeout = 1;
const int trialClearTimeout = 8;
int randNum;
bool leftActive = false;
bool rightActive = false;
bool inFailState = true;
unsigned long pokeWindowStart;
unsigned long trialTimeout;
unsigned long lightTimeout;

void setup() 
{
  // setup FED3 hardware
  fed3.begin();
  // seed the random number generator off noise in the A0 channel
  randomSeed(analogRead(0));
}

void loop() 
{
  fed3.run();
  
  inFailState = false;
  fed3.Right = false;
  fed3.Left = false;

  randNum = random(100);
  if (randNum % 2)
  {
    rightActive = true; // set right port to be active
    fed3.activePoke = 0;
  }
  else
  {
    leftActive = true; // set left port to be active
    fed3.activePoke = 1;
  }

  // start of trial
  fed3.Tone(4000, 200);           // play tone to indicate the start of a new trial
  trialStartTime = millis();      // get the amount of time that has passed from the beginning of the program to the start of the current trial
  timeout = trialStartTime + initialTimeout + random(0, 7000);  // calculate the total timeout period

  while (millis() < timeout)      // while in the timeout period, if the mouse pokes prematurely, play a tone and restart the loop
  {
    if (fed3.Right)
    {
      fed3.logRightPoke();
      failState();
      fed3.Right = false;
      break;
    }
    else if (fed3.Left)
    {
      fed3.logLeftPoke();
      failState();
      fed3.Left = false;
      break;
    }
  }

  if (inFailState)
  {
    return;
  }

  if (rightActive) // if right port is active
  {
    rightLight();                  

    pokeWindowStart = millis();
    trialTimeout = pokeWindowStart + trialPeriod;
    lightTimeout = pokeWindowStart + cuePeriod;

    while (millis() < trialTimeout)
    {
      while (millis() < lightTimeout)
      {
        continue;
      }
      fed3.colorWipe(fed3.strip.Color(0, 0, 0), 1);
  
      if (fed3.Right)
      {
        fed3.logRightPoke();
        fed3.Feed();
        fed3.Right = false;
        break;
      }
      else if (fed3.Left)
      {
        fed3.logLeftPoke();
        failState();
        fed3.Left = false;
        break;
      }
    }
  }
  else if (leftActive)
  {
    leftLight();

    pokeWindowStart = millis();
    trialTimeout = pokeWindowStart + trialPeriod;
    lightTimeout = pokeWindowStart + cuePeriod;

    while (millis() < trialTimeout)
    {
      while (millis() < lightTimeout)
      {
        continue;
      }
      fed3.colorWipe(fed3.strip.Color(0, 0, 0), 1);
      
      if (fed3.Left) 
      {
        fed3.logLeftPoke();
        fed3.Feed();
        fed3.Left = false;
        break;
      }
      else if (fed3.Right)
      {
        fed3.logRightPoke();
        failState();
        fed3.Right = false;
        break;
      }
    }
  }

  rightActive = false;
  leftActive = false;
  
  if (!inFailState)
  {
    fed3.Timeout(trialClearTimeout);
  }
}

bool failState()
{
  // what should occur if the mouse makes a mistake or pokes prematurely
  fed3.Tone(8000, 1000);
  fed3.Timeout(failStateTimeout);

  return inFailState = true;
}

void rightLight()
{
  fed3.rightStimulus();
  fed3.Event = "Right light";
  fed3.logdata(); 
}

void leftLight()
{
  fed3.leftStimulus();
  fed3.Event = "Left light";
  fed3.logdata(); 
}

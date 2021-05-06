/*
Programmers: BA Toth, KL Furman

Date: April 2021

Purpose: Automate a rPVT as described in Davis et al. 2016 (JOVE)
*/

#include <FED3.h>
String sketch = "Vigilance";
FED3 fed3 (sketch);

// constants definition & variable initialization
const unsigned long initialTimeout = 3000;      // 3 second timeout at the beginning of each trial (in ms)
unsigned long trialStartTime;                   // time that has passed at the start of the current trial
unsigned long timeout;                          // full timeout period in which mouse shouldn't poke  
unsigned long trialPeriod = 3000;               // duration in which mouse can poke for food reward (in ms)
unsigned long cuePeriod = 1000;                 // duration that cue (light) will be on (in ms)
const int failStateTimeout = 1;                 // timeout if trial is failed (in s)
const int trialClearTimeout = 8;                // timeout if trial is cleared (in s)
int randNum;                                    // random number generated at the start of each trial
bool leftActive = false;                        // variable to control whether light will flash left
bool rightActive = false;                       // variable to control whether light will flash right
bool inFailState = true;                        // keep track of if fail state has been entered
unsigned long pokeWindowStart;                  // start of the period in which mouse can poke
unsigned long trialTimeout;                     // time at which the poke period will end
unsigned long lightTimeout;                     // time at which light will shut off

void setup() 
{
  // setup FED3 hardware
  fed3.begin();
  // seed the random number generator off noise in the A0 channel
  randomSeed(analogRead(0));
}

void loop() 
{
  fed3.run();              // call at least once per run
  
  inFailState = false;     // make sure to set these variable to false at the beginning of each run 
  fed3.Right = false;
  fed3.Left = false;

  randNum = random(100);   // generate random number between 0 and 99
  
  if (randNum % 2)         // if number is even, make the right port active    
  {
    rightActive = true;
    fed3.activePoke = 0;
  }
  else                     // if number is odd, make the left port active
  {
    leftActive = true;
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
      failState();                // enter the fail state (see function below)
      fed3.Right = false;
      break;                      // break out of while statement and return to the main loop
    }
    else if (fed3.Left)
    {
      fed3.logLeftPoke();
      failState();
      fed3.Left = false;
      break;
    }
  }

  if (inFailState)                // if in the fail state at this stage, return to the start of the loop and restart the trial
  {
    return;
  }

  if (rightActive)                // if right port is active
  {
    rightLight();                 // function to control right light turning on (see below)

    pokeWindowStart = millis();   // find the time at which the poke window will begin
    trialTimeout = pokeWindowStart + trialPeriod;  // calculate the time at which the poke window will end
    lightTimeout = pokeWindowStart + cuePeriod;    // calculate the time at which the light will turn off

    while (millis() < trialTimeout)
    {
      while (millis() < lightTimeout)
      {
        continue;
      }
      fed3.colorWipe(fed3.strip.Color(0, 0, 0), 1);  // at the end of lightTimeout, turn the LED off
  
      if (fed3.Right)             // log right poke info
      {
        fed3.logRightPoke();
        fed3.Feed();
        fed3.Right = false;
        break;
      }
      else if (fed3.Left)         // log left poke info
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

  rightActive = false;  // set neither port to active
  leftActive = false;
  
  if (!inFailState)     // if not in the fail state at this point, trial has been cleared
  {                     // can occur due to either a correct poke or due to no poke
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
  // control the event of the right light turning on
  fed3.rightStimulus();
  fed3.Event = "Right light";
  fed3.logdata(); 
}

void leftLight()
{
  // control the event of the left light turning on
  fed3.leftStimulus();
  fed3.Event = "Left light";
  fed3.logdata(); 
}

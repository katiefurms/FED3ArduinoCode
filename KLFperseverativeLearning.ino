#include <FED3.h>
String sketch = "perseverativeLearning"; // name the sketch
const int MAX_NUM_POKES = 20; // this is the number of pokes in one block, before probabilities switch ports
const int PROBABILITY = 80; // this is the probability cutoff. PROBABILITY = 80 means the probabilities will be 80% or 20% for a given port
bool isInitialCondition = true; //   which swtiches at the end of a block MAX_NUM_POKES long, allowing us to switch probabilities
bool rightPoke;
bool leftPoke;

FED3 fed3 (sketch);

void setup() {
  // put your setup code here, to run once:
  fed3.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  fed3.run();
// if we've not yet reached the threshold MAX_NUM_POKES, stay in the initial probability condition. If we have, switch conditions.
  if (isInitialCondition == true)
  {
    initialCondition();
  }
  else
  {
    switchCondition();
  }
}

bool getProbability() // a function to get a random number, and decide whether we are above or below the threshold defined by PROBABILITY
{
  int randNum;
  bool isMoreLikely;

  randNum = random(0,99);

  if (randNum >= PROBABILITY)
  {
    isMoreLikely = false;
  }
  else
  {
    isMoreLikely = true;
  }
  return isMoreLikely;
}

void yesFoodLeft() // a function containing actions to take if the poke is meant to trigger a pellet drop
{
  fed3.logLeftPoke();
  fed3.colorWipe(fed3.strip.Color(0, 2, 2), 40); //blue colorwipe is CS+
  fed3.Feed(); // drop a pellet
    leftPoke = false;
}

void yesFoodRight() // a function containing actions to take if the poke is meant to trigger a pellet drop
{
  fed3.logRightPoke();
  fed3.colorWipe(fed3.strip.Color(0, 2, 2), 40); //blue colorwipe is CS+
  fed3.Feed(); // drop a pellet
    rightPoke = false;
}

void noFoodLeft() // a function containing actions to take if the poke is meant to NOT trigger a pellet drop
{
  fed3.logLeftPoke();
  fed3.Tone(4000,200); // 4000Hz tone is CS-
    leftPoke = false;
}

void noFoodRight() // a function containing actions to take if the poke is meant to NOT trigger a pellet drop
{
  fed3.logRightPoke();
  fed3.Tone(4000,200); // 4000Hz tone is CS-
    rightPoke = false;
}

void initialCondition() // define the probability of each port, and what is to execute from them, during the initial condition
{
    rightPoke = false;
    leftPoke = false;
  
  fed3.colorWipe(fed3.strip.Color(100, 0, 0), 40); // this will be taken out in the final code, but is here now to trigger a red colorwipe when conditions switch so that we can troubleshoot.
  for (int i = 0; i < MAX_NUM_POKES; i++) // only run this loop until we reach MAX_NUM_POKES, at which point it should switch
  {  
    while (!rightPoke && !leftPoke) // keep checking for a left or right poke as long as the mouse isn't poking
    {
      if (fed3.Right)
      {
        rightPoke = true;
      }
      else if (fed3.Left)
      {
        leftPoke = true;
      }
      else
      {
        fed3.colorWipe(fed3.strip.Color(0, 0, 0), 0); // this line does nothing, but we found it was necessary to include an "else" condition to keep the while loop running. 
      }
    }
    if (leftPoke == true) // code to execute if a leftpoke is detected
    {      
      if (!getProbability()) // After a left poke, this is the lower probability (20%, if PROBABILITY = 80) sequence of events
      {
        noFoodLeft();
      }
      else // After a left poke, this is the higher probability (80%, if PROBABILITY = 80) sequence of events
      {
        yesFoodLeft();
      }
    }
    else if (rightPoke == true) // code to execute if a leftpoke is detected
    {      
      if (getProbability()) // After a right poke, this is the higher probability (80%, if PROBABILITY = 80) sequence of events
      {
        noFoodRight();
      }
      else // After a right poke, this is the lower probability (20%, if PROBABILITY = 80) sequence of events
      {
        yesFoodRight();
      } 
    }
  }
  isInitialCondition = false; // switch the initial condition back to false, so that the main loop knows to switch to the other probability condition after MAX_NUM_POKES number of pokes
}

void switchCondition() // define the probability of each port, and what is to execute from them, during the switch condition
{
    rightPoke = false;
    leftPoke = false;
  fed3.colorWipe(fed3.strip.Color(100, 0, 0), 40); // this will be taken out in the final code, but is here now to trigger a red colorwipe when conditions switch so that we can troubleshoot.
  for (int i = 0; i < MAX_NUM_POKES; i++) // only run this loop until we reach MAX_NUM_POKES, at which point it should switch
  {
    while (!rightPoke && !leftPoke) // keep checking for a left or right poke as long as the mouse isn't poking
    {
      if (fed3.Right)
      {
        rightPoke = true;
      }
      else if (fed3.Left)
      {
        leftPoke = true;
      }
      else
      {
        fed3.colorWipe(fed3.strip.Color(0, 0, 0), 0); // this line does nothing, but we found it was necessary to include an "else" condition to keep the while loop running. 
      }
    }
    if (rightPoke == true) // code to execute if a rightpoke is detected
    {      
      if (!getProbability()) // After a right poke, this is the lower probability (20%, if PROBABILITY = 80) sequence of events
      {
        noFoodRight(); 
      }
      else // After a right poke, this is the higher probability (80%, if PROBABILITY = 80) sequence of events
      {
        yesFoodRight(); // initiate a blue colorwipe (CS+) and drop a food pellet
      }
    }
    else if (leftPoke == true) // code to execute if a rightpoke is detected
    {      
      if (getProbability()) // After a left poke, this is the higher probability (80%, if PROBABILITY = 80) sequence of events
      {
        noFoodLeft(); 
      }
      else // After a left poke, this is the lower probability (20%, if PROBABILITY = 80) sequence of events
      {
        yesFoodLeft(); // initiate a blue colorwipe (CS+) and drop a food pellet
      }
    }
  }
  isInitialCondition = true; // switch the initial condition back to truee, so that the main loop knows to switch back to the other probability condition after MAX_NUM_POKES number of pokes

}

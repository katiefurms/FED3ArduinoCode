#include <FED3.h>
String sketch = "perseverativeLearning";

const int block_size = 20
const int low_cutoff = 2
const int high_cutoff = 8
const int num_blocks = 6
int poke_num = 0 
int block_num = 0 

FED3 fed3 (sketch);

void setup() {
  // put your setup code here, to run once:
  fed3.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  fed3.run();

  while (block_num > num_blocks) //while we have less blocks than total block number,
  {
    if (block_num % 2 == 0) //check if its an even block. proceed if so
    {
      while (poke_num < block_size) //check if the block should end yet
      {
        if (fed3.Left){ //executes for left port as high probability port
          fed3.logLeftPoke();
          randnum = random(1,10); //pick a random number
          poke_num++;//increase poke_num
          if (randnum <= high_cutoff){ // high% of the time, food is delivered
            fed3.Feed();
            fed3.colorWipe(fed3.strip.Color(0, 0, 0), 10);
          }
          else{ //low% of the time, food is not delivered
            // no food deliver
            fed3.Tone(4000,200);
          }
        }
        else if (fed3.Right){ //executes for right port as low probability port
          fed3.logRightPoke();
          randnum = random(1,10); //pick a random number
          poke_num++; //increase poke_num
          if (randnum < low_cutoff){ //low% of the time, food is delivered
            fed3.Feed();
            fed3.colorWipe(fed3.strip.Color(0, 0, 0), 10);
            fed3.Blink(BNC_OUT, 200, randNum); //REPLACE THIS WITH OPTO BLINK
          }
          else{ //high% of the time, food is not delivered
            //no food deliver
            fed3.Tone(4000,200);
            
          }
        }
        
      }
    }
    else if (block_num %2 != 0) //check if its an odd block. proceed if so
    {
      while (poke_num < block_size) //check if the block should end yet
      {
        if (fed3.Right){ //executes for right port as high probability port
          fed3.logRightPoke();
          randnum = random(1,10); //pick a random number
          poke_num++; //increase poke_num
          if (randnum <= high_cutoff){ // high% of the time, food is delivered
            fed3.Feed();
            fed3.colorWipe(fed3.strip.Color(0, 0, 0), 10);
          }
          else{ //low% of the time, food is not delivered
            // no food deliver
            fed3.Tone(4000,200);
          }
        }
        else if (fed3.Left){ //executes for left port as low probability port
          fed3.logLeftPoke();
          randnum = random(1,10); //pick a random number
          poke_num++; //increase poke_num
          if (randnum < low_cutoff){ //low% of the time, food is delivered
            fed3.Feed();
            fed3.colorWipe(fed3.strip.Color(0, 0, 0), 10);
            fed3.Blink(BNC_OUT, 200, randNum); //REPLACE THIS WITH OPTO BLINK
          }
          else{ //high% of the time, food is not delivered
            //no food deliver
            fed3.Tone(4000,200);
            
          }
        }
        
      }
    }
    block_num++;
    poke_num = 0;
  }
  
}

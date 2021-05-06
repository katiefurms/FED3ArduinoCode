# FED3ArduinoCode
A collection of various Arduino sketches related to the Burgess Lab FED3 setup. Currently including 

[Perseverative Learning Paradigm](https://github.com/katiefurms/FED3ArduinoCode/blob/main/persLearningKLF)
 - Allows the two FED ports to be programmed with different probabilities of releasing a pellet, with a CS+ (blue LED colorwipe) paired with food release, and a CS- (tone) paired with food not being released. These probabilities can also be programmed to flip after N n umber of pokes, allowing for observation of perseverative errors after learning.


[Psychomotor Vigilance Task](https://github.com/katiefurms/FED3ArduinoCode/tree/main/vigilance)
- Either the left or right port on the FED is cued as active with a light underneath it, indicating that a pellet will be available from that port. The mouse must poke at the cued port to receive a pellet. If it pokes in the incorrect port, or fails to poke at all, an error tone will be triggered and a pellet will not be dropped.
- reference: https://www.ncbi.nlm.nih.gov/pmc/articles/PMC2605519/pdf/nihms77294.pdf
- Requires a different, altered FED3.h and FED3.cpp from the normal FED3 library.

#include "MovingAvg.cpp"

#define SAMPLE_GAIN 0.0034285714 //considers a 100k ohm potentiometer, with 23k ohm between SIG and GND
#define BATERY_LOW 6.0 //considers the batery minimum voltage to be 6.0V

//reads the voltage divider and calculates the batery voltage
//if the voltage is lower than BATERY_LOW turns on the buzzer
//returns the batery voltage
float checkBatery(unsigned int bat_pin, unsigned int buzzer_pin){
  static MovingAvg avg(10);
  static first_call = true;

  if(first_call){
    pinMode(buzzer_pin, OUTPUT);
    first_call = false;
  }

  int bat_level = avg.filter(analogRead(bat_pin));
  float bat_voltage = bat_level * SAMPLE_GAIN;
  digitalWrite(buzzer_pin,bat_voltage < BATERY_LOW);
  return bat_voltage;
}

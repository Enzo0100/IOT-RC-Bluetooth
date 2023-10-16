//requests a distance to the HC_SR04 sensor
//if the request is answered returns the current distance, else returns -1 
float HC_SR04(unsigned int trig, unsigned int echo){

  static unsigned int timestamp = 0;
  static unsigned int state = 0;
  static MovingAvg avg(10);
  static bool first_call = true;

  if(first_call){
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    first_call = false;
  }

  switch(state){
  
    case 0:
      digitalWrite(trig,1);
      timestamp = micros();
      state++;
    break;

    case 1:
      if(micros() > timestamp + 10){
        digitalWrite(trig,0);
        state++;
      }
    break;

    case 2:
      float distance = pulseIn(echo,1) / 58;
      state = 0;
      return avg.filter(distance);
    break;
  }
  
  return -1;
}

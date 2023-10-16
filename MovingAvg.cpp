#include <stdlib.h>

#ifndef MVAVG_CPP
#define MVAVG_CPP

//creates a object cable of calculating the moving avg of a datastream
//receives the desired filter order as a parameter

class MovingAvg{
  
  private:
    
    unsigned int order;
    unsigned int index = 0;
    float * samples;
    float avg = 0;
  
  public:

    MovingAvg(unsigned int ord){
      order = ord;
      samples = (float*)malloc(sizeof(float) * order);
      for(int i = 0; i < order; i++)
        samples[i] = 0;
    }

    ~MovingAvg(){
      free(samples);
    }

    float filter(float sample){
      sample /= order;
      avg += sample - samples[index];
      samples[index] = sample;
      index = (index + 1) % order;
      return avg;
    }

    float filter(int sample){
      float aux = sample;
      return filter(aux);
    }

    float getAvg(){
      return avg;
    }
};

#endif
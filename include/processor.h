#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

  //necessary private members
 private:
 long prevIdle{0};
 long prevActive{0};
};

#endif
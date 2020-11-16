#ifndef _Stepping_H__
#define _Stepping_H__

#include "Arduino.h"

class BJ
{
  private:


  public:
    static void pulse();
    static void Lifting(int path,int num);
    static void initial();
    static void Position(int path);
    static int BJ::getStepping();
};

#endif

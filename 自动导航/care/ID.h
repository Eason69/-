#ifndef _ID_H__
#define _ID_H__

#include "Arduino.h"

class ID
{
  private:


  public:
    static int IDcard();
    static bool isCard(char cardTemp[10], char card[10]);

};

#endif

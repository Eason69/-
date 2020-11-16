#ifndef _drive_H__
#define _drive_H__

#include "Stepping.h"
#include "Arduino.h"
#include <Wire.h>

class QD
{
  private:

  public:
    static void Rotating(int number,int button);//旋转
    static void tracing(int number);//导向
    static void SetCmd(unsigned char cmdSend, long data, int a);//I2C数据发送
    static void Stop();//停止
    static void translations(int number, int inquiries);//平移
};

#endif

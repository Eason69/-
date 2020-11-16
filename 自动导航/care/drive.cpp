#include "drive.h"
#include "Stepping.h"
#include "Arduino.h"
#include <Wire.h>

int sd = 20;//速度
int data1[8], data2[8];//磁导航数据

void QD::Rotating(int number, int button) //旋转
{
  if (number == 1) {
    SetCmd('E', 500, 1);
    SetCmd('e', 500, 1);
    SetCmd('E', -500, 2);
    SetCmd('e', -500, 2);
    delay(250);
  }
  if (number == 2) {
    SetCmd('E', 1020, 1);
    SetCmd('e', 1020, 1);
    SetCmd('E', -1020, 2);
    SetCmd('e', -1020, 2);
    delay(520);
  }
  if (button == 1) {//右
    SetCmd('E', 2800, 1);
    SetCmd('e', -2800, 1);
    SetCmd('E', 2800, 2);
    SetCmd('e', -2800, 2);
    delay(1400);
  }
  if (button == 2) {//左
    SetCmd('E', -2800, 1);
    SetCmd('e', 2800, 1);
    SetCmd('E', -2800, 2);
    SetCmd('e', 2800, 2);
    delay(1400);
  }
  if (button == 3) {//180
    SetCmd('E', -5600, 1);
    SetCmd('e', 5600, 1);
    SetCmd('E', -5600, 2);
    SetCmd('e', 5600, 2);
    delay(2800);
  }
}

void QD::translations(int number, int inquiries) //平移
{
  int a = 1720;
  int b = 1800;
  QD::Stop();
  delay(100);
  if (number == 1) { //第一列
    if (inquiries == 1) { //进
      SetCmd('E', -a-100, 1);
      SetCmd('e', -a, 2);
      SetCmd('E', a, 2);
      SetCmd('e', a+100, 1);
      delay(1200);
      SetCmd('E', b, 1);
      SetCmd('e', b, 1);
      SetCmd('E', -b, 2);
      SetCmd('e', -b, 2);
      delay(1200);
    }
    if (inquiries == 2) { //出
      SetCmd('E', -b, 1);
      SetCmd('e', -b, 1);
      SetCmd('E', b, 2);
      SetCmd('e', b, 2);
      delay(1200);
      SetCmd('E', a+100, 1);
      SetCmd('e', a, 2);
      SetCmd('E', -a, 2);
      SetCmd('e', -a-100, 1);
      delay(1200);
    }
  }
  if (number == 2) { //第二列
    if (inquiries == 1) { //进
      SetCmd('E', b, 1);
      SetCmd('e', b, 1);
      SetCmd('E', -b, 2);
      SetCmd('e', -b, 2);
      delay(1200);
    }
    if (inquiries == 2) { //出
      SetCmd('E', -b, 1);
      SetCmd('e', -b, 1);
      SetCmd('E', b, 2);
      SetCmd('e', b, 2);
      delay(1200);
    }
  }
  if (number == 3) { //第三列
    if (inquiries == 1) { //进
      SetCmd('E', a, 1);
      SetCmd('e', a, 2);
      SetCmd('E', -a, 2);
      SetCmd('e', -a, 1); 
      delay(1200);
      SetCmd('E', b, 1);
      SetCmd('e', b, 1);
      SetCmd('E', -b, 2);
      SetCmd('e', -b, 2);
      delay(1200);
    }
    if (inquiries == 2) { //出
      SetCmd('E', -b, 1);
      SetCmd('e', -b, 1);
      SetCmd('E', b, 2);
      SetCmd('e', b, 2);
      delay(1200);
      SetCmd('E', -a, 1);
      SetCmd('e', -a, 2);
      SetCmd('E', a, 2);
      SetCmd('e', a, 1);  
      delay(1200);
    }
  }
  if (number == 4) {
    if (inquiries == 0) { //退回原点
      SetCmd('E', -1900, 1);
      SetCmd('e', -1900, 1);
      SetCmd('E', 1900, 2);
      SetCmd('e', 1900, 2);
      delay(1500);
    }
  }

}

void QD::tracing(int number)//循迹
{
  int a, b, c, d, e, a_a, b_b, c_c;

  data1[0] = !digitalRead(22);
  data1[1] = !digitalRead(23);
  data1[2] = !digitalRead(24);
  data1[3] = !digitalRead(25);
  data1[4] = !digitalRead(26);
  data1[5] = !digitalRead(27);
  data1[6] = !digitalRead(28);
  data1[7] = !digitalRead(29);

  data2[0] = !digitalRead(30);
  data2[1] = !digitalRead(31);
  data2[2] = !digitalRead(32);
  data2[3] = !digitalRead(33);
  data2[4] = !digitalRead(34);
  data2[5] = !digitalRead(35);
  data2[6] = !digitalRead(36);
  data2[7] = !digitalRead(37);

  a = data1[7] * 8 + data1[6] * 6 + data1[5] * 4 + data1[4] * 2 - data1[3] * 2 - data1[2] * 4 - data1[1] * 6 - data1[0] * 8;
  b = data2[0] * 8 + data2[1] * 6 + data2[2] * 4 + data2[3] * 2 - data2[4] * 2 - data2[5] * 4 - data2[6] * 6 - data2[7] * 8;
  a_a = data1[7] + data1[6] + data1[5] + data1[4] + data1[3] + data1[2] + data1[1] + data1[0];
  b_b = data2[0] + data2[1] + data2[2] + data2[3] + data2[4] + data2[5] + data2[6] + data2[7];
  c = data1[0] && data1[1] && data1[2] && data1[3] && data1[4] && data1[5] && data1[6] && data1[7];
  c_c = data2[0] && data2[1] && data2[2] && data2[3] && data2[4] && data2[5] && data2[6] && data2[7];
  d = 1;
  //Serial.println(a);
  //Serial.println(b);
  e = (abs(a) + abs(b))/1.2;
  if (number == 1) {
    if (a <= 0 && b >= 0 && e > d && a_a <= 4 && b_b <= 4) //右转变直
    {
      SetCmd('M', sd + e, 1);
      SetCmd('m', sd - e, 1);
      SetCmd('M', -(sd - e), 2);
      SetCmd('m', -(sd + e), 2);
    }

    if (a >= 0 && b <= 0 && e > d && a_a <= 4 && b_b <= 4) //左转变直
    {
      SetCmd('M', sd - e, 1);
      SetCmd('m', sd + e, 1);
      SetCmd('M', -(sd + e), 2);
      SetCmd('m', -(sd - e), 2);
    }

    if (a <= 0 && b <= 0 && e > d && a_a <= 4 && b_b <= 4) //右平移居中
    {
      SetCmd('M', sd + e, 1);
      SetCmd('m', sd - e, 1);
      SetCmd('M', -(sd + e), 2);
      SetCmd('m', -(sd - e), 2);
    }

    if (a >= 0 && b >= 0 && e > d && a_a <= 4 && b_b <= 4) //左平移居中
    {
      SetCmd('M', sd - e, 1);
      SetCmd('m', sd + e, 1);
      SetCmd('M', -(sd - e), 2);
      SetCmd('m', -(sd + e), 2);
    }

    if ( (!c && e < d) || a_a > 4 || b_b > 4)//冲
    {
      SetCmd('M', sd, 1);
      SetCmd('m', sd, 1);
      SetCmd('M', -sd, 2);
      SetCmd('m', -sd, 2);
    }
  }
  if (number == 0) {
    if (a <= 0 && b >= 0 && e > d && a_a <= 4 && b_b <= 4) //右转变直
    {
      SetCmd('M', -(sd - e), 1);
      SetCmd('m', -(sd + e), 1);
      SetCmd('M', sd + e, 2);
      SetCmd('m', sd - e, 2);
    }

    if (a >= 0 && b <= 0 && e > d && a_a <= 4 && b_b <= 4) //左转变直
    {
      SetCmd('M', -(sd + e), 1);
      SetCmd('m', -(sd - e), 1);
      SetCmd('M', sd - e, 2);
      SetCmd('m', sd + e, 2);
    }

    if (a <= 0 && b <= 0 && e > d && a_a <= 4 && b_b <= 4) //右平移居中
    {
      SetCmd('M', -(sd - e), 1);
      SetCmd('m', -(sd + e), 1);
      SetCmd('M', sd - e, 2);
      SetCmd('m', sd + e, 2);
    }

    if (a >= 0 && b >= 0 && e > d && a_a <= 4 && b_b <= 4) //左平移居中
    {
      SetCmd('M', -(sd + e), 1);
      SetCmd('m', -(sd - e), 1);
      SetCmd('M', sd + e, 2);
      SetCmd('m', sd - e, 2);
    }

    if ( (!c && e < d) || a_a > 4 || b_b > 4)//冲
    {
      SetCmd('M', -sd, 1);
      SetCmd('m', -sd, 1);
      SetCmd('M', sd, 2);
      SetCmd('m', sd, 2);
    }
  }

}
void QD::Stop()
{
  SetCmd('M', 0, 1);
  SetCmd('m', 0, 1);
  SetCmd('M', 0, 2);
  SetCmd('m', 0, 2);
}

void QD::SetCmd(unsigned char cmdSend, long data, int a)//i2c发送
{
  unsigned char cmd[5];
  // command
  cmd[0] = cmdSend;

  // data
  // send MSB first
  cmd[1] = (unsigned char)(data >> 24);
  cmd[2] = (unsigned char)(data >> 16);
  cmd[3] = (unsigned char)(data >> 8);
  cmd[4] = (unsigned char)(data);

  // send command
  if (a == 1)
    Wire.beginTransmission(0x50);

  else if (a == 2)
    Wire.beginTransmission(0x60);

  Wire.write(cmd, 5);
  Wire.endTransmission();
  return;
}

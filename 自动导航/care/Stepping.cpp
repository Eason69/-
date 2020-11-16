#include "Stepping.h"
#include "Arduino.h"

int lower_1 = 3300;//位置1
int lower_2 = 2300;
int lower_3 = 15300;//位置2
int lower_4 = 14300;
int lower_5 = 14600;//位置3
int lower_6 = 13600;


bool lighting = true;//生成脉冲
byte stepping = 0;//启动脉冲
byte Software = 0;//步进电机运行方向，1上升，0下降
int step_i = 0;//脉冲记数
int lower = 0;//下限位脉冲个数
int number = 0;//目标脉冲数

void BJ::pulse()//生成脉冲
{
  if (stepping == 1) {
    digitalWrite(3, lighting);
    lighting = !lighting;
    step_i++;
    if (Software == 1) lower++;
    if (Software == 0) lower--;
  }
  //Serial.println(stepping_i);
  if (number == step_i) {
    stepping = 0;
    step_i = 0;
    number = 0;
  }
  if (lower == 17000) {
    stepping = 0;
    step_i = 0;
    number = 0;
    Lifting(1, 500);
  }

  if (digitalRead(4) == LOW) {
    stepping = 0;
    step_i = 0;
    number = 0;
    lower = 0;
    Lifting(0, 500);
  }
}

int BJ::getStepping(){
  Serial.println(stepping);
  return stepping;
}


void BJ::initial()//步进初始化
{
  Lifting(1, 20000);
  while (1) {
    pulse();
    delay(1);
    if (digitalRead(4) == LOW) {
      break;
    }
  }
}

void BJ::Position(int path)//位置控制
{
  if (path == 1) {
    if (lower_1 < lower) Lifting(1, lower - lower_1);
    if (lower_1 > lower) Lifting(0, lower_1 - lower);
  }
  if (path == 2) {
    if (lower_2 < lower) Lifting(1, lower - lower_2);
    if (lower_2 > lower) Lifting(0, lower_2 - lower);
  }
  if (path == 3) {
    if (lower_3 < lower) Lifting(1, lower - lower_3);
    if (lower_3 > lower) Lifting(0, lower_3 - lower);
  }
  if (path == 4) {
    if (lower_4 < lower) Lifting(1, lower - lower_4);
    if (lower_4 > lower) Lifting(0, lower_4 - lower);
  }
  if (path == 5) {
    if (lower_5 < lower) Lifting(1, lower - lower_5);
    if (lower_5 > lower) Lifting(0, lower_5 - lower);
  }
  if (path == 6) {
    if (lower_6 < lower) Lifting(1, lower - lower_6);
    if (lower_6 > lower) Lifting(0, lower_6 - lower);
  }
}

void BJ::Lifting(int path, int num) //脉冲控制
{
  if (path == 1) {
    digitalWrite(2, HIGH);
    Software = 0;
  }
  if (path == 0) {
    digitalWrite(2, LOW);
    Software = 1;
  }
  stepping = 1;
  number = num;
}

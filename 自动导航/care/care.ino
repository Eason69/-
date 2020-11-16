#include "drive.h"
#include "Stepping.h"
#include "ID.h"
#include <MsTimer2.h>
#include <Wire.h>

byte pattern = 0;//1为取货，2为送货
byte Shelves = 0;//4个货架号
byte column = 0;//列
byte line = 0;//行
byte line_front = 0;//之前的行数
byte IDnumber =  99;//之前的ID卡号
byte Trace = 0;//循迹标志位
byte steps = 0;//步骤
byte Continue = 0;//步进未到位停止
byte IDcurrent = 0;

float val = 0;//电压
float val_numer = 0;//之前电压
int val_i = 1000;//电压读取时间

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial3.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT);
  BJ::initial();
  MsTimer2::set(1.2, Tims);
  MsTimer2::start();
  delay(1000);
}

void loop() {
  Esp8266();
  navigation();
}

void navigation()//导向
{
  IDcurrent = ID::IDcard();//当前的ID卡号
  //Serial.println(Trace);
  if (Trace == 1) { //前进
    QD::tracing(1);
  }
  if (Trace == 2) { //后退
    QD::tracing(0);
  }
  if (Trace == 0) { //停止
    QD::Stop();
  }
  //取货———————————————————————————————————————————————
  if (pattern == 1) {
    if (Shelves == 1) {//1号货架
      if (IDcurrent == 8 && IDcurrent != IDnumber && steps == 0)  QD::Rotating(1, 2);
      if ((IDcurrent == 1 && IDcurrent != IDnumber  && steps == 0) || Continue == 1) {
        steps = 1;
        //Serial.println(BJ::getStepping());
        if (BJ::getStepping() == 1) {
          QD::Stop();
          Continue = 1;
        }
        if (BJ::getStepping() == 0) {
          if (column == 1) {//第一列
            picked(1);
          }
          if (column == 2) {//第二列
            picked(2);
          }
          if (column == 3) {//第三列
            picked(3);
          }
          Continue = 0;
        }
      }
      if (IDcurrent == 8 && IDcurrent != IDnumber && steps == 2) {
        BJ::Position(6);
        QD::Rotating(2, 2);
        steps = 3;
      }
    }
    if (Shelves == 2) {//2号货架
      if (IDcurrent == 10 && IDcurrent != IDnumber && steps == 0)  QD::Rotating(1, 2);
      if ((IDcurrent == 2 && IDcurrent != IDnumber  && steps == 0) || Continue == 1) {
        steps = 1;
        if (BJ::getStepping() == 1) {
          QD::Stop();
          Continue = 1;
        }
        if (BJ::getStepping() == 0) {
          if (column == 1) {//第一列
            picked(1);
          }
          if (column == 2) {//第二列
            picked(2);
          }
          if (column == 3) {//第三列
            picked(3);
          }
          Continue = 0;
        }
      }
      if (IDcurrent == 10 && IDcurrent != IDnumber && steps == 2) {
        BJ::Position(6);
        QD::Rotating(2, 2);
        steps = 3;
      }
    }
    if (Shelves == 3) {//3号货架
      if ((IDcurrent == 3 && IDcurrent != IDnumber  && steps == 0) || Continue == 1) {
        steps = 1;
        if (BJ::getStepping() == 1) {
          QD::Stop();
          Continue = 1;
        }
        if (BJ::getStepping() == 0) {
          if (column == 1) {//第一列
            picked(1);
          }
          if (column == 2) {//第二列
            picked(2);
          }
          if (column == 3) {//第三列
            picked(3);
          }
          Continue = 0;
        }
      }
      if (IDcurrent == 10 && IDcurrent != IDnumber && steps == 2) {
        BJ::Position(6);
        QD::Rotating(2, 3);
        steps = 3;
      }
    }
    if (Shelves == 4) {//4号货架
      if (IDcurrent == 10 && IDcurrent != IDnumber && steps == 0)  QD::Rotating(1, 1);
      if ((IDcurrent == 4 && IDcurrent != IDnumber  && steps == 0) || Continue == 1) {
        steps = 1;
        if (BJ::getStepping() == 1) {
          QD::Stop();
          Continue = 1;
        }
        if (BJ::getStepping() == 0) {
          if (column == 1) {//第一列
            picked(1);
          }
          if (column == 2) {//第二列
            picked(2);
          }
          if (column == 3) {//第三列
            picked(3);
          }
          Continue = 0;
        }
      }
      if (IDcurrent == 10 && IDcurrent != IDnumber && steps == 2) {
        BJ::Position(6);
        QD::Rotating(2, 1);
        steps = 3;
      }
    }

    //卸货区~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (IDcurrent == 7 && IDcurrent != IDnumber && steps == 3) {
      QD::Rotating(1, 2);
    }
    if ((IDcurrent == 6 && IDcurrent != IDnumber && steps == 3) || Continue == 2) {
      steps = 4;
      if (BJ::getStepping() == 1) {
        QD::Stop();
        Continue = 2;
      }
      if (BJ::getStepping() == 0) {
        QD::translations(2, 1);
        BJ::Position(5);
        delay(800);
        QD::translations(2, 2);
        steps = 5;
        Continue = 0;
      }
    }
    if (IDcurrent == 7 && IDcurrent != IDnumber && steps == 5) {
      QD::Rotating(2, 2);
      QD::translations(4, 0);
      steps = 6;
    }
    //根据ID位置控制电机前进后退
    if (steps == 0) Trace = 1;
    if (steps == 1) Trace = 0;
    if (steps == 2) Trace = 2;
    if (steps == 3) Trace = 1;
    if (steps == 4) Trace = 0;
    if (steps == 5) Trace = 2;
    if (steps == 6 && BJ::getStepping()) Trace = 0;
    if (steps == 6 && !BJ::getStepping()) {
      pattern = 0; Shelves = 0; column = 0; line = 0;
      line_front = 0; IDnumber =  99; Trace = 0; steps = 0;
      Serial3.print("state1");
    }

    //步进提前控制
    if (line == 1 && line != line_front) BJ::Position(1);
    if (line == 2 && line != line_front) BJ::Position(3);

  }
  //送货———————————————————————————————————————————————
  if (pattern == 2) {
    if (IDcurrent == 7 && IDcurrent != IDnumber && steps == 0) {
      BJ::Position(5);
      QD::Rotating(1, 1);
    }
    if ((IDcurrent == 6 && IDcurrent != IDnumber && steps == 0) || Continue == 2) {
      steps = 1;
      if (BJ::getStepping() == 1) {
        QD::Stop();
        Continue = 2;
      }
      if (BJ::getStepping() == 0) {
        QD::translations(2, 1);
        BJ::Position(6);
        delay(800);
        QD::translations(2, 2);
        steps = 2;
        Continue = 0;
        //步进提前控制
        if (line == 1 ) BJ::Position(2);
        if (line == 2 ) BJ::Position(4);
      }
    }
    if (IDcurrent == 7 && IDcurrent != IDnumber && steps == 2) {
      QD::Rotating(2, 2);
      steps = 3;
    }

    //卸货区~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (Shelves == 1) {//1号货架
      if (IDcurrent == 8 && IDcurrent != IDnumber && steps == 3)  QD::Rotating(1, 2);
      if ((IDcurrent == 1 && IDcurrent != IDnumber && steps == 3) ||  Continue == 1) {
        steps = 4;
        if (BJ::getStepping() == 1) {
          QD::Stop();
          Continue = 1;
        }
        if (BJ::getStepping() == 0) {
          if (column == 1) {//第一列
            delivery(1);
          }
          if (column == 2) {//第二列
            delivery(2);
          }
          if (column == 3) {//第三列
            delivery(3);
          }
          Continue = 0;
        }
      }
      if (IDcurrent == 8 && IDcurrent != IDnumber && steps == 5) {
        BJ::Position(6);
        QD::Rotating(2, 1);
      }
    }
    if (Shelves == 2) {//2号货架
      if (IDcurrent == 10 && IDcurrent != IDnumber && steps == 3)  QD::Rotating(1, 2);
      if ((IDcurrent == 2 && IDcurrent != IDnumber  && steps == 3) || Continue == 1) {
        steps = 4;
        if (BJ::getStepping() == 1) {
          QD::Stop();
          Continue = 1;
        }
        if (BJ::getStepping() == 0) {
          if (column == 1) {//第一列
            delivery(1);
          }
          if (column == 2) {//第二列
            delivery(2);
          }
          if (column == 3) {//第三列
            delivery(3);
          }
          Continue = 0;
        }
      }
      if (IDcurrent == 10 && IDcurrent != IDnumber && steps == 5) {
        BJ::Position(6);
        QD::Rotating(2, 1);
      }
    }
    if (Shelves == 3) {//3号货架
      if ((IDcurrent == 3 && IDcurrent != IDnumber  && steps == 3) || Continue == 1) {
        steps = 4;
        if (BJ::getStepping() == 1) {
          QD::Stop();
          Continue = 1;
        }
        if (BJ::getStepping() == 0) {
          if (column == 1) {//第一列
            delivery(1);
          }
          if (column == 2) {//第二列
            delivery(2);
          }
          if (column == 3) {//第三列
            delivery(3);
          }
          Continue = 0;
        }
      }
      if (IDcurrent == 10 && IDcurrent != IDnumber && steps == 5) {
        BJ::Position(6);
      }
    }
    if (Shelves == 4) {//4号货架
      if (IDcurrent == 10 && IDcurrent != IDnumber && steps == 3)  QD::Rotating(1, 1);
      if ((IDcurrent == 4 && IDcurrent != IDnumber  && steps == 3) || Continue == 1) {
        steps = 4;
        if (BJ::getStepping() == 1) {
          QD::Stop();
          Continue = 1;
        }
        if (BJ::getStepping() == 0) {
          if (column == 1) {//第一列
            delivery(1);
          }
          if (column == 2) {//第二列
            delivery(2);
          }
          if (column == 3) {//第三列
            delivery(3);
          }
          Continue = 0;
        }
      }
      if (IDcurrent == 10 && IDcurrent != IDnumber && steps == 5) {
        BJ::Position(6);
        QD::Rotating(2, 2);
      }
    }

    if (IDcurrent == 7 && IDcurrent != IDnumber && steps == 5) {
      QD::translations(4, 0);
      steps = 6;
    }

    //根据ID位置控制电机前进后退
    if (steps == 0) Trace = 1;
    if (steps == 1) Trace = 0;
    if (steps == 2) Trace = 2;
    if (steps == 3) Trace = 1;
    if (steps == 4) Trace = 0;
    if (steps == 5) Trace = 2;
    if (steps == 6 && BJ::getStepping()) Trace = 0;
    if (steps == 6 && !BJ::getStepping()) {
      pattern = 0; Shelves = 0; column = 0; line = 0;
      line_front = 0; IDnumber =  99; Trace = 0; steps = 0;
      Serial3.print("state1");
    }
  }

  line_front = line;
  IDnumber = IDcurrent;

}

void picked(int number) { //取货
  QD::translations(number, 1);
  if (line == 1) {//第一行
    BJ::Position(2);
  }
  if (line == 2) {//第二行
    BJ::Position(4);
  }
  delay(1000);
  QD::translations(number, 2);
  steps = 2;
}

void delivery(int number) { //送货
  QD::translations(number, 1);
  if (line == 1) {//第一行
    BJ::Position(1);
  }
  if (line == 2) {//第二行
    BJ::Position(3);
  }
  delay(1000);
  QD::translations(number, 2);
  steps = 5;
}

void Tims() {
  BJ::pulse();
  val_i--;
  if(val_i <= 0){
    val_i = 1000;
    voltage();
  }
}


void voltage() { //检测电压
  val = analogRead(A15);//获取A0模拟值 赋值给val
  val = val * (5.00 / 1023.00) * 3.49; 
  if ((val_numer - val) > 0.1 || ((val - val_numer) > 0.1)) {//限幅滤波
    if (val != val_numer) {
      Serial3.print("voltage");
      Serial3.print(val);
    }
  }
  val_numer = val;
}

void Esp8266() {//接收esp07s的串口信息并处理
  String comdata = "";
  while (Serial3.available() > 0)
  {
    comdata += char(Serial3.read());
    delay(2);
  }
  if (comdata.length() > 0)
  {
    int j = -1;
    for (int i = 0; i < comdata.length() ; i++) {
      if (comdata[i] == '#') {
        j++;
        String a = getValue(comdata, '#', j);
       // Serial.println(a);
        S0.tring b = getValue(a, '&', 0);
        int c = getValue(a, '&', 1).toInt();
        wmt_port(b, c);
        //Serial.println(b);
        //Serial.println(c);
      }
    }
  }
  comdata = String("");
}

void wmt_port(String Name, int number) {//把接收到的数据保存
  if (Name == "pattern") pattern = number;
  if (Name == "Shelves") Shelves = number;
  if (Name == "column") column = number;
  if (Name == "line") line = number;
}

String getValue(String data, char separator, int index)//字符串分割
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

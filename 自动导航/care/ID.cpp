#include "ID.h"
#include "Arduino.h"

int ID::IDcard() {//ID卡读取
  int card_k = 0;
  int card_i = 0;
  char card0[10];
  char card1[10] = {'0', '0', '1', '2', '8', '2', '0', '4', '1', '6'};
  char card2[10] = {'0', '0', '1', '2', '7', '7', '5', '4', '4', '3'};
  char card3[10] = {'0', '0', '1', '2', '8', '3', '5', '8', '9', '4'};
  char card4[10] = {'0', '0', '1', '2', '8', '1', '3', '3', '7', '3'};
  char card5[10] = {'0', '0', '1', '2', '7', '9', '7', '9', '9', '6'};
  char card6[10] = {'0', '0', '1', '2', '8', '1', '1', '1', '7', '4'};
  char card7[10] = {'0', '0', '1', '2', '8', '2', '0', '4', '1', '1'};
  char card8[10] = {'0', '0', '1', '2', '8', '3', '9', '9', '4', '4'};
  char card9[10] = {'0', '0', '1', '2', '7', '7', '5', '6', '3', '7'};
  char card10[10] = {'0', '0', '1', '2', '8', '0', '6', '1', '1', '9'};
  char card11[10] = {'0', '0', '0', '5', '5', '8', '2', '6', '3', '7'};
  while (Serial1.available()) //当发现缓存中有数据时，将数据送至字符数组中
  {
    card0[card_i] = Serial1.read();
    card_i++;
    delay(3);
  }
  //Serial.print(card0);
  if (isCard(card0, card1)) card_k = 1;
  if (isCard(card0, card2)) card_k = 2;
  if (isCard(card0, card3)) card_k = 3;
  if (isCard(card0, card4)) card_k = 4;
  if (isCard(card0, card5)) card_k = 5;
  if (isCard(card0, card6)) card_k = 6;
  if (isCard(card0, card7)) card_k = 7;
  if (isCard(card0, card8)) card_k = 8;
  if (isCard(card0, card9)) card_k = 9;
  if (isCard(card0, card10)) card_k = 10;
  if (isCard(card0, card11)) card_k = 11;
    
  return card_k;
}


bool ID::isCard(char cardTemp[10], char card[10]) {//ID卡比对
  bool isTure = true;
  for (int i = 0 ; i < 10; i++) {
    if (cardTemp[i] != card[i])  {
      isTure = false;
      break;
    }
  }
  return isTure;
}

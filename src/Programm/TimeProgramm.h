#ifndef TIME_PROGRAMM_H
#define TIME_PROGRAMM_H

#include <Arduino.h>
#include "RTClib.h"

//Класс программы, срабатывающей по времени
class TimeProgramm
{
  public:

    //Время активации программы, мин
    uint16_t  StartTime;

    //Время деактивации, мин
    uint16_t  EndTime;

    //Результат
    bool Result;

    //Конструктор класса
    TimeProgramm();

    //Конструктор класса
    TimeProgramm(uint16_t starttime, uint16_t endtime);

    //Обновление данных. Метод вызывается в loop
    //Управляет логикой
    bool Update(uint16_t minutes);


   private:

          //Текущая дата
          DateTime CurrentDateTime;

          //Текущая минута
          uint16_t CurrentMinute = 0;
};

#endif
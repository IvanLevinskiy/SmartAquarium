#ifndef TERMOCOUPLE_PROGRAMM_H
#define TERMOCOUPLE_PROGRAMM_H

#include <Arduino.h>

//Класс программы, срабатывающей по значению температуры
class TermocoupleProgramm
{
  public:

    //Температура отключения
    double  HighLimit;

    //Температура включения
    double  LowLimit;

    //Результат
    bool Result;

    //Конструктор класса
    TermocoupleProgramm();

    //Конструктор класса
    TermocoupleProgramm(double lowlimit, double highlimit);

    //Обновление данных. Метод вызывается в loop
    //Управляет логикой
    bool Update(double currentTemperature);

   private:

};

#endif
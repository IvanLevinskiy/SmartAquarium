#ifndef SHORT_RELAY_ACTIVATE_PROGRAMM_H
#define SHORT_RELAY_ACTIVATE_PROGRAMM_H

#include <Arduino.h>
#include "Timers/Timer.h"

//Класс программы, срабатывающей по времени
class ShortRelayActivateProgramm
{
  public:

    //Результат
    bool Result;

    //Конструктор класса
    ShortRelayActivateProgramm(long ms);

    //Обновление данных. Метод вызывается в loop
    //Управляет логикой
    bool Update(bool trigger);

    private:

    //Длительность импульса, мс
    long ms = 0;

    //Флаг для определения готовности
    bool ready = true;

    //Флаг, указывающий, что
    //программа работает
    bool isrun = false;

    //Время, когда запушена программа
    long start_millis;

    //Текущее время
    long current_millis;

    //Определение тика
    bool _isTick();
};

#endif
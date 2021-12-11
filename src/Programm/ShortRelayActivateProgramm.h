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

    //Таймер
    xTimer* timer;

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

    //Метод для активации программы
    void StartProgramm();


};

#endif
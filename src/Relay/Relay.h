#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

//Класс программы, срабатывающей по времени
class Relay
{
  public:

    //Пин, к которому подключено реле
    uint8_t  Pin;

    //Результат
    bool State;

    //Конструктор класса
    Relay();

    //Конструктор класса
    Relay(uint8_t pin);

    //Обновление данных. Метод вызывается в loop
    //Управляет логикой
    void Send();


   private:

          //Пин, к которому подключено реле
          int pin;
};

#endif
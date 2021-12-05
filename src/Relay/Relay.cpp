#include "Relay\Relay.h"

//Конструктор класса
Relay::Relay(uint8_t pin)
{
    Pin = pin;
    pinMode(Pin, OUTPUT);
}

//Метод для отправки значения в пин
void Relay::Send()
{
    digitalWrite(Pin, !State);
}

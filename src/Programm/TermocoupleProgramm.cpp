#include "Programm\TermocoupleProgramm.h"

//Конструктор класса
TermocoupleProgramm::TermocoupleProgramm(double lowlimit, double highlimit)
{
    HighLimit = highlimit;
    LowLimit = lowlimit;
}

bool TermocoupleProgramm::Update(double currentTemperature)
{
    //Выключение при превышении температуры
    //выше максимума
    if(currentTemperature > HighLimit)
    {
        Result = false;
    }

    //Включение при понижении температуры
    //меньше минимума
    if(currentTemperature <= LowLimit)
    {
        Result = true;
    }

    //Возврат результата
    return Result;
}
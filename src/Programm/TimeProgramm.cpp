#include "Programm\TimeProgramm.h"

//Конструктор класса
TimeProgramm::TimeProgramm(uint16_t starttime, uint16_t endtime)
{
    StartTime = starttime;
    EndTime   = endtime;
}

//Обновление данных. Метод вызывается в loop
//Управляет логикой
bool TimeProgramm::Update(uint16_t currentMinutes)
{
    //Обновление результата программы
    Result = currentMinutes >= StartTime && currentMinutes < EndTime;

    //Возврат результата
    return Result;
}
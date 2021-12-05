#include "Programm\TimeProgramm.h"

//Конструктор класса
TimeProgramm::TimeProgramm(uint16_t starttime, uint16_t endtime, RTC_DS1307* rtc)
{
    StartTime = starttime;
    EndTime   = endtime;
    this->RTC = rtc;
}

//Обновление данных. Метод вызывается в loop
//Управляет логикой
bool TimeProgramm::Update()
{
    //Получаем текущее время
    CurrentDateTime = RTC->now();
    CurrentMinute = (uint16_t)CurrentDateTime.hour() * 60 + (uint16_t)CurrentDateTime.minute();

    //Обновление результата программы
    Result = CurrentMinute >= StartTime && CurrentMinute < EndTime;

    //Возврат результата
    return Result;
}
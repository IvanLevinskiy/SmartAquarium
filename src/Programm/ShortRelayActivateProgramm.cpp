#include"Programm/ShortRelayActivateProgramm.h"

//Конструктор класса
ShortRelayActivateProgramm::ShortRelayActivateProgramm(long ms)
{
    this->ms = ms;
}

//Определение события тика
bool ShortRelayActivateProgramm::_isTick()
{
    //Получение текущего времени
    current_millis = millis();

    if (current_millis >= start_millis) 
    {   
        return (current_millis >= (start_millis + ms));
    } 
    else  
    {   
        return(current_millis >=(4294967295 - start_millis + ms));
    }
}

//Обновление данных. Метод вызывается в loop
//Управляет логикой
bool ShortRelayActivateProgramm::Update(bool trigger)
{
    //Проверка на необходимость активации
    //программы
    if(trigger == true && ready == true)
    {
        //Запоминание времени, когда 
        //сработал триггер
        this->start_millis = millis();
        isrun = true;
        ready = false;
        Result = true;
        Serial.print(start_millis);
        Serial.println("click");
    }

    //Взвод флага готовности по окончанию
    //программы и сбросу сигнала триггекра
    if(trigger == false && isrun == false)
    {
        this->ready = true;
        this->Result = false;
    }

    //Если программа не выполняется-
    //выход
    if(this->isrun == false )
    {
        this->Result = false;
        return false;
    }

    //Если время тика настало
    if(_isTick() == true)
    {
        Result = false;
        isrun = false;
    }

    //Возврат результата
    return Result;
}


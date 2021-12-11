#include"Programm/ShortRelayActivateProgramm.h"

//Конструктор класса
ShortRelayActivateProgramm::ShortRelayActivateProgramm(long ms)
{
    this->ms = ms;
}

//Метод для активации программы
void ShortRelayActivateProgramm::StartProgramm()
{
    //Удаление указателя на таймер
    delete timer;

    //Создание нового экземпляра таймера
    timer = new xTimer(ms);

    //Запуск таймера
    timer->Start();

    //Установка результата
    Result = true;

}

//Обновление данных. Метод вызывается в loop
//Управляет логикой
bool ShortRelayActivateProgramm::Update(bool trigger)
{
    //Проверка на необходимость активации
    //программы
    if(trigger == true && ready == true)
    {
        StartProgramm();
        ready = false;
    }

    //Защита от нулевого 
    //указателя на объект timer
    if(timer == NULL)
    {
        Result = false;
        return false;
    }

    //Обновление таймера
    timer->Update();

    //Если сработал таймер
    //деактивирует результат
    //выподнения программы
    if(timer->IsTick())
    {
        Result = false;
        ready = true;
    }

    //Возврат результата
    return Result;


}
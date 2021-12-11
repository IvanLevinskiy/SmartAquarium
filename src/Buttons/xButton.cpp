#include<Buttons/xButton.h>

xButton::xButton()
{
}

//Конструктор класса
xButton::xButton(int pin)
{
    this->pin = pin;
    oldState = true;

    //Настраиваем пин как вход
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);

    //Подтягиваем пин к плюсу питания
    //digitalWrite(pin, HIGH);

    //Запускаем таймер
    Timer.Start();
}

//Метод для чтения состояния 
//пина к которму подключена кнопка
bool xButton::Read()
{
    return digitalRead(pin);
}

//Метод возвращает состояние
//зафиксировано ли нажатие кнопки
bool xButton::IsPressed()
{
   return !Read();
}

//Сброс
void xButton::Reset()
{
    TimeIsFrontPositive = millis();
}

bool xButton::FrontPositive()
{
    bool state = IsPressed();

    //Если состояние кнопки изменилось с 0 на 1
    if(oldState == false && state == true)
    {
        //Запоминаем время, когда кнопка нажата
        TimeIsFrontPositive = millis();
        oldState = state;
        return true;
    }

    //В противном случае запоминаем состояние кнопки и возвращаем
    //0
    oldState = state;
    return false;
}

//Хитрый метод, который поможет изменить значение 
//переменой при нажатой кнопке вверх или вниз
int xButton::GetAdditive()
{
    //Обновляем состояние таймера
    Timer.Update();

    //Если кнопка не нажата, возвращаем 0
    if(Read() == true)
    {
        return 0;
    }

    //Если кнопка нажата, то добавляем значение переменной
    //вычисляя по формуле
    int difference = millis() - TimeIsFrontPositive;

    //Если разница маленькая, тогда возвращаем 0
    //имитируя задержку
    if(difference < 500)
    {
        return 0;
    }

    //Смещаем, чтоб значение изменялось
    //равномерно
    difference-= 500;

    if(difference > 5000)
    {
        difference = 100;
        goto M1;
    }

    //Масштабируем
    difference = difference / 50 ;

    //Если таймер тикает, тогда возвращаем число
   M1: if(Timer.IsTick())
        {
            return difference;
        }

    return difference;
    

}
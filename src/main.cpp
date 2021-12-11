#include <Arduino.h>
#include "RTClib.h"
#include <OneWire.h>
#include "Timers/Timer.h"
#include <DallasTemperature.h>
#include "Relay/Relay.h"
#include "Buttons/xButton.h"
#include "Programm/TimeProgramm.h"
#include "Programm/TermocoupleProgramm.h"
#include "Programm/ShortRelayActivateProgramm.h"

//Часы реального времени RTC
RTC_DS1307 rtc;

//Кнопка
xButton xbutton = xButton(10);

// Пин, к которому подключен датчик температуры
#define ONE_WIRE_BUS 2

// Инициализация OneWire
OneWire oneWire(ONE_WIRE_BUS);

//Инициализация  DallasTemperature
DallasTemperature sensors(&oneWire);

//Инициализация датчика температуры
DeviceAddress Thermometer;

//Таймер для обновления температуры
xTimer TemperatureTimer = xTimer(1000);

#pragma region ИНИЦИАЛИЗАЦИЯ РЕЛЕ

//Реле 1
Relay Relay_1 = Relay(1);

//Реле 2
Relay Relay_2 = Relay(1);

//Реле 3
Relay Relay_3 = Relay(1);

//Реле 4
Relay Relay_4 = Relay(1);

//Реле 5
Relay Relay_5 = Relay(1);

//Реле 6
Relay Relay_6 = Relay(1);

//Реле 7
Relay Relay_7 = Relay(1);

//Реле 8
Relay Relay_8 = Relay(1);

#pragma endregion ИНИЦИАЛИЗАЦИЯ РЕЛЕ

#pragma region ПРОГРАММА ДЛЯ РЕЛЕ

//Программа для включения реле 1 с 09:00 до 13:00
TimeProgramm* programm_10;

//Программа для включения реле 1 с 16:30 до 21:30
TimeProgramm* programm_11;

//Программа для включения реле 2 с 08:30 до 21:00
TimeProgramm* programm_20;

//Программа для кратковременного включения
//реле 3 на 5000мс в 10:10
ShortRelayActivateProgramm* programm_30;

//Программа для включения реле 4 с 10:09 до 10:14
TimeProgramm* programm_40;

//Программа для кратковременного включения
//реле 4 при нажатии кнопки
ShortRelayActivateProgramm* programm_41;

//Программа для кратковременного включения
//реле 5 на 5 сек в 09.00
ShortRelayActivateProgramm* programm_50;

//Программа для кратковременного включения
//реле 6 на 5 сек в 22.00
ShortRelayActivateProgramm* programm_60;

//Программа для включения реле 7 при падении температуры до 27 градусов
//и выключении при превышении температуры 25 градусов
TermocoupleProgramm* programm_70;

//Программа для включения реле 8 при падении температуры до 26 градусов
//и выключении при превышении температуры 28 градусов
TermocoupleProgramm* programm_80;

#pragma endregion ПРОГРАММА ДЛЯ РЕЛЕ

//Температура
float temperature = 0;

//Метод для чтения температуры
//с датчика температуры
void ReadTemperature(bool force = false)
{
  //Обновление состояния таймера
  //при срабатывании которого
  //обновляется значение температуры
  //считанное с датчика температуры
  TemperatureTimer.Update();

  //Получение температуры раз в секунду
  //по тику таймера TemperatureTimer либо по сигнало force
  if(TemperatureTimer.IsTick() == true || force == true)
  {
      temperature = sensors.getTempC(Thermometer);
  }
}

//Инициализация
void setup() 
{
  //Инициализация связи часов реального
  //времени с микроконтроллером
  rtc.begin();

  //Запуск шины OneWire
  sensors.begin();

  //Создание программы для реле 1
  //Время включения  540 мин (09:00)
  //Время выключения 780 мин (13:00)
  programm_10 = new TimeProgramm(540, 780);

  //Создание программы для реле 1
  //Время включения  990 мин (16:30)
  //Время выключения 1290 мин (21:30)
  programm_11 = new TimeProgramm(1290, 780);

  //Создание программы для реле 2
  //Время включения  510 мин (08:30)
  //Время выключения 1260 мин (21:00)
  programm_20 = new TimeProgramm(510, 1260);

  //Создание программы для реле 3
  //включение реле на 5 сек в 10:10
  programm_30 = new ShortRelayActivateProgramm(5000);

  //Создание программы для реле 4
  //Время включения  609 мин (10:09)
  //Время выключения 614 мин (10:14)
  programm_40 = new TimeProgramm(609, 614);

  //Создание программы для реле 4
  //активация на 5 мин при нажатии кнопки
  programm_41 = new ShortRelayActivateProgramm(30000);

  //Создание программы для реле 5
  //активация на 5 сек в 09:00
  programm_50 = new ShortRelayActivateProgramm(5000);

  //Создание программы для реле 6
  //активация на 5 сек в 22:00
  programm_60 = new ShortRelayActivateProgramm(5000);

  //Создание программы для реле 7
  //активация при падении температуры до 25 градусов
  //деактивация при превышении темпетауры свыше 27 градусов
  programm_70 = new TermocoupleProgramm(25.0, 27.0);

  //Создание программы для реле 8
  //активация при падении температуры до 25 градусов
  //деактивация при превышении темпетауры свыше 27 градусов
  programm_80 = new TermocoupleProgramm(26.0, 28.0);

  //Запуск таймера
  TemperatureTimer.Start();

  //Чтение температуры по сигналу force
  ReadTemperature(true);
}

//Количество минут с начала
//суток
uint16_t CurrentMinute;

//Бесконечный цикл
void loop() 
{
  //Чтение температуры с датчика
  //температуры
  ReadTemperature();

  //Определение нажатия кнопки
  bool btnPressed = xbutton.IsPressed();

  //Получение количества минут 
  //с начала суток
  CurrentMinute = (uint16_t)rtc.now().hour() * 60 + (uint16_t)rtc.now().minute();
  
  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 1
  Relay_1.State  = programm_10->Update(CurrentMinute);
  Relay_1.State |= programm_11->Update(CurrentMinute);
  Relay_1.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 2
  Relay_2.State = programm_20->Update(CurrentMinute);
  Relay_2.Send();

  //КРАТКОВРЕМЕННОЕ ВКЛЮЧЕНИЕ РЕЛЕ 3
  //на 5000 мс в 10.10
  bool rly3ActivateCMD = CurrentMinute == 610;
  Relay_3.State = programm_30->Update(rly3ActivateCMD);
  Relay_3.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 4
  //РЕЛЕ 4 ВКЛЮЧАЕТСЯ:
  //  1 - С 10.09 ДО 10.14
  //  2 - НА 5000 МС ПРИ НАЖАТИИ КНОПКИ
  Relay_4.State  = programm_40->Update(CurrentMinute);
  Relay_4.State |= programm_41->Update(btnPressed);
  Relay_4.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 5
  //кратковременное включение на 5000 МС в 09:00
  bool rly5ActivateCMD = CurrentMinute == 540;
  Relay_5.State  = programm_50->Update(rly5ActivateCMD);
  Relay_5.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 6
  //кратковременное включение на 5000 МС в 22:00
  bool rly6ActivateCMD = CurrentMinute == 1320;
  Relay_6.State  = programm_60->Update(rly6ActivateCMD);
  Relay_6.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 7
  //включение реле при 25 градусах и выключение при 27
  Relay_7.State  = programm_70->Update(temperature);
  Relay_7.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 8
  //включение реле при 26 градусах и выключение при 28
  Relay_8.State  = programm_80->Update(temperature);
  Relay_8.Send();


}
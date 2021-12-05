#include <Arduino.h>
#include "RTClib.h"
#include <OneWire.h>
#include "Timer/Timer.h"
#include <DallasTemperature.h>
#include "Relay/Relay.h"
#include "Programm/TimeProgramm.h"
#include "Programm/TermocoupleProgramm.h"

//Часы реального времени RTC
RTC_DS1307 rtc;

// Пин, к которому подключен датчик температуры
#define ONE_WIRE_BUS 2

// Инициализация OneWire
OneWire oneWire(ONE_WIRE_BUS);

//Инициализация  DallasTemperature
DallasTemperature sensors(&oneWire);

//Инициализация датчика температуры
DeviceAddress Thermometer;

//Таймер для обновления температуры
Timer TemperatureTimer = Timer(1000);

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
TimeProgramm* programm_1;

//Программа для включения реле 1 с 16:30 до 21:30
TimeProgramm* programm_2;

//Программа для включения реле 2 с 08:30 до 21:00
TimeProgramm* programm_3;

//Программа для включения реле 7 при падении температуры до 27 градусов
//и выключении при превышении температуры 25 градусов
TermocoupleProgramm* programm_4;

#pragma endregion ПРОГРАММА ДЛЯ РЕЛЕ

//Температура
float temperature = 0;

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
  programm_1 = new TimeProgramm(540, 780, &rtc);

  //Создание программы для реле 1
  //Время включения  990 мин (16:30)
  //Время выключения 1290 мин (21:30)
  programm_2 = new TimeProgramm(1290, 780, &rtc);

  //Создание программы для реле 2
  //Время включения  510 мин (08:30)
  //Время выключения 1260 мин (21:00)
  programm_3 = new TimeProgramm(510, 1260, &rtc);

  //Создание программы для реле 7
  //Температура включения  25 градусов
  //Температура выключения  27 градусов
  programm_4 = new TermocoupleProgramm(25.0, 27.0);


  //Запуск таймера
  TemperatureTimer.Start();

  //Чтение температуры с датчика температуры
  temperature = sensors.getTempC(Thermometer);
}

//Бесконечный цикл
void loop() 
{
  //Обновление состояния таймера
  TemperatureTimer.Update();

  //Получение температуры раз в секунду
  if(TemperatureTimer.IsTick() == true)
  {
      temperature = sensors.getTempC(Thermometer);
  }
  
  //Обновление статуса реле 1
  Relay_1.State = programm_1->Update() || programm_2->Update();
  Relay_1.Send();

  //Обновление статуса реле 2
  Relay_2.State = programm_3->Update();
  Relay_2.Send();

  //Обновление статуса реле 7
  Relay_7.State = programm_4->Update(0.0);
  Relay_7.Send();

}
#include <Arduino.h>
#include "RTClib.h"
#include <OneWire.h>
#include "Timers/Timer.h"
#include <DallasTemperature.h>
#include "Relay/Relay.h"
#include "Buttons/xButton.h"
#include <LiquidCrystal_I2C.h>
#include "Programm/TimeProgramm.h"
#include "Programm/TermocoupleProgramm.h"
#include "Programm/ShortRelayActivateProgramm.h"

//Часы реального времени RTC
RTC_DS1307 rtc;

//Текущая дата-время
DateTime now;

//Кнопка
xButton xbutton = xButton(A0);

// Пин, к которому подключен датчик температуры
#define ONE_WIRE_BUS 9

// Инициализация OneWire
OneWire oneWire(ONE_WIRE_BUS);

//Инициализация  DallasTemperature
DallasTemperature sensor(&oneWire);

//Инициализация датчика температуры
DeviceAddress Thermometer;

//Таймер для обновления температуры
xTimer TemperatureTimer = xTimer(1000);

//lcd экран
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

#pragma region ИНИЦИАЛИЗАЦИЯ РЕЛЕ

//Реле 1
Relay Relay_1 = Relay(0);

//Реле 2
Relay Relay_2 = Relay(1);

//Реле 3
Relay Relay_3 = Relay(2);

//Реле 4
Relay Relay_4 = Relay(3);

//Реле 5
Relay Relay_5 = Relay(4);

//Реле 6
Relay Relay_6 = Relay(5);

//Реле 7
Relay Relay_7 = Relay(6);

//Реле 8
Relay Relay_8 = Relay(7);

#pragma endregion ИНИЦИАЛИЗАЦИЯ РЕЛЕ

#pragma region ПРОГРАММА ДЛЯ РЕЛЕ

//ПРОГРАММА ДЛЯ ВКЛЮЧЕНИЯ РЕЛЕ 1
//Время включения  540 мин (09:00)
//Время выключения 780 мин (13:00)
TimeProgramm programm_10 = TimeProgramm(540, 780);

//ПРОГРАММА ДЛЯ ВКЛЮЧЕНИЯ РЕЛЕ 1
//Время включения  990 мин (16:30)
//Время выключения 1290 мин (21:30)
TimeProgramm programm_11 = TimeProgramm(990, 1290);

//ПРОГРАММА ДЛЯ ВКЛЮЧЕНИЯ РЕЛЕ 2
//Время включения  510 мин (08:30)
//Время выключения 1260 мин (21:00)
TimeProgramm programm_20 = TimeProgramm(510, 1260);

//ПРОГРАММА ДЛЯ ВКЛЮЧЕНИЯ РЕЛЕ 3
//кратковременное включение реле на 5 сек в 10:10
ShortRelayActivateProgramm programm_30 = ShortRelayActivateProgramm(5000);

//ПРОГРАММА ДЛЯ ВКЛЮЧЕНИЯ РЕЛЕ 4
//Время включения  609 мин (10:09)
//Время выключения 614 мин (10:14)
TimeProgramm programm_40 = TimeProgramm(609, 614);

//ПРОГРАММА ДЛЯ ВКЛЮЧЕНИЯ РЕЛЕ 4
//Активация реле на 5 мин при нажатии кнопки
ShortRelayActivateProgramm programm_41 = ShortRelayActivateProgramm(300000);

//ПРОГРАММА ДЛЯ ВКЛЮЧЕНИЯ РЕЛЕ 5
//Реле 5 на 5 сек в 09.00
ShortRelayActivateProgramm programm_50 = ShortRelayActivateProgramm(5000);

//ПРОГРАММА ДЛЯ ВКЛЮЧЕНИЯ РЕЛЕ 6
//кратковременное включение реле на 5 сек в 22:00
ShortRelayActivateProgramm programm_60 = ShortRelayActivateProgramm(5000);

//ПРОГРАММА ДЛЯ ВКЛЮЧЕНИЯ РЕЛЕ 7
//активация при падении температуры до 25 градусов
//деактивация при превышении темпетауры свыше 27 градусов
TermocoupleProgramm programm_70 = TermocoupleProgramm(27.0, 25.0);

//ПРОГРАММА ДЛЯ ВКЛЮЧЕНИЯ РЕЛЕ 8
//активация при падении температуры до 26 градусов
//деактивация при превышении темпетауры свыше 28 градусов
TermocoupleProgramm programm_80 = TermocoupleProgramm(28.0, 26.0);

#pragma endregion ПРОГРАММА ДЛЯ РЕЛЕ

//Температура
float temperature = 0;

//Вывод элемента даты 
void printnum(uint8_t num)
{
  if(num < 10)
  {
    lcd.print('0');
  }

  lcd.print(num);

}

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
      // отправляем запрос на измерение температуры
      sensor.requestTemperatures();
      delay(100);

      // считываем данные из регистра датчика
      temperature = sensor.getTempCByIndex(0);
  }
}

//Инициализация
void setup() 
{
  //Инициализация LCD дисплея
  lcd.init();                   

  //Включение подсветки LCD дисплея        
  lcd.backlight();          

  //Инициализация связи часов реального
  //времени с микроконтроллером
  rtc.begin();

  //Установка времени RTC из ПК при прошивке
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //Запуск шины OneWire
  sensor.begin();

  //устанавливаем разрешение датчика от 9 до 12 бит
  sensor.setResolution(12);

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
  Relay_1.State  = programm_10.Update(CurrentMinute);
  Relay_1.State |= programm_11.Update(CurrentMinute);
  Relay_1.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 2
  Relay_2.State = programm_20.Update(CurrentMinute);
  Relay_2.Send();

  //КРАТКОВРЕМЕННОЕ ВКЛЮЧЕНИЕ РЕЛЕ 3
  //на 5000 мс в 10.10
  bool rly3ActivateCMD = CurrentMinute == 610;
  Relay_3.State = programm_30.Update(rly3ActivateCMD);
  Relay_3.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 4
  //РЕЛЕ 4 ВКЛЮЧАЕТСЯ:
  //  1 - С 10.09 ДО 10.14
  //  2 - НА 5000 МС ПРИ НАЖАТИИ КНОПКИ
  Relay_4.State  = programm_40.Update(CurrentMinute);
  Relay_4.State |= programm_41.Update(btnPressed);
  Relay_4.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 5
  //кратковременное включение на 5000 МС в 09:00
  bool rly5ActivateCMD = CurrentMinute == 540;
  Relay_5.State  = programm_50.Update(rly5ActivateCMD);
  Relay_5.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 6
  //кратковременное включение на 5000 МС в 22:00
  bool rly6ActivateCMD = CurrentMinute == 1320;
  Relay_6.State  = programm_60.Update(rly6ActivateCMD);
  Relay_6.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 7
  //включение реле при 25 градусах и выключение при 27
  Relay_7.State  = programm_70.Update(temperature);
  Relay_7.Send();

  //ОБНОВЛЕНИЕ СОСТОЯНИЯ РЕЛЕ 8
  //включение реле при 26 градусах и выключение при 28
  Relay_8.State  = programm_80.Update(temperature);
  Relay_8.Send();

  //Получение даты-времени из RTC
  now = rtc.now();


  //Вывод времени в lcd
  lcd.setCursor(0,0);
  //lcd.print(now.year(), DEC);
  //lcd.print('/');
  //lcd.print(now.month(), DEC);
  //lcd.print('/');
  //lcd.print(now.day(), DEC);
  //lcd.print(" ");
  printnum(now.hour());
  lcd.print(':');
  printnum(now.minute());
  lcd.print(':');
  printnum(now.second());

  //Вывод температуры
  lcd.setCursor(0,1);
  lcd.print(temperature);
}


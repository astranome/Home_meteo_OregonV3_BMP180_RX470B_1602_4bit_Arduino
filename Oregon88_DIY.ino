#include <Oregon_NR.h>

//#include <LiquidCrystal_I2C.h>
//#include <Wire.h>
#include <LiquidCrystalRus.h>
#include <Adafruit_BMP085.h>
// подключаем стандартную библиотеку LiquidCrystal
//#include <LiquidCrystal.h>
 
// инициализируем объект-экран, передаём использованные 
// для подключения контакты на Arduino в порядке:
// RS, E, DB4, DB5, DB6, DB7
LiquidCrystalRus lcdm(12, 11, 9, 8, 7, 6);

#define UNO
//#define pirPin 4
#define ledPin 13
Adafruit_BMP085 bmp;
//Oregon_NR oregon(2, 0, 13); // Для Arduino UNO/Nano - датчик на выводе D2, Прерывание 0, Светодиод приёма на вывод 13
Oregon_NR oregon(2, 0); // Если светодиод не нужен
//LiquidCrystal_I2C lcd(0x27,16,2); 
long a=0;

void setup() {
   Serial.begin(115200);
  //вкючение прослушивания радиоканала  
  //pinMode(pirPin, INPUT);
  pinMode(ledPin,OUTPUT);
  // устанавливаем размер (количество столбцов и строк) экрана
  lcdm.begin(16, 2);
  // печатаем первую строку
  lcdm.print("Еле включил !@#$%^&* :-)");
 if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
  
  oregon.start(); 
 oregon.receiver_dump = 0;       //true - Включает "осциллограф" - отображение данных, полученных с приёмника
  
  //Если не распознаются последние байты принятого пакета можно попробовать подстроить частоту захвата данных
  oregon.timing_correction = 0;  // коррекция частоты завхвата данных. Обычно достаоточно подобрать значение от -5 до 5
  oregon.decode_method = 3;      //или использовать метод 3 для декодирования

// lcd.init();  
//  lcd.backlight();
}
void loop() {
//*
   //oregon.capture(0);
 if (a > 1000000) {
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    /*
    lcdm.setCursor(0, 0);
    lcdm.print("Температура ");
    lcdm.print(bmp.readTemperature());
    lcdm.println(" *C");
    */
 // печатаем вторую строку
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    lcdm.setCursor(10, 0);
    //lcdm.print("Давление ");
    int p = (bmp.readPressure()* 0.0075);
    lcdm.print(p);
    lcdm.println("ммРт");
    // delay(500);
    a=0;
 }
    a=a+1;
   // */
   //////////////////////////////////////////////////////////////////////
  //Захват пакета,//////////////////////////////////////////////

  oregon.capture(0);
  //Захваченные данные годны до следующего вызова capture
  //ОБработка полученного пакета//////////////////////////////////////////////
  if (oregon.captured)  {
    //Вывод информации в Serial
     Serial.print ((float) millis() / 1000, 1); //Время
    Serial.print ("s\t\t");
    //Версия протокола
    if (oregon.ver == 2) Serial.print("  ");
    if (oregon.ver == 3) Serial.print("3 ");
    
    if ((oregon.sens_type == THGR810) && oregon.crc_c){
    
    //if ((oregon.sens_type == THGN132 || (oregon.sens_type & 0x0FFF) == RTGN318 || oregon.sens_type == THGR810 || oregon.sens_type == THN132) && oregon.crc_c){
      lcdm.setCursor(0,0);
    /*
      if (oregon.sens_tmp > 0 && oregon.sens_tmp < 10) lcd.print("T:  ");
      if (oregon.sens_tmp < 0 && oregon.sens_tmp >-10) lcd.print("T: ");
      if (oregon.sens_tmp <= -10) lcd.print("T:");
      if (oregon.sens_tmp >= 10) lcd.print("T: ");
      */
      lcdm.print(oregon.sens_tmp, 1);
     lcdm.print((char)223);   
     lcdm.print("C ");
            
      //lcd.print("BAT:");
     // if (oregon.sens_battery) lcd.print("F"); else lcd.print("e");
        Serial.print("HUM: ");
        Serial.print(oregon.sens_hmdty, 0);
        Serial.print("%");
        //lcd.setCursor(0,1);
       // lcd.print("HUM: ");
        lcdm.print(oregon.sens_hmdty, 0);
        lcdm.print("% ");
        
    }
   if ((oregon.sens_type == WGR800) && oregon.crc_c){
      Serial.print("\t");
      //float wnddata;
      Serial.print(" TYPE: ");
      Serial.print("WGR800");
      Serial.print(" AVG WS: ");
      Serial.print(oregon.sens_avg_ws, 1);
      lcdm.setCursor(0,1);
      lcdm.print("Вет: ");
      lcdm.print(oregon.sens_avg_ws, 1);
     /* 
      Serial.print("m/s MAX WS: ");
      Serial.print(oregon.sens_max_ws, 1);
      Serial.print("m/s WDIR: "); //N = 0, E = 4, S = 8, W = 12
      Serial.print(oregon.sens_wdir);
      */
      lcdm.print("m/s D:"); //N = 0, E = 4, S = 8, W = 12
      lcdm.print(oregon.sens_wdir);
      /*
      Serial.print(" BAT: ");
      if (oregon.sens_battery) Serial.print("F "); else Serial.print("e ");
      Serial.print("ID: ");
      Serial.print(oregon.sens_id, HEX);
      */
   } 
  }
}

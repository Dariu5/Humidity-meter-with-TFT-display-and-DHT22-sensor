#define cs   10
#define dc   9
#define rst  8  

#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <SPI.h>
#include <stdio.h>
#include "DHT.h"

#define DHTPIN 7     
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);


#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif


Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

float humidity, temperature,humidex;
String message;

void setup(void) {

  dht.begin(); 
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);  
}

void loop() {

  // get data from DHT22 

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();


  humidex = calculate_humidex (temperature, humidity);

  temperature_to_lcd (temperature, 0);
  humidity_to_lcd (humidity, 55);
  humidex_to_lcd (humidex, 110);
}

void temperature_to_lcd (float temperature, unsigned char text_position )
{
  tft.setCursor(0,text_position);       
  tft.setTextColor(ST7735_WHITE,ST7735_BLACK);
  tft.setTextSize(1);
  tft.print("Temperature:");

  tft.setTextSize(3);

  if (temperature>0) {
    tft.setTextColor(ST7735_RED,ST7735_BLACK);
  }

  else {
    tft.setTextColor(ST7735_BLUE,ST7735_BLACK);
  }


  tft.setCursor(0,text_position+20);
  fix_number_position(temperature);
  tft.print(temperature,1);
  tft.print(" C"); 

}



void humidity_to_lcd (float humidity, unsigned char text_position )
{


  tft.setTextColor(ST7735_WHITE,ST7735_BLACK);
  tft.setCursor(0,text_position); 
  tft.setTextSize(1); 
  tft.println("Humidity:");
  tft.setTextSize(3);
  tft.setCursor(0,text_position+20);

  fix_number_position(humidity);

  tft.print(humidity,1);
  tft.print(" %");       

}

void humidex_to_lcd (float humidex, unsigned char text_position )

{
  tft.setCursor(0,text_position); 

  tft.setTextSize(1); 
  tft.println("Humidex:");
  tft.setTextSize(3);

  tft.setCursor(0,text_position+17);

  

  if ((humidex >= 21 )&&(temperature < 44)) {
    
    fix_number_position(humidex);
    get_humidex_color_warning_message(humidex);

    tft.print(humidex,1);
    tft.print(" C"); 


    tft.setTextSize(1); 
    tft.println(message);
  }


  else {
    tft.print(" --.- C"); 
    tft.println("                             "); 
  };

}

void fix_number_position(float number)

{


  if ((number >= -40)&&(number < -9.9))
  {
    ;
  } 

  if ((number >= -9.9)&&(number < 0.0))
  {
    tft.print(" ");
  }

  if ((number >= 0.0 )&&(number < 9.9))
  {
    tft.print("  ");
  }

  if ((number >= 9.9 )&&(number < 99.9))
  {
    tft.print(" ");
  }

  if ((number >= 99.9 )&&(number < 151))
  {
    tft.print("");
  }
}

float calculate_humidex(float temperature,float humidity) {
  float e;

  e = (6.112 * pow(10,(7.5 * temperature/(237.7 + temperature))) * humidity/100); //vapor pressure

  float humidex = temperature + 0.55555555 * (e - 10.0); //humidex
  return humidex;

}

void get_humidex_color_warning_message(float humidex)
{
  if ((humidex >= 21 )&&(humidex < 27))
  {
    tft.setTextColor(tft.Color565(0, 137, 0),ST7735_BLACK);
    message= "No discomfort";
  } // dark green

  if ((humidex >= 27 )&&(humidex < 35))
  {
    tft.setTextColor(tft.Color565(76, 255, 0),ST7735_BLACK); // light green
    message= "Some discomfort      ";
  }

  if ((humidex >= 35 )&&(humidex < 40))
  {
    tft.setTextColor(tft.Color565(255, 255, 0),ST7735_BLACK);
    message= "Great discomfort     ";
  } // yellow


  if ((humidex >= 40 )&&(humidex < 46))
  {
    tft.setTextColor(tft.Color565(255, 140, 0),ST7735_BLACK);
    message= "Health risk         ";
  } //light orange

  if ((humidex >= 46 )&&(humidex < 54))
  {
    tft.setTextColor(tft.Color565(221, 128, 0),ST7735_BLACK);
    message= "Great health risk    ";
  } //dark orange

  if ((humidex >= 54 ))
  {
    tft.setTextColor(tft.Color565(255, 0, 0),ST7735_BLACK);
    message= "Heat stroke immninent ";
  } // red
}


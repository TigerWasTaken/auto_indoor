#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "ESP8266_AT.h"
#include "config.h"

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
DHT dht22(DHT22_PIN, DHT22);

/***WI FI constants.***/
#define SEND_DEMO
char _buffer[150];
uint8_t Connect_Status;

void setup() {
    // Needed to communicate with esp86.
    Serial.begin(BAUD_RATE);
    // Init lcd screen of 16x2.
    lcd.begin(16, 2);
    dht22.begin();
    // Start screen at (0, 0) position.
    lcd.home();
    lcd.backlight();
    pinMode(PUMP_PIN, OUTPUT);
    // /***Init WI FI module with thingspeak connection***/
    // while(!ESP8266_Begin());
    // ESP8266_WIFIMode(BOTH_STATION_AND_ACCESPOINT);	/* 3 = Both (AP and STA) */
    // ESP8266_ConnectionMode(SINGLE);     			/* 0 = Single; 1 = Multi */
    // ESP8266_ApplicationMode(NORMAL);    			/* 0 = Normal Mode; 1 = Transperant Mode */
    // if(ESP8266_connected() == ESP8266_NOT_CONNECTED_TO_AP)/*Check WIFI connection*/
    // ESP8266_JoinAccessPoint(SSID, PASSWORD);		/*Connect to WIFI*/
    // ESP8266_Start(0, THINGSPEAK_DOMAIN, PORT);
}

    //DRY SOIL: 1022 - 0
    //WET SOIL: 305 - 100
int getSoilMoisturePercent(int higrometerPin){
    return map(analogRead(higrometerPin),1022,260,0,100);
}

void printHumidityAndTempValues(LiquidCrystal_I2C lcd, int moisturePercent, float temp_value, int soilMoisture){
    lcd.home();
    String msg = "H: " + String(moisturePercent) + "%" + " Soil: " + String(soilMoisture) + "%";
    lcd.print(msg);
    String msg_2 = "Tmp: " + String((int)trunc(temp_value)) + "*C";
    lcd.setCursor(0, 1);
    lcd.print(msg_2);
}

void waterPumpSystem(int soilMoisturePercent, int pump_pin){
    if (soilMoisturePercent < 40)
        digitalWrite(pump_pin, HIGH);
    else
        digitalWrite(pump_pin, LOW);
}

void loop() {
    int soilMoisturePercent =  getSoilMoisturePercent(HIGROMETER_PIN);
    float temp_value =  dht22.readTemperature();
    float moisturePercent = dht22.readHumidity();
    printHumidityAndTempValues(lcd, moisturePercent, temp_value, soilMoisturePercent);
    waterPumpSystem(soilMoisturePercent, PUMP_PIN);
    delay(100);


    // Connect_Status = ESP8266_connected();
    // if(Connect_Status == ESP8266_NOT_CONNECTED_TO_AP)	/*Again check connection to WIFI*/
    // ESP8266_JoinAccessPoint(SSID, PASSWORD);			/*Connect to WIFI*/
    // if(Connect_Status == ESP8266_TRANSMISSION_DISCONNECTED)
    // ESP8266_Start(0, THINGSPEAK_DOMAIN, PORT);						/*Connect to TCP port*/

    // #ifdef SEND_DEMO
    // memset(_buffer, 0, 150);
    // /*connect to thingspeak server to post data using your API_WRITE_KEY*/
    // sprintf(_buffer, "GET /update?api_key=%s&field1=%d", API_WRITE_KEY, DHT22_PIN); 	
    // ESP8266_Send(_buffer);
    // /* Thingspeak server delay */
    // delay(15000); 
    // #endif
}

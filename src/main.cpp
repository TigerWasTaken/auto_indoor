#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "ESP8266_AT.h"
#include "config.h"

// LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows
DHT dht22(DHT22_PIN, DHT22);

/***WI FI constants.***/
#define SEND_DEMO
char _buffer[150];
uint8_t Connect_Status;

void setup() {
    // Needed to communicate with esp86.
    // Serial.begin(BAUD_RATE);
    Serial.begin(9600);
    pinMode(HIGROMETER_PIN5, INPUT);
    pinMode(HIGROMETER_PIN6, INPUT);
    // Init lcd screen of 16x2.
    // lcd.begin(16, 2);
    lcd.init();
    dht22.begin();
    // Start screen at (0, 0) position.
    lcd.home();
    lcd.backlight();
    digitalWrite(PUMP_PIN1, HIGH);
    digitalWrite(PUMP_PIN2, HIGH);
    digitalWrite(PUMP_PIN3, HIGH);
    digitalWrite(PUMP_PIN4, HIGH);
    digitalWrite(PUMP_PIN5, HIGH);
    //digitalWrite(PUMP_PIN6, HIGH);
    pinMode(PUMP_PIN1, OUTPUT);
    pinMode(PUMP_PIN2, OUTPUT);
    pinMode(PUMP_PIN3, OUTPUT);
    pinMode(PUMP_PIN4, OUTPUT);
    pinMode(PUMP_PIN5, OUTPUT);
    //pinMode(PUMP_PIN6, OUTPUT);
    // /***Init WI FI module with thingspeak connection***/
    // while(!ESP8266_Begin());
    // ESP8266_WIFIMode(BOTH_STATION_AND_ACCESPOINT);	/* 3 = Both (AP and STA) */
    // ESP8266_ConnectionMode(SINGLE);     			/* 0 = Single; 1 = Multi */
    // ESP8266_ApplicationMode(NORMAL);    			/* 0 = Normal Mode; 1 = Transperant Mode */
    // if(ESP8266_connected() == ESP8266_NOT_CONNECTED_TO_AP)/*Check WIFI connection*/
    // ESP8266_JoinAccessPoint(SSID, PASSWORD);		/*Connect to WIFI*/
    // ESP8266_Start(0, THINGSPEAK_DOMAIN, PORT);
}

//     DRY SOIL: 1022 - 0
//     WET SOIL: 305 - 100
int getAnalogSoilMoisturePercent(int higrometerPin){
    return map(analogRead(higrometerPin),1022,260,0,100);
}

//     DRY SOIL: 1
//     WET SOIL: 0
bool isDigitalSoilDry(int higrometerDigitalPin){
    return digitalRead(higrometerDigitalPin) == HIGH;
}

void printHumidityAndTempValues(LiquidCrystal_I2C lcd, int moisturePercent, float temp_value){
    String msg = "Hamb:" + String(moisturePercent) + " Tamb:" + String((int)trunc(temp_value)) + " ";
    lcd.print(msg);
}

void printHumiditySoilValue(LiquidCrystal_I2C lcd, int soilMoisture, int plantNumber){
    String msg = "H" +String(plantNumber) +":"+ String(soilMoisture) + " ";
    lcd.print(msg);
}

void waterPumpSystem(int soilMoisturePercent, int pump_pin){
    if (soilMoisturePercent < 40)
    {
        digitalWrite(pump_pin, LOW);
        delay(4000);
        digitalWrite(pump_pin, HIGH);
    }else{
        digitalWrite(pump_pin, HIGH);
    }
}

void loop() {
    lcd.home();
    int soilMoisturePercent1 =  getAnalogSoilMoisturePercent(HIGROMETER_PIN1);
    int soilMoisturePercent2 =  getAnalogSoilMoisturePercent(HIGROMETER_PIN2);
    int soilMoisturePercent3 =  getAnalogSoilMoisturePercent(HIGROMETER_PIN3);
    int soilMoisturePercent4 =  getAnalogSoilMoisturePercent(HIGROMETER_PIN4);
    bool isSoilDry1 =  isDigitalSoilDry(HIGROMETER_PIN5);
    bool isSoilDry2 =  isDigitalSoilDry(HIGROMETER_PIN6);
    float temp_value =  dht22.readTemperature();
    float moisturePercent = dht22.readHumidity();
    printHumidityAndTempValues(lcd, moisturePercent, temp_value);
    lcd.setCursor(0, 1);
    printHumiditySoilValue(lcd, soilMoisturePercent1, 1);
    printHumiditySoilValue(lcd, soilMoisturePercent2, 2);
    printHumiditySoilValue(lcd, soilMoisturePercent3, 3);
    lcd.setCursor(0, 2);  
    printHumiditySoilValue(lcd, soilMoisturePercent4, 4);
    printHumiditySoilValue(lcd, isSoilDry1, 5);
    printHumiditySoilValue(lcd, isSoilDry2, 6);
    delay(2000);
    waterPumpSystem(soilMoisturePercent1, PUMP_PIN1);
    delay(5000);
    waterPumpSystem(soilMoisturePercent2, PUMP_PIN2);
    delay(5000);
    waterPumpSystem(soilMoisturePercent3, PUMP_PIN3);
    delay(5000);
    waterPumpSystem(soilMoisturePercent3, PUMP_PIN4);
    delay(5000);
    waterPumpSystem(soilMoisturePercent3, PUMP_PIN5);
    delay(5000);
    // waterPumpSystem(soilMoisturePercent3, PUMP_PIN6);
    // delay(5000);
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

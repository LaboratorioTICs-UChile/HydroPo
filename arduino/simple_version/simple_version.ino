/* Servidor y WiFi */
#include "UbidotsMicroESP8266.h"
#define TOKEN  "A1E-Lsc6lPvU8HqOflsasPNwYWNeaAJlNF"  // Put here your Ubidots TOKEN
#define TEMPERATURA1 "59de34ccc03f97323a08732c"
#define TEMPERATURA2 "59e0d730c03f9712d0f6011d"
#define HUMEDAD "59dd42d6c03f9718ebe1682c"
#define HUM_SUELO "59de4024c03f973fd864f643"
#define LUZ "59de34e7c03f9732af48fb5d"

#define WIFISSID "Pulpito"
#define PASSWORD "t3l3c0av"

Ubidots client(TOKEN);

/* Sensores */
#include "DHT.h"
#define DHTPIN 14     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

#include "Wire.h"
#define TMP102 72
#define MINION 9

#define PIN_HUM 0
#define PIN_FOTOR A0

float humedad, temperatura1, temperatura2;
int hum_suelo, luz, ruido;

int getMinionAnalog(int x);

void setup() {
  pinMode(DHTPIN, INPUT);
  pinMode(PIN_HUM, INPUT);
  pinMode(PIN_FOTOR, INPUT);

  Serial.begin(115200);
  delay(10);
  client.wifiConnection(WIFISSID, PASSWORD);

  dht.begin();
  Wire.begin();
}

void loop() {
  // SENSOR HUMEDAD Y TEMPERATURA DHT11
  humedad = dht.readHumidity();
  temperatura1 = dht.readTemperature();

  // SENSOR TEMPRATURA TMP102
  temperatura2 = getTemp102();
  
  // SENSOR HUMEDAD DEL SUELO YL69
  hum_suelo = digitalRead(PIN_HUM);
  
  // FOTORESISTENCIA
  luz = analogRead(PIN_FOTOR);

  // RUIDO
  ruido = getMinionAnalog(2);
  
  // ENVIAR INFORMACION
  //  Serial.println("Temperatura | Humedad | Hum.Suelo | Luz");
  //  Serial.print(temperatura1); Serial.print("       | ");
  //  Serial.print(humedad);   Serial.print("   | ");
  //  Serial.print(hum_suelo); Serial.print("         | ");
  //  Serial.println(luz); Serial.println("");

  //digitalWrite(LED_BUILTIN,HIGH); delay(500); digitalWrite(LED_BUILTIN,LOW);

  Serial.println("");Serial.print("ruido: "); Serial.println(ruido);Serial.println("");

  client.add(TEMPERATURA1, temperatura1);
  client.add(TEMPERATURA2, temperatura2);
  client.add(HUMEDAD, humedad);
  client.add(HUM_SUELO, hum_suelo);
  client.add(LUZ, luz);
  client.sendAll(false);
  
  delay(5000);
}

float getTemp102(){
  byte firstbyte, secondbyte; //these are the bytes we read from the TMP102 temperature registers
  int val;
  float convertedtemp;
 
  Wire.beginTransmission(TMP102); //Say hi to the sensor.
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(TMP102, 2);
  Wire.endTransmission();
 
 
  firstbyte      = (Wire.read());
  secondbyte     = (Wire.read());
  
  val = ((firstbyte) << 4);
  val |= (secondbyte >> 4);
  convertedtemp = val*0.0625;

  return convertedtemp;
}

int getMinionAnalog(int x /*hex - 0x00*/) {
  byte firstbyte, secondbyte;
  int val;
 
  Wire.beginTransmission(MINION);
  Wire.write(x); // hex - 0x00
  Wire.endTransmission();
  Wire.requestFrom(MINION, 2);
  Wire.endTransmission();
 
 
  firstbyte      = (Wire.read());
  secondbyte     = (Wire.read());
  
  val = ((firstbyte) << 4);
  val |= (secondbyte >> 4);

  Serial.print("Analog "); Serial.print(x); Serial.print(": "); Serial.println(val);

  return val;
}




#include "DHT.h"                 //libraria pentru senzorul de temepratura si umiditate atmosferica DHT11

#define DHTPIN  A0              //pinul de pe Arduino Mega care va citi valoarea de pe senzorul DHT
#define DHTTYPE DHT11           //tipul de senzor DHT pe care il folosesc e DHT11

#define sensorPower 14          //folosesc acest pin pentru a alimenta senzorul de umiditate numai cand facem o citire

#define soilWet 500             // Define max value we consider soil 'wet'
#define soilDry 750

int moisturePin = A1;           //pinul de pe Arduino Mega care va citi valoarea de pe senzorul de umiditate sol
int sensorValue = 0;            //valoare propiu-zisa citita de senzorul de umiditate
int percentM = 0;               //valoarea citita de la senzorul de umiditate dupa maaparea acestuia

#define lightSensorPin A3       //pinul digital de pe care vom citi valoarea intensitatii luminoase
int luminositySensorValue = 0; 

//pentru a afisa pe led-uri am folosit led RGB tricolor cu catod comun => avem nevoie de un pin pentru fiecare culoare
int redPinTemperature=13;
int greenPinTemperature=12;
int bluePinTemperature=11;


int redPinAirHumidity=10;
int greenPinAirHumidity=9;
int bluePinAirHumidity=8;

int redPinSoilMoisture=7;
int greenPinSoilMoisture=6;
int bluePinSoilMoisture=5;

int redPinLight=4;
int greenPinLight=3;
int bluePinLight=2;


float temperatureMAX=22.00;
float humidityMAX=50.00;
float luminosityMAX=32.00;
float moistureMAX=120.00;

//creez un obiect de tipul DHT
//DHT- e o clasa din libraria "DHT.h"
DHT dht(DHTPIN,DHTTYPE);

String str;
String typedTemperature = "";  
String typedHumidity = "";  
String typedMoisture = "";  
String  typedLuminosity= "";  

String readString="";




void setup() {
  pinMode(sensorPower, OUTPUT);
  pinMode(lightSensorPin,INPUT); 
	// Initial pastrez senzorul oprit 
	digitalWrite(sensorPower, LOW);
  Serial.begin(115200); 
  Serial1.begin(115200);
  dht.begin();
  delay(500); 
  delay(1000);
}

void loop() {
  
  float temperature=dht.readTemperature();
  float humidity=dht.readHumidity();
  //verificam daca temperatura si umiditatea sunt de tipul float sau nu; daca nu inseamna ca nu s-a citi corect si trebuie sa transmitem un mesaj
  if (isnan(temperature) || isnan(humidity))
  {
    //Serial.println("DHT failed to read temperature and humidity.....Check out your connections!!");
    return;

  }
  //Serial.println("Temperature = "+ String(temperature));
  if(temperature > 20.00 && temperature < temperatureMAX){
      digitalWrite(greenPinTemperature, HIGH);
      digitalWrite(redPinTemperature, LOW);
      digitalWrite(bluePinTemperature, LOW);
  }
  else
  {
      digitalWrite(greenPinTemperature, LOW);
      digitalWrite(redPinTemperature, HIGH);
      digitalWrite(bluePinTemperature, LOW);
  }
  
 // Serial.println("Current humidity = "+ String(humidity));
  if(humidity > 30.00 && humidity< humidityMAX)
  {
      digitalWrite(greenPinAirHumidity, HIGH);
      digitalWrite(redPinAirHumidity, LOW);
      digitalWrite(bluePinAirHumidity, LOW);
  }
  else
  {
      digitalWrite(greenPinAirHumidity, LOW);
      digitalWrite(redPinAirHumidity, HIGH);
      digitalWrite(bluePinAirHumidity, LOW);
  }
  
  sensorValue = readSensor();
  percentM = map(sensorValue, 1017, 244, 0 , 100);
	//Serial.println("soil moisture =  "+String(percentM));

	// Determin statusul umiditatii solului: in functie de valorile definite este prea uscat, prea umed , sau in stare perfecta
	if (sensorValue < moistureMAX) {
		//Serial.println("Status: Soil is too wet");
    digitalWrite(greenPinSoilMoisture, LOW);
    digitalWrite(redPinSoilMoisture, LOW);
    digitalWrite(bluePinSoilMoisture, HIGH);

	} else if (sensorValue >= moistureMAX && sensorValue < soilDry) {
		//Serial.println("Status: Soil moisture is perfect");
    digitalWrite(greenPinSoilMoisture, HIGH);
    digitalWrite(redPinSoilMoisture, LOW);
    digitalWrite(bluePinSoilMoisture, LOW);
	} else {
		//Serial.println("Status: Soil is too dry - time to water!");
    digitalWrite(greenPinSoilMoisture, LOW);
    digitalWrite(redPinSoilMoisture, HIGH);
    digitalWrite(bluePinSoilMoisture, LOW);
	}




  luminositySensorValue=analogRead(lightSensorPin); //assign value of LDR sensor to a temporary variable
 // Serial.println("Light = "+String(luminositySensorValue));
  int light=luminositySensorValue;
  if(luminositySensorValue>luminosityMAX) { //HIGH means,light got blocked
      digitalWrite(redPinLight,HIGH); 
      digitalWrite(greenPinLight, LOW);
      digitalWrite(bluePinLight, LOW);
 }
 else
 {
      digitalWrite(redPinLight,LOW); 
      digitalWrite(greenPinLight, HIGH);
      digitalWrite(bluePinLight, LOW);
 }


      //trimit datele s=serial la ESP
      str = String(temperature) + String(humidity) + String(percentM) + String(luminositySensorValue);
      Serial1.println(str);
      
    
    //asteptam 10 secunde pana sa reaccesam senzorii si sa recitim datele
    delay(10000);


/*
    Serial.println("Enter the desired temperature:"); 
    while (Serial.available() == 0) {}  
    typedTemperature = Serial.readString();  
    Serial.println("Enter the desired humidity:");  
    while (Serial.available() == 0) {}  
    typedHumidity = Serial.readString();  
    Serial.println("Enter the desired moisture level:");  
    while (Serial.available() == 0) {}  
    typedMoisture = Serial.readString();  
    Serial.println("Enter the desired luminosity:");  
    while (Serial.available() == 0) {}  
    typedLuminosity = Serial.readString();  
    Serial.println("-------------------------"); //Showing the details  
    Serial.println("READ TEMPERATURE:" + String(temperature)); 
    Serial.println("NEW TEMPERATURE:" + typedTemperature);  
    Serial.println("READ HUMIDITY:" + String(humidity));  
    Serial.println("NEW HUMIDITY:" + typedHumidity); 
    Serial.println("READ MOISTURE:" + String(percentM)); 
    Serial.println("NEW MOISTURE:" + typedMoisture);  
    Serial.println("READ LUMINOSITY: " + String(luminositySensorValue));
    Serial.println("NEW LUMINOSITY: " + typedLuminosity);  
    Serial.println("Waiting for the system to proceed the changes..... ");  
    Serial.println("");  

     //  Oentru grafice in processing:
     // Serial.print(temperature);
    //Serial.print(".");
    //Serial.write(percentM);
    */
   /* while (Serial1.available() == 0) {} 

    if (Serial1.available()>0)
    {
        // read the incoming string:
         String incomingString = Serial1.readString();
   
        
    }*/

    if (Serial1.available())  {
      String c = Serial1.readStringUntil("\n"); 
      int ind = c.indexOf(':');  
      String val = c.substring(0, ind);  
      String val2 = c.substring(ind+1, c.length());
      if(val=="temperature")
      {
         temperatureMAX=val2.toFloat();
      }
      else if (val=="humidity")
      {
          humidityMAX=val2.toFloat();
      }
      else if (val=="moisture")
      {
        moistureMAX=val2.toFloat();
      }
      else if (val=="luminosity")
      {
        luminosityMAX=val2.toFloat();
      }
      Serial.println(c);
      Serial.println(val);
      Serial.println(val2);
      c="";
      val="";
      val2="";
    }
     
}



int readSensor() {
	digitalWrite(sensorPower, HIGH);      	 // Turn the sensor ON
	delay(10);							                 // Allow power to settle
	int val = analogRead(moisturePin);	     // Read the analog value form sensor
	digitalWrite(sensorPower, LOW);		       // Turn the sensor OFF
	return val;							                // Return analog moisture value
}

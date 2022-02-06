#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

char* ssid="RazvanPhone";
char* password="asdzxcqwe";

String data = " "; //intreaga informatie transmisa de placa Arduino catre modul
String t = " "; //temperatura
String h = " "; //umiditate aer
String hs = " "; //umiditate sol
String l= " "; //luminozitate
char c;


//valori pe care le voi citi din text-boxurile webpage-ului
const char* PARAM_INPUT_1 = "temperature";
const char* PARAM_INPUT_2 = "humidity";
const char* PARAM_INPUT_3 = "moisture";
const char* PARAM_INPUT_4 = "luminosity";

String inputParameterToSend="";
String inputValueToSend=" ";


AsyncWebServer server(80);


//codul paginii web
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<!-- viewport is the user's visible area of a web page:width=device-width part sets the width of the page to follow the screen-width of the device -->
<!-- initial-scale=1.0 part sets the initial zoom level when the page is first loaded by the browser -->

<meta name="viewport" content="width=device-width, initial-scale=1">


<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
<style>
html {
font-family: Garamond ;
display: inline;
margin: 10px auto;
font-weight: bold;


background-image: url("bg.png");
background-size: cover;
background-position: center;
text-align: left;
margin-left: 60px;
}
h1 { font-size: 3.2rem;
  color:#00673a;
 }

p { font-size: 1.3rem; }

form{ font-size: 1.3rem;
  color: #cc0000;}
.units { font-size: 1.0rem; }

.labels{
font-size: 1.5rem;
vertical-align:middle;
padding-bottom: 15px;

}


.button {
  border: none;
  color: #00673a;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 15px;
  margin: 4px 2px;
  cursor: pointer;
}



</style>
</head>
<body>
<h1>Check your greenhouse parameters here:</h1>
<br>
</br>
<i class="fas fa-thermometer-half"style="font-size:28px;color:#bd0000;"></i>
<span class="labels"> Temperature: </span>
<span id="temperature"> %TEMPERATURE% </span>
<br>
</br>
<form action="/get">
    &emsp; Desired temperature: <input type="text" name="temperature" size="5">
    <!-- <button class="button buttonTemperatura">Submit</button> -->
    <input type="submit" value="Submit">
</form>

</p>


<p>
<i class="fas fa-tint" style="font-size:28px;color:#78d4ff;"></i>
<span class="labels">  Humidity: </span>
<span id="humidity"> %HUMIDITY% </span>

<form action="/get">

    &emsp; Desired humidity level: <input type="text" name="humidity" size="5">
    <!-- <button class="button buttonUmiditate">Submit</button> -->
    <input type="submit" value="Submit">
</form>
</p>


<p>
<i class="fas fa-seedling" style="font-size:28px;color:#078209;"></i>
<span class="labels">  Soil moisture: </span>
<span id="moisture">%MOISTURE%</span>

<form action="/get">
    &emsp; Desired soil moisture level <input type="text" name="moisture" size="5">
    <!-- <button class="button buttonSoilMoisture">Submit</button> -->
    <input type="submit" value="Submit">
</form>



</p>
<p>
<i class="far fa-lightbulb" style="color:#faff78;"></i>
<span class="labels">  Lumisnosity: </span>
<span id="luminosity">%LUMINOSITY%</span>
<form action="/get">
    &emsp; Desired luminosity level <input type="text" name="luminosity" size="5">
    <!-- <button class="button buttonLuminozitate">Submit</button> -->
    <input type="submit" value="Submit">
</form>
</p>

</body>
</html>)rawliteral";


String processor(const String& var){
if(var == "TEMPERATURE"){
return String(t);
}
else if(var == "HUMIDITY"){
return String(h);
}
else if(var == "MOISTURE"){
return String(hs);
}
else if(var == "LUMINOSITY"){
return String(l);
}
return String();
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup(){

Serial.begin(115200);
// Connect to Wi-Fi
WiFi.enableInsecureWEP(true); 
WiFi.begin(ssid, password);
Serial.println("Connecting to WiFi");
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.println(".");
}

// Print ESP8266 Local IP Address
Serial.println(WiFi.localIP());


server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
request->send_P(200, "text/html", index_html, processor);
});
server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
request->send_P(200, "text/plain", String(t).c_str());
});
server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
request->send_P(200, "text/plain", String(h).c_str());
});
server.on("/moisture", HTTP_GET, [](AsyncWebServerRequest *request){
request->send_P(200, "text/plain", String(hs).c_str());
});
server.on("/luminosity", HTTP_GET, [](AsyncWebServerRequest *request){
request->send_P(200, "text/plain", String(l).c_str());
});


  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      //Serial.println("I have to send to the Arduino the parameter "+inputParam+" with the value of "+inputMessage + "!");
      Serial.println(inputParam+":"+inputMessage);
      //Serial1.println(inputParam+" "+inputMessage);
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
     Serial.println(inputParam+":"+inputMessage);
     //Serial1.println(inputParam+" "+inputMessage);
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
      Serial.println(inputParam+":"+inputMessage);
      //Serial1.println(inputParam+" "+inputMessage);
    }
    else if (request->hasParam(PARAM_INPUT_4)) {
      inputMessage = request->getParam(PARAM_INPUT_4)->value();
      inputParam = PARAM_INPUT_4;
      Serial.println(inputParam+":"+inputMessage);
      //Serial1.println(inputParam+" "+inputMessage);
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    //Serial.println(inputMessage);
    request->send(200, "text/html", "You have sent to the system for the parameter: " 
                                     + inputParam + " the value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
   
  });
  server.onNotFound(notFound);


// Start server
server.begin();
}

//cod program
void loop() {

  //avem cel putin un caracter in buffer-ul serial
  if(Serial.available()){
    //asteptam sa intre mai multe caractere in buffer  
    delay(50);
    //curatam ce s-a stocat la ultimul update in string-ul in care preluam informatia din buffer
    t = " ";
    h = " ";
    hs = " ";
    l= " ";
    data = " ";
    //preluam datele pe care placa Arduino le transmite serial
    while(Serial.available()){
      c = Serial.read();
      data.concat(c);
      t = data.substring(1,6);
      h = data.substring(6,11);
      hs = data.substring(11,12);
      l=data.substring(12,15);
    }
    //Serial.println("Temperature:"+String(t));
    //Serial.println("Humidity:"+String(h));
    //Serial.println("Moisture:"+String(hs));
    //Serial.println("Luminosity:"+String(l));
    
    
  }
 
}

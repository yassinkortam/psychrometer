
/************************** Configuration ***********************************/
//#include "config.h"
#include "psychrometry.h"

/************************ Display Imports & setup ***********************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/************************ Bluetooth ***********************************/
#include "BluetoothSerial.h"

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

#define BUTTON_A 15
#define BUTTON_B 32
#define BUTTON_C 14

/************************ Example Starts Here *******************************/
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// pin connected to DH22 data line
#define DATA_PIN 32

//Create bluetooth object
BluetoothSerial SerialBT;

// create DHT22 instance
DHT dht(DATA_PIN, DHT22);

// set up the 'temperature' and 'humidity' feeds
/*
AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *humidity = io.feed("humidity");
AdafruitIO_Feed *pws1 = io.feed("Pws");
AdafruitIO_Feed *w1 = io.feed("W");
AdafruitIO_Feed *ws1 = io.feed("Ws");
AdafruitIO_Feed *v1 = io.feed("V");
AdafruitIO_Feed *p1 = io.feed("P");
AdafruitIO_Feed *h1 = io.feed("H");
AdafruitIO_Feed *td1 = io.feed("Td");
AdafruitIO_Feed *twb1 = io.feed("Twb");
*/

long pair_time = 0;

void setDisplay(double font_size){

  display.clearDisplay();
  display.display();

  // text display tests
  display.setTextSize(font_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

}

void setup() {

  // start the serial connection
  Serial.begin(9600);

  //Start bluetooth with given device name
  SerialBT.begin("ESP32");

  // wait for serial monitor to open
  while(! Serial);

  //startup Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(1000);

  Serial.println("OLED FeatherWing test");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  Serial.println("OLED begun");
  SerialBT.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  // initialize dht22
  dht.begin();

//**********************ADAFRUIT IO**************************
/*
  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  SerialBT.print("Connecting to Adafruit IO");
  setDisplay(2);
  display.print("Connecting");
  display.display();

  io.connect();

    // wait for a connection
  long start = millis(), finish;
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    SerialBT.print(".");
    display.print(".");
    display.display();
    delay(500);
    finish = millis();
    pair_time = finish - start;
    if (pair_time > 4000){
      break;
    }
  }
  if(io.status() < AIO_CONNECTED){
    setDisplay(2);

    Serial.print("No WiFi");
    SerialBT.print("No WiFi");

    display.print("No WiFi");
    display.display();
    delay(2000);
  }
  else{
    setDisplay(2);

    Serial.print("Connected");
    SerialBT.print("Connected");

    display.print("Connected");
    display.display();
    delay(2000);
  }
*/
}

void rst(){
  //Display Reset
  setDisplay(2);
  if(!digitalRead(BUTTON_C)){
    display.print("Sleeping...");
    display.display();
    delay(2000);
    setDisplay(2);
    while (true){
        if(!digitalRead(BUTTON_C)){
          setDisplay(2);
          display.print("Waking...");
          display.display();
          delay(2000);
          break;
        }
    }
  }
}
void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.

  if (pair_time < 4000){
    //io.run();
  }

  sensors_event_t event;

  float celsius = dht.readTemperature();
  float percent = dht.readHumidity();
  float fahrenheit = dht.readTemperature(true);
  double temp = celsius, 
          phi = percent*0.01,
          pws_val = pws(temp),
          w_val = w(temp, phi),
          ws_val = ws(temp, phi),
          v_val = v(temp, phi),
          p_val = p(temp, phi),
          h_val = h(temp, phi),
          td_val = td(temp, phi),
          twb_val = twb(temp, phi);

  double values[10] = {temp, phi, pws_val, w_val, ws_val, v_val, p_val, h_val, td_val, twb_val};
  string labels[10] = {"Tmp", "Hum", "Pws", "W", "Ws", "V", "P", "H", "Td", "Twb"};

  //things to display
for (int i=0; i<10; i++){
  rst();
  setDisplay(2);
  display.print(labels[i].c_str());
  display.print(": ");
  display.print(values[i]);
  display.display();


  SerialBT.print(labels[i].c_str());
  SerialBT.print(": ");
  SerialBT.print(values[i]);
  SerialBT.println();
  delay(2000);
}

  // save fahrenheit (or celsius) to Adafruit IO
  if (pair_time < 4000){
    /*
    temperature->save(fahrenheit);
    humidity->save(event.relative_humidity);
    pws1->save(pws_val);
    w1->save(w_val);
    ws1->save(ws_val);
    v1->save(v_val);
    p1->save(p_val);
    h1->save(h_val);
    td1->save(td_val);
    twb1->save(twb_val);
    */
  }

}
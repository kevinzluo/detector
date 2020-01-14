int tempPin = 0;
//                BS E  D4 D5  D6  D7
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int index = 0;
double breakTemp1 = 27.00;
double breakTemp2 = 30.00;

int state = 0;
//                R  G  B
int ledPorts[] = {3, 2, 4};


int LED_RED = ledPorts[0];
int LED_GREEN = ledPorts[1];
int LED_BLUE = ledPorts[2];
  
void setup()
{
  //lcd.begin(16, 2);
  Serial.begin(9600);

  for (int pin : ledPorts) {
    //pinMode(pin, OUTPUT);
  }
}

void loop()
{


  int tempReading = analogRead(tempPin); //Analog between 0 and 1024
  /*
     Let r_t thermister resistance, r_0 be resistor resistance
         V_t thermister potential drop, V_0 be total voltage (5V)
     Then V_t = r_t / (r_t + r_0) * V_0
     To convert analog reading to voltage, we have
     V_0 - V_t = analogRead / 1024 * V_0
     or
     r_0 / (r_t + r_0) * V_0 = analogRead / 1024 * V_0
     r_0 / (r_t + r_0) = analogRead / 1024
     (r_t + r_0) / r_0 = 1024 / analogRead
     1 + r_t / r_0 = 1024 / analogRead
     r_t / r_0 = 1024 / analogRead - 1
     r_t = r_0 * (1024 / analogRead - 1)

  */
  double resistance = 10000.0 * ((1024.0 / tempReading - 1));
  // Steinhart-Hart: 1/T = A + B ln R + C (ln R)^3
  //                 T = 1 / (A + (B + (C (ln R)^2)) * ln R)
  double lnR = log(resistance);
  double tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * lnR * lnR )) * lnR ); // Steinhart-Hart

  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0) / 5.0 + 32.0; // Convert Celcius to Fahrenheit

  // Display Temperature in C
  Serial.print(String(index) + "\t(analog): " + String(tempReading) + "\t");
  Serial.println(String(index) + "(tempC): " + String(tempC));

  
  digitalWrite(LED_RED, indRed(tempC));
  digitalWrite(LED_GREEN, indGreen(tempC));
  digitalWrite(LED_BLUE, indBlue(tempC));
  

  Serial.println("\tRed: " + String(indRed(tempC)) + "\tGreen: " + String(indGreen(tempC)) + "\tBlue: " + String(indBlue(tempC)));
  /*
   * red < breakTemp1 <= green < breakTemp2 <= blue
    
  */
  delay(1000);
  index++;
}


int indRed(double temp) {
  double val = -(temp - breakTemp1);
  return getSign(val);
}

int indGreen(double temp) {
  double val = -(temp - breakTemp1) * (temp - breakTemp2);
  return getSign(val);
}

int indBlue(double temp) {
  double val = temp - breakTemp2;
  return getSign(val);
}


int getSign(double val) {
  if(val >= 0) {
    return 1;
  } else {
    return 0;
  }
}

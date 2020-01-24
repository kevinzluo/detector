//#include <LiquidCrystal.h>

int tempPin = 0;
int index = 0;
double breakTemp1 = 0.50;
double breakTemp2 = 2.00;


//                BS E  D4 D5  D6  D7
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//int state = 0;
//                R  G  B
int ledPorts[] = {2, 3, 4};


int LED_RED = ledPorts[0];
int LED_GREEN = ledPorts[1];
int LED_BLUE = ledPorts[2];


double analogVoltageToTemp(int analogVoltage, double A = 0.001167419, double B = 0.000226582, double C = 0.000000142456334686082) {
  /* V_t calculation
     Let r_t thermister resistance, r_0 be resistor resistance
         V_t thermister potential drop, V_0 be total voltage (5V)
     Then V_t = r_t / (r_t + r_0) * V_0
     To convert analog reading to voltage, we have
     V_0 - V_t = analogVoltage / 1024 * V_0
     V_t = V_0 ( 1 - analogVoltage / 1024 )

     Old:
       double resistance = 10000.0 * ((1024.0 / tempReading - 1));
       double lnR = log(resistance);
       double tempK = 1 / (A + (B + (C * lnR * lnR )) * lnR ); // Steinhart-Hart

  */
  double volt = 5.0 * (1 - analogVoltage / 1024.0);
  /* Steinhart-Hart 
   * 1/T = A + B ln R + C (ln R)^3
   * T = 1 / (A + B ln R + C (ln R)^3)
   * T = 1 / (A + (B + C (ln R)^2) * ln R)
   */

  /* V_t to r_t:
   * V_t = r_t / (r_t + r_0) * V_0
   * V_t / V_0 = r_t / (r_t + r_0)
   * V_0 / V_t = 1 + r_0 / r_t
   * r_0 / r_t = V_0 / V_t - 1
   * r_t / r_0 = 1/(V_0 / V_t - 1)
   * r_t = r_0 / (V_0 / V_t - 1)
   *
   * In 2 steps:
   * double res = 10000.0 / (5.0 / voltage - 1);
   * double temperature = 1 / ( A + B * log(res) + C * pow(res, 3) );
   */
  double temp = 1 / ( A + B * log(10000.0 / (5.0 / volt - 1)) + C * pow( log(10000 / ( 5.0 / volt - 1 ) ), 3) );
 
  return temp;
}


int indRed(double temp) {
  if (temp < breakTemp1) {
    return 1;
  }
  return 0;
}

int indGreen(double temp) {
  if(breakTemp1 <= temp && temp < breakTemp2) {
    return 1;
  }
  return 0;
}

int indBlue(double temp) {
  if (breakTemp2 < temp) {
    return 1;
  }
  return 0;
}

void setup()
{
  //lcd.begin(16, 2);
  Serial.begin(9600);

  for (int pin : ledPorts) {
    pinMode(pin, OUTPUT);
  }

  
}

void loop()
{


  int tempReading = analogRead(tempPin); // Analog between 0 and 1024

  // Steinhart-Hart: 1/T = A + B ln R + C (ln R)^3
  //                 T = 1 / (A + (B + (C (ln R)^2)) * ln R)
  
  //double res = 10000.0 * ((1024.0 / tempReading - 1));
  //double lnR = log(resistance);
  //double tempK1 = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * lnR * lnR )) * lnR );   
  double tempK1 = analogVoltageToTemp(tempReading, 0.001229645, 0.000214748, 0.000000191714874203215);
  double tempK2 = analogVoltageToTemp(tempReading);
  float tempC1 = tempK1 - 273.15;             // Convert Kelvin to Celcius
  float tempC2 = tempK2 - 273.15;
  //float tempF = (tempC * 9.0) / 5.0 + 32.0; // Convert Celcius to Fahrenheit

  // Display Temperature in C
  Serial.println("----------------------------------------------------");
  Serial.println(String(index) + "\tanalog: " + String(tempReading) + "\t");
  Serial.println("\ttempC1: " + String(tempC1) + "\t" + "tempC2:" + String(tempC2));
  
  digitalWrite(LED_RED, indRed(tempC2));
  digitalWrite(LED_GREEN, indGreen(tempC2));
  digitalWrite(LED_BLUE, indBlue(tempC2));
 

  Serial.println("\tRed: " + String(indRed(tempC2)) + "\tGreen: " + String(indGreen(tempC2)) + "\tBlue: " + String(indBlue(tempC2)));
  /*
    lcd.setCursor(0,0);
    lcd.print("Temp        C");
    lcd.setCursor(6,0);
    lcd.print(tempC1);
  */
  delay(1000);
  index++;
}

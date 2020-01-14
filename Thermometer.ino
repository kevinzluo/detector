//#include <LiquidCrystal.h>
int tempPin = 0;
//                BS E  D4 D5  D6  D7
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int index = 0;
void setup()
{
  //lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop()
{
  int tempReading = analogRead(tempPin); //Analog between 0 and 1024
  /*
   * Let r_t thermister resistance, r_0 be resistor resistance
   *     V_t thermister potential drop, V_0 be total voltage (5V)
   * Then V_t = r_t / (r_t + r_0) * V_0
   * To convert analog reading to voltage, we have
   * V_0 - V_t = analogRead / 1024 * V_0
   * or
   * r_0 / (r_t + r_0) * V_0 = analogRead / 1024 * V_0 
   * r_0 / (r_t + r_0) = analogRead / 1024
   * (r_t + r_0) / r_0 = 1024 / analogRead
   * 1 + r_t / r_0 = 1024 / analogRead
   * r_t / r_0 = 1024 / analogRead - 1
   * r_t = r_0 * (1024 / analogRead - 1) 
   * 
   */
  double resistance = 10000.0 * ((1024.0 / tempReading - 1)); 
  // Steinhart-Hart: 1/T = A + B ln R + C (ln R)^3
  // T = 1 / (A + (B + (C (ln R)^2)) * ln R)
  double lnR = log(resistance);
  double tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * lnR * lnR )) * lnR ); // Steinhart-Hart
  
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0) / 5.0 + 32.0; // Convert Celcius to Fahrenheit
  /*  replaced
    float tempVolts = tempReading * 5.0 / 1024.0;
    float tempC = (tempVolts - 0.5) * 10.0;
    float tempF = tempC * 9.0 / 5.0 + 32.0;
  */
  // Display Temperature in C
  Serial.print(String(index) + "(AnalogRead): " + String(tempReading) + "\t");
  Serial.println(String(index) + "(converted temp): " + String(tempC));
  //lcd.setCursor(0, 0);
  //lcd.print("Temp         C  ");
  //lcd.setCursor(6, 0);
  //lcd.print(tempC);
  //lcd.print("Temp         F  ");
  //lcd.print(tempF);


  
  delay(1000);
  index++;
}

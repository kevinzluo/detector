int tempPin = 0;
int index = 0;
double breakTemp1 = 0.50;
double breakTemp2 = 2.00;

//                R  G  B
int ledPorts[] = {2, 3, 4};

int LED_RED = ledPorts[0];
int LED_GREEN = ledPorts[1];
int LED_BLUE = ledPorts[2];

double analogVoltageToTemp(int analogVoltage, double A = 0.001229645, double B = 0.000214748, double C = 0.000000191714874203215) {
  /* 
   * Let r_t thermister resistance, r_0 be resistor resistance
   *     V_t thermister potential drop, V_0 be total voltage (5V)
   * Then V_t = r_t / (r_t + r_0) * V_0
   * To convert analog reading to voltage, we have
   * V_0 - V_t = analogRead / 1024 * V_0
   * Combining:
   * V_0 - r_t / (r_t + r_0) * V_0 = analogRead / 1024 * V_0
   * r_0 / (r_t + r_0) * V_0 = analogRead / 1024 * V_0
   * r_0 / (r_t + r_0) = analogRead / 1024
   * (r_t + r_0) / r_0 = 1024 / analogRead
   * 1 + r_t / r_0 = 1024 / analogRead
   * r_t / r_0 = 1024 / analogRead - 1
   * r_t = r_0 * (1024 / analogRead - 1)
   Steinhart-Hart 
   * 1/T = A + B ln R + C (ln R)^3
   * T = 1 / (A + B ln R + C (ln R)^3)
   * T = 1 / (A + (B + C (ln R)^2) * ln R)
   * 
   * In 2 steps:
   * double res = 10000.0 * (1024.0 / analogVoltage - 1);
   * double temperature = 1 / ( A + B * log(res) + C * pow(res, 3) );
   */
  double temp = 1 / ( A + B * log( 10000.0 * (1024.0 / analogVoltage - 1) ) + C * pow( log( 10000.0 * (1024.0 / analogVoltage - 1) ), 3) );
  
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
  Serial.begin(9600);

  for (int pin : ledPorts) {
    pinMode(pin, OUTPUT);
  }

}

void loop()
{

  int tempReading = analogRead(tempPin); // Analog between 0 and 1024
 
  double tempK = analogVoltageToTemp(tempReading);
  float tempC = tempK - 273.15;             // Convert Kelvin to Celcius

  // Display Temperature in C
  Serial.println("----------------------------------------------------");
  Serial.println(String(index) + "\tanalog: " + String(tempReading) + "\t");
  Serial.println("\ttempC: " + String(tempC)));
  
  digitalWrite(LED_RED, indRed(tempC));
  digitalWrite(LED_GREEN, indGreen(tempC));
  digitalWrite(LED_BLUE, indBlue(tempC));
 
  Serial.println("\t\tRed: " + String(indRed(tempC)) + "\t\tGreen: " + String(indGreen(tempC)) + "\t\tBlue: " + String(indBlue(tempC)));

  delay(500);
  index++;
}

int tempPin = 0;
//                BS E  D4 D5  D6  D7
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int index = 0;
double breakTemp1 = 25.00;
double breakTemp2 = 27.00;

int state = 0;
//                R  G  B
int ledPorts[] = {2, 3, 4};


int LED_RED = ledPorts[0];
int LED_GREEN = ledPorts[1];
int LED_BLUE = ledPorts[2];
  
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
  Serial.print(String(index) + "(AnalogRead): " + String(tempReading) + "\t");
  Serial.println(String(index) + "(converted temp): " + String(tempC));

  /*
   * 1 < breakTemp1 <= 2 < breakTemp2 <= 3
    if(tempC < breakTemp1) {
    changeState(1);
    } else if (tempC >= breakTemp2) {
    changeState(3);
    } else {
    changeState(2);
    }
  */
  delay(1000);
  index++;
}

void changeState(int newState) {
  //reset lights
  //Serial.println(state);
  if (newState = state) {
    return;
  }

  state = newState;

  // reset the lights
  for (int pin : ledPorts) {
    digitalWrite(pin, LOW);
  }

  // light up the right light
  if (state == 1) {
    digitalWrite(LED_RED, HIGH);
  } else if (state == 2) {
    digitalWrite(LED_GREEN, HIGH);
  } else {
    digitalWrite(LED_BLUE, HIGH);
  }

  /*
     TODO:
     Maybe set up lighting ranges individually? like with an indicator function; pretty easy with polynomial zeroes
     main issue is with maintaining the condition of the lights; idk anymore
  */
}

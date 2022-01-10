/*

   Board : Arduino Pro or Uno Pro mini, ATmega328 (3.3V, 8MHz)
           ATtiny13 (internal 9.6 MHz)

   Memo :
               ATtiny13A
                +---+
   ADC0/D5/PB5 1|   |8 VCC
   ADC3/D3/PB3 2|   |7 PB2/D2/ADC1
   ADC2/D4/PB4 3|   |6 PB1/D1/PWM1
           GND 4|   |5 PB0/D0/PWM0
                +---+

            SN74HC4066N
                +---+
            1A 1|   |14 VCC
            1B 2|   |13 1C
            2B 3|   |12 4C
            2A 4|   |11 4A
            2C 5|   |10 4B
            3C 6|   |9  3B
           GND 7|   |8  3A
                +---+

*/

//#define DEBUG

// Macro for ATtiny13 (internal 9.6 MHz clock)
#ifdef ARDUINO_AVR_ATTINY13_8
  #warning ATtiny13
  #include <avr/sleep.h>
  #define    F_CPU    9600000UL
  #include  <util/delay.h>
  #define TimeRate 0.15   // Correct the time by matching with the actual product
  #define Ch1 2  //（PB2, pin7)
  #define Ch2 3  //（PB3, pin2)
  #define Ch3 1  //（PB1, pin6)
  #define Ch4 0  //（PB0, pin5)
  #define In1 4  //（PB4, pin3)
#else // Change the port because the serial port is booked with another port and you can't debug serially.
  #warning ATmega328P
#endif

#define MAX_LOOP 600 // 60s = 600 x 100ms

int Key[4];
int loopNum = MAX_LOOP;
int keyNum = 0;
bool keyPress = true;

void setup() {
#ifdef DEBUG
  #ifndef ARDUINO_AVR_ATTINY13_8
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);
  #endif
#endif

  pinMode(Ch1, OUTPUT);
  pinMode(Ch2, OUTPUT);
  pinMode(Ch3, OUTPUT);
  pinMode(Ch4, OUTPUT);
  pinMode(In1, INPUT_PULLUP);
  digitalWrite(Ch1, LOW);
  digitalWrite(Ch2, LOW);
  digitalWrite(Ch3, LOW);
  digitalWrite(Ch4, LOW);
  
  Key[0]=Ch1;
  Key[1]=Ch2;
  Key[2]=Ch3;
  Key[3]=Ch4;
}

void loop() {
  // When the auxiliary switch is pressed, the ch3 and ch4 buttons are pressed in order.
  if (digitalRead(In1) == LOW){
    digitalWrite(Ch3, HIGH);
    delay(100 * TimeRate);
    digitalWrite(Ch3, LOW);
    delay(500 * TimeRate);
    digitalWrite(Ch4, HIGH);
    delay(100 * TimeRate);
    digitalWrite(Ch4, LOW);
    delay(500 * TimeRate);
  }

  // Press the ch1 and ch2 buttons alternately every 60s.
  if (keyPress) {
    digitalWrite(Key[keyNum], HIGH);
    keyPress = false;
  } else {
    if (loopNum == MAX_LOOP) {
      digitalWrite(Key[keyNum], LOW);
    }
    if (loopNum == 0) {
      keyNum++;
      if (keyNum>1) keyNum=0;
      loopNum = MAX_LOOP;
      keyPress = true;
    } else {
      loopNum--;
    }
  }
  delay(100 * TimeRate);
}

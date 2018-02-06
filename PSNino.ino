//    PSNino v0.1 - a pure, simple Playstation 1 Modchip
//
//    Author: Oliver Koester
//    Special thanks to the ATTYnee and PSNee developers!
//
//--------------------------------------------------
//                  Pinouts
//--------------------------------------------------
//FOR ARDUINO UNO (WITH ATMEGA328):
// - PB0 = Arduino pin 8   = ATMega pin 14
// - PB1 = Arduino pin 9   = ATMega pin 15
// - PB2 = Arduino pin 10  = ATMega pin 16
// - PB3 = Arduino pin 11  = ATMega pin 17
// - PB4 = Arduino pin 12  = ATMega pin 18

//FOR ATTINY13/25/45/85:
// - PB0 = ATTiny pin 5
// - PB1 = ATTiny pin 6
// - PB2 = ATTiny pin 7
// - PB3 = ATTiny pin 2
// - PB4 = ATTiny pin 3


//--------------------------------------------------
//               Chip select
//--------------------------------------------------

#define ATTINY
//#define ARDUINO_BOARD


#ifdef ARDUINO_BOARD
//Pins
#define DATA_PIN 8             //The pin that outputs the SCEE SCEA SCEI string
#define GATE_PIN 9             //The GATE_PIN control
#define DELAY_BETWEEN_BITS 4
#define DELAY_BETWEEN_INJECTIONS 90
#define LED_PIN 13             //Blinks on every single Inject
#endif

#ifdef ATTINY
//Pins
#define DATA_PIN 2            //The pin that outputs the SCEE SCEA SCEI string
#define GATE_PIN 4            //The GATE_PIN control
#define DELAY_BETWEEN_BITS 4
#define DELAY_BETWEEN_INJECTIONS 90
#define LED_PIN 3             //Blinks on every single Inject
#endif

bool readBitFromByteSet(int index, const unsigned char *ByteSet)
{
  int byte_index = index >> 3;
  byte bits = pgm_read_byte(&(ByteSet[byte_index]));
  int bit_index = index & 0x7;
  byte mask = 1 << bit_index;
  return (0 != (bits & mask));
}

void injectSCEx()
{
  static const unsigned char ByteSet[] PROGMEM = {0b01011001, 0b11001001, 0b01001011, 0b01011101, 0b11101010, 0b00000010}; //SCEE
  //static const unsigned char ByteSet[] PROGMEM = {0b01011001, 0b11001001, 0b01001011, 0b01011101, 0b11111010, 0b00000010}; SCEA
  //static const unsigned char ByteSet[] PROGMEM = {0b01011001, 0b11001001, 0b01001011, 0b01011101, 0b11011010, 0b00000010}; //SCEI

  for (unsigned char i = 0; i < 44; i++) {
    if (readBitFromByteSet(i, ByteSet) == 0)
    {
      pinMode(DATA_PIN, OUTPUT);
      digitalWrite(DATA_PIN, 0);
      delay(DELAY_BETWEEN_BITS);
    }
    else
    {
      pinMode(DATA_PIN, INPUT);
      delay(DELAY_BETWEEN_BITS);
    }
  }

  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(DATA_PIN, 0);
}

void setup()
{
  pinMode(DATA_PIN, INPUT);
  pinMode(GATE_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  delay(1200);
  pinMode(GATE_PIN, OUTPUT);
  digitalWrite(GATE_PIN, 0);
  delay(100);

  for (unsigned char i = 0; i < 2; i++)
  {
    digitalWrite(LED_PIN, HIGH);
    injectSCEx();
    digitalWrite(LED_PIN, LOW);
    delay(DELAY_BETWEEN_INJECTIONS);
  }

  pinMode(GATE_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);
}

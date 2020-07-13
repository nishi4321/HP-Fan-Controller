#define portOfPin(P)\
  (((P)>=0&&(P)<8)?&PORTD:(((P)>7&&(P)<14)?&PORTB:&PORTC))
#define ddrOfPin(P)\
  (((P)>=0&&(P)<8)?&DDRD:(((P)>7&&(P)<14)?&DDRB:&DDRC))
#define pinOfPin(P)\
  (((P)>=0&&(P)<8)?&PIND:(((P)>7&&(P)<14)?&PINB:&PINC))
#define pinIndex(P)((uint8_t)(P>13?P-14:P&7))
#define pinMask(P)((uint8_t)(1<<pinIndex(P)))

#define pinAsOutput(P) *(ddrOfPin(P))|=pinMask(P)
#define digitalLow(P) *(portOfPin(P))&=~pinMask(P)
#define digitalHigh(P) *(portOfPin(P))|=pinMask(P)
#define isHigh(P)((*(pinOfPin(P))& pinMask(P))>0)
#define isLow(P)((*(pinOfPin(P))& pinMask(P))==0)
#define digitalState(P)((uint8_t)isHigh(P))

#define fixTime(t)((uint16_t)t*64)

const uint8_t fan_outpin[6] = {11, 10, 9, 6, 5, 3};

// Set of Fan Speed each fans.
static float fan_ratio[6] = {0.20, 0.20, 0.20, 0.20, 0.20, 0.20};

void setup() {
  uint8_t i;
  for (i = 0; i < 6; i++) {
    pinAsOutput(fan_outpin[i]);
  }
  // Set to 31.25kHz to reach 25kHz requirement
  TCCR0B = _BV(CS00);
  TCCR1B = _BV(CS00);
  TCCR2B = _BV(CS00);
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00); 

}

void loop() {
  float out;
  uint8_t pwm_out;
  for (uint8_t i = 0; i < 6; i++) {
    out = fan_ratio[i];
    pwm_out = 255 - out * 255;
    analogWrite(fan_outpin[i], pwm_out);
  }
  delay(fixTime(1000));
}

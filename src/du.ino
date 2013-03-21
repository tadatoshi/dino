#include <Servo.h>
#include <SPI.h>

bool debug = false;

int index = 0;

char messageBuffer[12];
char cmd[3];
char pin[3];
char val[4];
char aux[4];

Servo servo;

void setup() {
  Serial.begin(115200);
}

void loop() {
  while(Serial.available() > 0) {
    char x = Serial.read();
    if (x == '!') index = 0;      // start
    else if (x == '.') process(); // end
    else messageBuffer[index++] = x;
  }
}

/*
 * Deal with a full message and determine function to call
 */
void process() {
  index = 0;

  strncpy(cmd, messageBuffer, 2);
  cmd[2] = '\0';
  strncpy(pin, messageBuffer + 2, 2);
  pin[2] = '\0';

  if (atoi(cmd) > 90) {
    strncpy(val, messageBuffer + 4, 2);
    val[2] = '\0';
    strncpy(aux, messageBuffer + 6, 3);
    aux[3] = '\0';
  } else if (atoi(cmd) < 90 && atoi(cmd) > 80) {
    strncpy(val, messageBuffer + 2, 2);
    val[2] = '\0';
    strncpy(aux, messageBuffer + 4, 3);
    aux[4] = '\0';    
  } else {
    strncpy(val, messageBuffer + 4, 3);
    val[4] = '\0';
    strncpy(aux, messageBuffer + 7, 3);
    aux[4] = '\0';
  }

  if (debug) {
    Serial.println(messageBuffer);
  }
  int cmdid = atoi(cmd);

  // Serial.println(cmd);
  // Serial.println(pin);
  // Serial.println(val);
  // Serial.println(aux);

  switch(cmdid) {
    case 0:  sm(pin,val);              break;
    case 1:  dw(pin,val);              break;
    case 2:  dr(pin,val);              break;
    case 3:  aw(pin,val);              break;
    case 4:  ar(pin,val);              break;
    case 81: handleSpi(val,aux);       break;
    case 97: handlePing(pin,val,aux);  break;
    case 98: handleServo(pin,val,aux); break;
    case 99: toggleDebug(val);         break;
    default:                           break;
  }
}

/*
 * Toggle debug mode
 */
void toggleDebug(char *val) {
  if (atoi(val) == 0) {
    debug = false;
    Serial.println("goodbye");
  } else {
    debug = true;
    Serial.println("hello");
  }
}

/*
 * Set pin mode
 */
void sm(char *pin, char *val) {
  if (debug) Serial.println("sm");
  int p = getPin(pin);
  if(p == -1) { if(debug) Serial.println("badpin"); return; }
  if (atoi(val) == 0) {
    pinMode(p, OUTPUT);
  } else {
    pinMode(p, INPUT);
  }
}

/*
 * Digital write
 */
void dw(char *pin, char *val) {
  if (debug) Serial.println("dw");
  int p = getPin(pin);
  if(p == -1) { if(debug) Serial.println("badpin"); return; }
  pinMode(p, OUTPUT);
  if (atoi(val) == 0) {
    digitalWrite(p, LOW);
  } else {
    digitalWrite(p, HIGH);
  }
}

/*
 * Digital read
 */
void dr(char *pin, char *val) {
  if (debug) Serial.println("dr");
  int p = getPin(pin);
  if(p == -1) { if(debug) Serial.println("badpin"); return; }
  pinMode(p, INPUT);
  int oraw = digitalRead(p);
  char m[7];
  sprintf(m, "%02d::%02d", p,oraw);
  Serial.println(m);
}

/*
 * Analog read
 */
void ar(char *pin, char *val) {
  if(debug) Serial.println("ar");
  int p = getPin(pin);
  if(p == -1) { if(debug) Serial.println("badpin"); return; }
  pinMode(p, INPUT); // don't want to sw
  int rval = analogRead(p);
  char m[8];
  sprintf(m, "%s::%03d", pin, rval);
  Serial.println(m);
}

void aw(char *pin, char *val) {
  if(debug) Serial.println("aw");
  int p = getPin(pin);
  pinMode(p, OUTPUT);
  if(p == -1) { if(debug) Serial.println("badpin"); return; }
  analogWrite(p,atoi(val));
}

int getPin(char *pin) { //Converts to A0-A5, and returns -1 on error
  int ret = -1;
  if(pin[0] == 'A' || pin[0] == 'a') {
    switch(pin[1]) {
      case '0':  ret = A0; break;
      case '1':  ret = A1; break;
      case '2':  ret = A2; break;
      case '3':  ret = A3; break;
      case '4':  ret = A4; break;
      case '5':  ret = A5; break;
      default:             break;
    }
  } else {
    ret = atoi(pin);
    if(ret == 0 && (pin[0] != '0' || pin[1] != '0')) {
      ret = -1;
    }
  }
  return ret;
}

/*
 * Handle Ping commands
 * fire, read
 */
void handlePing(char *pin, char *val, char *aux) {
  if (debug) Serial.println("ss");
  int p = getPin(pin);

  if(p == -1) { if(debug) Serial.println("badpin"); return; }
  Serial.println("got signal");

  // 01(1) Fire and Read
  if (atoi(val) == 1) {
    char m[16];

    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
    delayMicroseconds(2);
    digitalWrite(p, HIGH);
    delayMicroseconds(5);
    digitalWrite(p, LOW);

    Serial.println("ping fired");

    pinMode(p, INPUT);
    sprintf(m, "%s::read::%08d", pin, pulseIn(p, HIGH));
    Serial.println(m);

    delay(50);
  }
}

/*
 * Handle Servo commands
 * attach, detach, write, read, writeMicroseconds, attached
 */
void handleServo(char *pin, char *val, char *aux) {
  if (debug) Serial.println("ss");
  int p = getPin(pin);
  if(p == -1) { if(debug) Serial.println("badpin"); return; }
  Serial.println("signal: servo");

  // 00(0) Detach
  if (atoi(val) == 0) {
    servo.detach();
    char m[12];
    sprintf(m, "%s::detached", pin);
    Serial.println(m);

  // 01(1) Attach
  } else if (atoi(val) == 1) {
    // servo.attach(p, 750, 2250);
    servo.attach(p);
    char m[12];
    sprintf(m, "%s::attached", pin);
    Serial.println(m);

  // 02(2) Write
  } else if (atoi(val) == 2) {
    Serial.println("writing to servo");
    Serial.println(atoi(aux));
    // Write to servo
    servo.write(atoi(aux));
    delay(15);

    // TODO: Experiment with microsecond pulses
    // digitalWrite(pin, HIGH);   // start the pulse
    // delayMicroseconds(pulseWidth);  // pulse width
    // digitalWrite(pin, LOW);    // stop the pulse

  // 03(3) Read
  } else if (atoi(val) == 3) {
    Serial.println("reading servo");
    int sval = servo.read();
    char m[13];
    sprintf(m, "%s::read::%03d", pin, sval);
    Serial.println(m);
  }
}

/*
 * Handle SPI commands
 * begin, end, setBitOrder, setClockDivider, setDataMode, transfer
 */
void handleSpi(char *val, char *aux) {
  if (debug) Serial.println("spi");
  Serial.println("signal: spi");

  // 00(0) Begin
  if (atoi(val) == 0) {
    SPI.begin();
    char m[12];
    sprintf(m, "SPI started");
    Serial.println(m);

  // 01(1) End
  } else if (atoi(val) == 1) {
    SPI.end();
    char m[12];
    sprintf(m, "SPI ended");
    Serial.println(m);

  // 02(2) setBitOrder
  } else if (atoi(val) == 2) {
    int bit_order_identifier = atoi(aux);
    int bit_order;
    switch(bit_order_identifier) {
      case 0: bit_order = LSBFIRST; break;
      case 1: bit_order = MSBFIRST; break;
    }
    SPI.setBitOrder(bit_order);
    char m[12];
    sprintf(m, "Bit order is set to %s", aux);
    Serial.println(m);

  // 03(3) setClockDivider
  } else if (atoi(val) == 3) {
    int clock_divider_identifier = atoi(aux);
    int clock_divider;
    switch(clock_divider_identifier) {
      case 2:   clock_divider = SPI_CLOCK_DIV2;   break;
      case 4:   clock_divider = SPI_CLOCK_DIV4;   break;
      case 8:   clock_divider = SPI_CLOCK_DIV8;   break;
      case 16:  clock_divider = SPI_CLOCK_DIV16;  break;
      case 32:  clock_divider = SPI_CLOCK_DIV32;  break;
      case 64:  clock_divider = SPI_CLOCK_DIV64;  break;
      case 128: clock_divider = SPI_CLOCK_DIV128; break;
      default:  clock_divider = SPI_CLOCK_DIV4;   break;                  
    }  
    SPI.setClockDivider(clock_divider);
    char m[12];
    sprintf(m, "Clock divider is set to %s", aux);
    Serial.println(m);

  // 04(4) setDataMode
  } else if (atoi(val) == 4) {
    int data_mode_identifier = atoi(aux);
    int data_mode;
    switch(data_mode_identifier) {
      case 0: data_mode = SPI_MODE0; break;
      case 1: data_mode = SPI_MODE1; break;
      case 2: data_mode = SPI_MODE2; break;
      case 3: data_mode = SPI_MODE3; break;                
    }   
    SPI.setDataMode(data_mode);
    char m[12];
    sprintf(m, "Data mode is set to %s", aux);
    Serial.println(m);

  // 05(5) transfer
  } else if (atoi(val) == 5) {
    SPI.transfer(aux);
    char m[12];
    sprintf(m, "%s is sent to SPI bus", aux);
    Serial.println(m);

  }
}
int pir = A3;
int detekcija = 0;
int alarm = 2;
int brojanje = 3;
volatile int brprit = 0;
volatile int alarmprit = 0;
volatile int mod = 2;
volatile int ispis = 0;
volatile int ugaseno = 0;
volatile int ugasenobr = 0;
int crvena = 7;
int zelena = 8;
int zuta = 9;
volatile int stanjeC = LOW;
volatile int stanjeZ = LOW;
volatile int stanjeZu = LOW;
volatile int brojac = 0;

void setup() {
  pinMode(crvena, OUTPUT);
  pinMode(zelena, OUTPUT);
  pinMode(zuta, OUTPUT);
  pinMode(pir, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  pinMode(alarm, INPUT_PULLUP);
  pinMode(brojanje, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(alarm), prekid1, HIGH);
  attachInterrupt(digitalPinToInterrupt(brojanje), prekid2, HIGH);
}

void loop() {

  if (ispis == 1) {
    ispis = 0;
    Serial.println("Alarm mod ukljucen!");
  }
  if (ispis == 2) {
    ispis = 0;
    Serial.println("Brojanje mod ukljucen!");
  }
  if (ugaseno == 1) {
    ugaseno = 0;
    Serial.println("Alarm iskljucen!");
  }
  if (ugasenobr == 1) {
    ugasenobr = 0;
    Serial.println("Brojanje iskljuceno!");
  }
  /* alarmprit = digitalRead(alarm);
    brprit = digitalRead(brojanje);

    if(alarmprit != 0){
     Serial.println("Alarm mod ukljucen!");
    }

    if(brprit !=0){
     Serial.println("Brojanje mod ukljucen!");
    }

    if (alarmprit != 0  || mod == 0) {
     mod = 0;
    }
    if (brprit != 0 || mod == 1) {
     mod = 1;
    }*/
  if (mod == 0) {
    if (analogRead(pir) > 500 && detekcija == 0 && stanjeC == 0) {
      detekcija = 1;
      Serial.println("Upad");
      //stanjeC = HIGH;
      stanjeC = 3000;
      digitalWrite(LED_BUILTIN, HIGH);
    }
    if (analogRead(pir) < 200) {
      detekcija = 0;
      digitalWrite(LED_BUILTIN, LOW);
    }

  }
  if (mod == 1) {
    if (analogRead(pir) > 500 && detekcija == 0) {
      detekcija = 1;
      brojac++;
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Prosao");
      Serial.println(brojac);
    }
    if (analogRead(pir) < 200) {
      detekcija = 0;
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  //digitalWrite(crvena, stanjeC);
  digitalWrite(zuta, stanjeZu);
  digitalWrite(zelena, stanjeZ);
  if (stanjeC > 0) {
    tone(crvena, stanjeC, 500);
  }


  delay(300);


}

void prekid1() {

  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300)
  {
    if (mod == 2) {
      mod = 0;
      stanjeZu = HIGH;
      ispis = 1;
    }
    else if (mod == 0) {
      stanjeZu = LOW;
      stanjeC = 0;
      mod = 2;
      ugaseno = 1;
    }
  }
  last_interrupt_time = interrupt_time;
}

void prekid2() {

  static unsigned long last_interrupt_time2 = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 300ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time2 > 300)
  {
    if (mod == 2) {
      stanjeZ = HIGH;
      mod = 1;
      ispis = 2;
    }
    else if (mod == 1) {
      brojac = 0;
      stanjeZ = LOW;
      mod = 2;
      ugasenobr = 1;
    }
  }
  last_interrupt_time2 = interrupt_time;
}


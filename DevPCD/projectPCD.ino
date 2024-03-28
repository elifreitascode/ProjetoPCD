int max_calculo = 0;
int tempc = 0;
int pin=A0;
int i=0;

void setup() {
  Serial.begin(9600);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
}

void loop() {
  max_calculo=0;
  for(i=0;i<10;i++){
    max_calculo = (( 5.0 * analogRead(pin) * 100.0) / 1024) + max_calculo;
    delay(100);
  }
  tempc= max_calculo/11;
  Serial.println(tempc);
  if(tempc < 10){
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
  }
  else if(tempc > 10){
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
  }
  else{
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
  }
}

#include <SPI.h> //INCLUSÃO DE BIBLIOTECA DO RADIO FREQUENCIA
#include <MFRC522.h> //INCLUSÃO DE BIBLIOTECA
#include <Servo.h> //INCLUSÃO DE BIBLIOTECA DO SERVO MOTOR
#include <Ethernet.h> //INCLUSÃO DE BIBLIOTECA DA ETHERNET
#include <PubSubClient.h> // INCLUSÃO DE BIBLIOTECA DO CLIENTE MQTT

#define SS_PIN 8 //PINO SDA
#define RST_PIN 9 //PINO DE RESET
#define SERVO 3 // Porta Digital 3 PWM
#define IO_USERNAME  "ekko14"
#define IO_KEY       "aio_xHXF09HcEcxXSbGZkckjDb8JGTom"
int ID_Random = random(0,99999);// Endereçamento IP utilizado para o cliente e servidor
byte mac[]    = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(52,54,163,195); //adafruit.io

EthernetClient ethClient;
PubSubClient client(ethClient);

//Função callback chamada quando uma mensagem for recebida para subscrições:
void callback(char* topic, byte* payload, unsigned int length) {

}

MFRC522 rfid(SS_PIN, RST_PIN); //PASSAGEM DE PARÂMETROS REFERENTE AOS PINOS

const int pinoLed = 6; //PINO DIGITAL UTILIZADO PELO LED
const int pinoSensor = 5; //PINO DIGITAL UTILIZADO PELO SENSOR
unsigned long tempoAtual=0;
unsigned long deccorrree=0;
unsigned long Tempo_comcarro = 0;
unsigned long Tempo_semcarro=0;
unsigned long Tempo_intervalo1=15000;
String palavra_chave[2] = {"43:E3:29:BE","DA:F9:3C:16"};
String strID = "";
Servo s; // Variável Servo
int pos; // Posição Servo
int8_t aux = 0;
int8_t aux1 = 0;
String resposta;
void setup(){
  s.attach(SERVO);
  Serial.begin(9600);
  s.write(10); // Inicia motor posição zero
  pinMode(pinoSensor, INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO
  //DO ARDUINO PARA GARANTIR QUE NÃO EXISTA FLUTUAÇÃO ENTRE 0 (LOW) E 1 (HIGH)
  pinMode(pinoLed, OUTPUT); //DEFINE O PINO COMO SAÍDA
  digitalWrite(pinoLed, LOW); //LED INICIA DESLIGADO
  SPI.begin(); //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); //INICIALIZA MFRC522
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac );
  delay(5000); // Allow the hardware to sort itself out

}

void loop(){
  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");

    if (client.connect(ID_Random,IO_USERNAME,IO_KEY)) { //TROCAR O NOME DO CLIENTE PARA NÃO TER PROBLEMA COM A CONEXÃO  
   Serial.println("connected");
    }
    else {
         Serial.println(client.state());
    }
  }
  unsigned long currentTime = millis();
  if(digitalRead(pinoSensor) == LOW){
   Tempo_comcarro = currentTime;
   Serial.print("Tempo com carro:");
   Serial.print(Tempo_comcarro);
   Serial.println();
  }
  if(digitalRead(pinoSensor) == HIGH){
    Tempo_semcarro = currentTime;
    Serial.print("Tempo sem carro:");
   Serial.print(Tempo_semcarro);
    Serial.println();
    aux = 0;
   

  }
  if(Tempo_comcarro > Tempo_semcarro + Tempo_intervalo1 ){
   //Serial.println("DEU CERTO");
   resposta = String("Vaga Ocupada Erradamente");
     if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
   
  for (byte i = 0; i < 4; i++) {
    strID += (rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX) + (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
  /FIM DO BLOCO DE CÓDIGO RESPONSÁVEL POR GERAR A TAG RFID LIDA/

  Serial.print("Identificador (UID) da tag: "); //IMPRIME O TEXTO NA SERIAL //IMPRIME NA SERIAL O UID DA TAG RFID
  Serial.print(strID); //IMPRIME NA SERIAL O UID DA TAG RFID
  rfid.PICC_HaltA(); //PARADA DA LEITURA DO CARTÃO
  rfid.PCD_StopCrypto1(); //PARADA DA CRIPTOGRAFIA NO PCD

  }
  if(strID==palavra_chave[0] || strID==palavra_chave[1]){
      digitalWrite(pinoLed, LOW); //APAGA O LED
      aux = 0;
  }
  else{
   
    digitalWrite(pinoLed, HIGH);
    aux = 1;

    if(millis()-deccorrree>10 && aux1==0){
    pos++;
    s.write(pos);
    deccorrree=millis();
    }
    if(pos>90){
      aux1=1;
  }
  if(millis()-deccorrree>10 && aux1==1){
    pos--;
    s.write(pos);
    deccorrree=millis();
    }
    if(pos==0){
      aux1=0;
  }
  }
  
   
 
  else{ //SENÃO, FAZ
    digitalWrite(pinoLed, LOW); //APAGA O LED
    strID = "";
  }
    // Aguarda conexão    

     String Palavra = String("VAGA OCUPADAAAAAAAA ERRADDOO");
     char msg[50];
     
     Palavra.toCharArray(msg,Palavra.length()+1);
     
     String Palavra_2 = String("Tudo Certo :)");
     char msg_2[50];
     
     Palavra_2.toCharArray(msg_2,Palavra_2.length()+1);

  if(millis()-tempoAtual >15000){
   
      tempoAtual=millis();
      if(aux != 0){
      client.publish("ekko14/feeds/vaga1",msg);
     }
     else{
            client.publish("ekko14/feeds/vaga1",msg_2);
     }
    }
  client.loop();
}

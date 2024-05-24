#include <DHT.h>
#include <Servo.h>
#include <LiquidCrystal.h>

//campanello
int ButtonStateCampanello = 0;
int buzzerCampanello = 51;
const int inputPinCampanello = 53;

//PIR
int pirPin = 7;
int sensorState = 0;
int volte = 0;

// Buzzer
int buzzer = 10;
int controllo  = 2;

//BUTTON
const int inputPin = 13;
bool buttonPressed = false;

//Display
const int rs=12, en=11, d4=5, d5=4, d6=3, d7=2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//fototransistor
int Photo_Pin = A0;

//RGB
int rossoPin = 22;
int verdePin = 24;
int bluPin = 26;

//SERVO MOTORE
#define sensorThreshold 7 // Valore soglia del sensore di distanza
Servo myServo;  // Oggetto servo

//DHT11
#define DHTPIN 9     // Definisci il pin a cui è collegato il sensore
#define DHTTYPE DHT11   // Definisci il tipo di sensore utilizzato
DHT dht(DHTPIN, DHTTYPE); // Inizializza il sensore DHT

//SENSORE DISTANZA
#define trigPin 52 // Definisci il pin Trig
#define echoPin 50 // Definisci il pin Echo

long duration; // Variabile per memorizzare la durata del segnale
int distance; // Variabile per memorizzare la distanza

void setup() {
  Serial.begin(9600); // Inizializza la comunicazione seriale
  
  //DHT11
  dht.begin();// Inizializza il sensore DHT

  //PIR
  pinMode(pirPin, INPUT);

  //BUZZER
  pinMode(buzzer, OUTPUT);

  //BUTTON
  pinMode(inputPin, INPUT);

  //Display
  lcd.begin(16, 2);

  //fototransistor
  pinMode(Photo_Pin, INPUT);

  //RGB
  pinMode(rossoPin, OUTPUT);
  pinMode(verdePin, OUTPUT);
  pinMode(bluPin, OUTPUT);
  // Inizializzazione del generatore di numeri casuali
  randomSeed(analogRead(0));

  //SENSORE DISTANZA
  pinMode(trigPin, OUTPUT); // Imposta il pin Trig come output
  pinMode(echoPin, INPUT); // Imposta il pin Echo come input

  //SERVO MOTORE
  myServo.attach(A1);  // Collega il servo al pin analogico A1

  //Campanello
  pinMode(inputPinCampanello, INPUT);
}

void loop() {
  
  //SENSORE DISTANZA
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Misura il tempo impiegato dal segnale ad andare da HIGH a LOW sull'Echo
  duration = pulseIn(echoPin, HIGH);
  // Converti il tempo in distanza (in cm)
  distance = duration * 0.034 / 2;
  /*Serial.print("Distanza: ");
  Serial.print(distance);
  Serial.println(" cm");*/

  //SERVO MOTORE
  if (distance < sensorThreshold) { // Se la distanza è inferiore alla soglia
    // Ruota il servo di 90 gradi
    myServo.write(90);
    delay(800); // Attendere un breve momento per permettere al servo di raggiungere la posizione desiderata
  } else {
    // Riporta il servo nella posizione iniziale (0 gradi)
    myServo.write(180);
    delay(800); // Attendere un breve momento per permettere al servo di raggiungere la posizione desiderata
  }


  //fototransistror
  int readPhoto = analogRead(Photo_Pin);
  if(readPhoto < 1){
    int redValue = random(256);   // Valore casuale tra 0 e 255 per il rosso
    int greenValue = random(256); // Valore casuale tra 0 e 255 per il verde
    int blueValue = random(256);  // Valore casuale tra 0 e 255 per il blu

    analogWrite(rossoPin, redValue);
    analogWrite(verdePin, greenValue);
    analogWrite(bluPin, blueValue);  

  }else{
    analogWrite(rossoPin, 0);
    analogWrite(verdePin, 0);
    analogWrite(bluPin, 0);
  }


  // PIR
  if(volte == 0){
    sensorState = digitalRead(pirPin);
    if (sensorState == HIGH) {
      volte = 1;

    }
  }
      

    if (volte == 1) {
      delay(800);
      tone(buzzer, 1000);
      delay(1000);
      noTone(buzzer);
    } 
  

  // Check if the button is pressed
  if (digitalRead(inputPin) == HIGH) {
    volte = 2; 
    noTone(buzzer); 
  }

  // Leggi la temperatura e l'umidità dal sensore
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();


  // Stampa i valori di temperatura e umidità sulla porta seriale
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Umidità: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Mostra i valori di temperatura e umidità sull'LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Umid: ");
  lcd.print(humidity);
  lcd.print(" %");

  ButtonStateCampanello = digitalRead(inputPinCampanello);
  if(ButtonStateCampanello == HIGH){
      tone(buzzerCampanello, 1000);
  }else{
    noTone(buzzerCampanello);
  }

  delay(1000);
}
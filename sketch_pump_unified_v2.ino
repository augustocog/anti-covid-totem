#include <Adafruit_MLX90614.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SI2C.h>
#include <Wire.h>

// Adafruit MLX90614 Library - Version: Latest 

#define I2C_ADDR 0x27 //endere?o p/mapear pinos A4 e A5 ao l2c
#define BACKLIGHT_PIN 3 // declarando pinos do LCD
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
#define buzzer A2

#define echoPin2 12                // definindo pinos e vari?veis do sensor de presen�a
#define trigPin2 11
#define pinPOT A3

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); // fun��es para mapear os pinos ao l2c e declarar o sensor MLX90614

int long duration;
int distance;
int value;

const int motor = 8;

int temperature; // armazena a temperatura p/verifica��o e disparo do buzzer

void setup()
{
  pinMode(echoPin2, INPUT);        // definindo pinos e modos sensor presen�a
  pinMode(trigPin2, OUTPUT);  
  pinMode(motor, OUTPUT);
  pinMode(pinPOT, INPUT);
  pinMode(buzzer, OUTPUT);

  mlx.begin();
  lcd.begin (16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH); //luz de fundo
  lcd.home ();
  Serial.begin(9600);        
}

void loop()
{

  lcd.setCursor(0,0);
  lcd.print("Ambiente ");
  lcd.print(mlx.readAmbientTempC());
  lcd.print(" C");
 
  lcd.setCursor(0,1);
  lcd.print("Alvo  ");
  lcd.print(mlx.readObjectTempC());
  lcd.print(" C");
  temperature = mlx.readObjectTempC();
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.println(" C");

  digitalWrite(trigPin2,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2,LOW);
  duration=pulseIn(echoPin2,HIGH);
  distance=(duration*0.034/2);   // l? a dist?ncia para o sensor de presen�a
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  
  if(distance <= 10)               // verifica a presen?a de m?os em 10 cm de dist?ncia
  {  
    Serial.println("Pump in operation");
    digitalWrite(motor, LOW);    // inicia e opera a bomba
    delay(value*2);              // valor do delay muda com retorno da leitura do pot
    digitalWrite(motor, HIGH);     // para a bomba
    delay(1000);                 // delay para impedir opera??o cont?nua
  }
  else 
  {
    digitalWrite(motor, LOW);     // caso n?o detecte m?os, bomba parada
  }

  if(distance<=10 && temperature >= 37)               // verifica a presen?a de pessoa em 10 cm de dist?ncia
  {  
    Serial.println("Body temp high");
    tone(buzzer,100);           // inicia e opera o alarme buzzer
    delay(1000);                
    noTone(buzzer);             // desliga o buzzer
    delay(1000);                // delay para impedir opera??o cont?nua
  }
  else 
  {
    noTone(buzzer);     // caso n?o detecte pessoa, buzzer desligado
  }
}

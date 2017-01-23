/*

  BLUETOOTH CONTROLLED TUNING CAR
  
  Project by:
  Pedro Lopez Escudero
  Gabriel Bonilla Ruiz
  Sergio Jimenez Salmeron
  Javier Martin Fernandez
  
  26 Jan 2017
  
  You can find the entire project in:

*/


#include <SoftwareSerial.h>
#include <TimerOne.h>

SoftwareSerial BT(4,2);                                    // 4 RX 2 TX
#define IN1 8
#define IN2 9
#define IN3 12
#define IN4 13
#define PWM 6
#define PWM2 5
#define claxon 3
#define faros 10
#define xenon 11


volatile int Velocidad = 100;
volatile int VelocidadDir = 255;
volatile char Orden;
volatile int ClaxEnc = 0;
volatile int direccion_der = 0;
volatile int direccion_izq = 0;
volatile unsigned int xenon_cont = 0;
volatile int sum = 1;
volatile int luces = 0;

void setup() {

  BT.begin(9600);                                                // Serial Port Configuration 

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(PWM, OUTPUT);

  Timer1.initialize(10000);                                      // Dispara cada 240 ms
  Timer1.attachInterrupt(ISR_Blink);                             // Activa la interrupcion y la asocia a ISR_Blink

  analogWrite(PWM, Velocidad);                                   // turn the LED on (HIGH is the voltage level)
  analogWrite(PWM2, VelocidadDir);
                                                                          
  pinMode(claxon, OUTPUT);                                       //Sonido
}

void loop() {
  
  if (BT.available()){
      Orden = BT.read();
      switch (Orden){
        case 'f': // adelante
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
        break;
        case 'b': //atras
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
        break;
        case 'i': //izquierda
          if (direccion_der == 1){
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            direccion_der = 0;
          } else if (direccion_der == 0) {
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            direccion_izq = 1;
          }
        break;
        case 'd': //derecha
          if (direccion_izq == 1){
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            direccion_izq = 0;
          } else if (direccion_izq == 0){
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            direccion_der = 1;
          }
        break;
    
        case 's': //parar
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          Velocidad = 100;
          analogWrite(PWM, Velocidad);
        break;
    
        case 'p': //mas velocidad
          if(Velocidad < 255){
            Velocidad+=10;
          }
          analogWrite(PWM, Velocidad);
        break;
    
        case 'n': // menos velocidad
          if(Velocidad > 0){
            Velocidad-=10;
          }
          analogWrite(PWM, Velocidad);
        break;
        case 'a': //claxon
          if(!ClaxEnc){
            ClaxEnc = 1;
            analogWrite(claxon,200);
          }else{
            ClaxEnc = 0;
            analogWrite(claxon,0);
          }
        break;
        case 'l': //luces
          if(!luces){
            luces = 1;
            analogWrite(faros,255);
          }else{
            luces = 0;
            analogWrite(faros,0);
          }
        break;
    }
  }
}

void ISR_Blink () {
  
  if (sum == 1){
    xenon_cont += 5;
    if (xenon_cont >= 255){
      sum = 0;  
    }
  } else if (sum == 0){
    xenon_cont -= 5;
    if (xenon_cont <= 0){
      sum = 1;
    }
  }
  analogWrite(xenon,xenon_cont);
}  

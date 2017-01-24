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

SoftwareSerial BT(4,2);                            // Configure the Bluetooth module ports (pin 4 to receive, pin 2 to transfer)
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

  BT.begin(9600);                                  // Serial Port Configuration 

  pinMode(IN1, OUTPUT);                            // Configure the H bridge pins
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(PWM, OUTPUT);

  Timer1.initialize(10000);                        // Initialize the Timer1 at 240 ms
  Timer1.attachInterrupt(ISR_Blink);               // Activates the Interrupt Rountine of Timer1 asociated to ISR_Blink

  analogWrite(PWM, Velocidad);                     // Configure the PWM of the H bridge for IN3 and IN4
  analogWrite(PWM2, VelocidadDir);                 // Configure the PWM of the H bridge for IN1 and IN2
                                                                          
  pinMode(claxon, OUTPUT);                         // Configure the output pin of the buzzer
}

void loop() {                                                    
  
  if (BT.available()){                                            
      Orden = BT.read();
      switch (Orden){
        
        case 'f':                                  // In case you want to move forward
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
        break;
        
        case 'b':                                  // In case you want to move backward
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
        break;
        
        case 'i':                                  // In case you want to turn to the left
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
        
        case 'd':                                  // In case you want to turn to the right
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
    
        case 's':                                  // In case you want to stop the car.
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          Velocidad = 100;
          analogWrite(PWM, Velocidad);
        break;
    
        case 'p':                                  // In case you want to go faster
          if(Velocidad < 255){
            Velocidad+=10;
          }
          analogWrite(PWM, Velocidad);
        break;
    
        case 'n':                                  // In case you want to go slower
          if(Velocidad > 0){
            Velocidad-=10;
          }
          analogWrite(PWM, Velocidad);
        break;
        
        case 'a':                                  // Beep of the buzzer
          if(!ClaxEnc){
            ClaxEnc = 1;
            analogWrite(claxon,200);
          }else{
            ClaxEnc = 0;
            analogWrite(claxon,0);
          }
        break;
        
        case 'l':                                  // Turn on the frontal lights
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

void ISR_Blink () {                                // Interrupt Routine asociated to Timer1. Turns off/on gradually the LEDs that simulates the neon lights
  
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

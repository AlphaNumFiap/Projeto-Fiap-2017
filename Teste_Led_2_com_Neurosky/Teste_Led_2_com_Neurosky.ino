#include <SoftwareSerial.h>
#include <Brain.h>

// Set up the software serial port on pins 10 (RX) and 11 (TX). We'll only actually hook up pin 10.
SoftwareSerial softSerial(10, 11);

// Set up the brain reader, pass it the software serial object you want to listen on.
Brain brain(softSerial);

const long interval = 1000;
const int pinCount = 1;
const int qtd_linha = 8;
int ledPins[1][8] = {{2, 3, 4, 5, 6, 7, 8, 9}};
String letras[1][8] = {"A", "B", "C", "D", "E", "F", "G", "H"};

//Variáveis de Verificação
int verificar = 0;
int linha_escolhida = 0;
int inicio_led = 0;
int final_led = 0;
int contador_nivel_2 = 0;
int contador_nivel_3 = 0;

char leitura; 

int conta = 0;

unsigned long previousMillis = 0; 

int pausa = 0;

void setup() {
  // Start the software serial.
  softSerial.begin(57600);

  // Start the hardware serial.
  Serial.begin(57600);
  
  // put your setup code here, to run once:
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    for (int thisPin1 = 0; thisPin1 < qtd_linha; thisPin1++) {
      pinMode(ledPins[thisPin][thisPin1], OUTPUT);
    }
  } 
}

void loop() {
  // put your main code here, to run repeatedly:  
  if (brain.update()) {
    //Serial.println(brain.readCprintCSVprintCSVSV());    
    // Attention runs from 0 to 100.  
    
  }
  
  unsigned long currentMillis = millis();      
  if(brain.readSignalQuality() <60) {
    if(currentMillis - previousMillis >= interval) {
      Serial.print("Onda Theta:");
      Serial.print(brain.readCSV());
      Serial.print("\n");
      previousMillis = currentMillis;  
      leitura = Serial.read();     
      switch(verificar){
          case 0:
            if (brain.readAttention() > 70){
              if(conta == 0){
                liga_linha(0);
                linha_escolhida = 0;
                verificar = 1;
              }
              else{
                liga_linha(conta-1);
                linha_escolhida = conta-1;
                verificar = 1; 
              }              
            }            
            else{
              if(conta == pinCount){
                led_nivel_1(0, conta-1);
                conta = 1;  
              }
              else{
                led_nivel_1(conta, conta-1);
                conta += 1;  
              } 
            }
          break;
          case 1:
              if (brain.readAttention() > 70){
                if(contador_nivel_2 == 0){                
                  inicio_led = (qtd_linha/2);
                  final_led = qtd_linha;
                  verificar = 2;
                  contador_nivel_3 = (qtd_linha/2);
                  Serial.print("Contador: ");
                  Serial.print(contador_nivel_2, DEC);
                  Serial.print("\n");
                  liga_linha2(linha_escolhida, inicio_led, final_led);
                }
                else{
                  inicio_led = 0;
                  final_led = qtd_linha/2;
                  verificar = 2;
                  contador_nivel_3 = (qtd_linha/2);
                  Serial.print("Contador: ");
                  Serial.print(contador_nivel_2, DEC);
                  Serial.print("\n");
                  liga_linha2(linha_escolhida, inicio_led, final_led);
                }
              }
              else{
                liga_linha(linha_escolhida);
              }
          break;
          case 2:
            if (brain.readAttention() > 70){
              led_escolhido(linha_escolhida, contador_nivel_3-1);
            }
            else{
              liga_linha2(linha_escolhida, inicio_led, final_led);
            }
          break; 
          default:
          break;       
      }
    }
  }
    //Serial.print("-------------TERMINA O LOOP-------------");
    //Serial.print("\n");
}

void led_nivel_1(int num, int desliga){
  Serial.print("LED: ");
  Serial.print(num, DEC);
  Serial.print(" / LED Anterior: ");
  Serial.print(desliga, DEC);
  Serial.print("\n");
  int thisPin=num;
  for (int thisPin1 = 0; thisPin1 < qtd_linha; thisPin1++) {
     digitalWrite(ledPins[desliga][thisPin1], LOW);
     Serial.print(ledPins[thisPin][thisPin1], DEC);
     Serial.print("\n");
     digitalWrite(ledPins[thisPin][thisPin1], HIGH);     
  }
}

void liga_linha(int linha){
  Serial.print("LED Escolhido: ");
  Serial.print(linha, DEC);
  Serial.print("\n");
  
  for (int thisPin3 = 0; thisPin3 < qtd_linha; thisPin3++) {    
    digitalWrite(ledPins[linha][thisPin3], LOW);
  }  

  if(contador_nivel_2 == 0){
    for (int thisPin3 = 0; thisPin3 < qtd_linha/2; thisPin3++) {    
      Serial.print(ledPins[linha][thisPin3], DEC);
      Serial.print("\n");
      leitura = Serial.read();   
      if(brain.readAttention() > 70){
        inicio_led = 0;
        final_led = (qtd_linha/2);
        verificar = 2;
        contador_nivel_3 = 0;
        liga_linha2(linha, final_led, final_led);                
      }
      else{      
        digitalWrite(ledPins[linha][thisPin3], HIGH);        
      }
    }
    contador_nivel_2 = contador_nivel_2 + 1;
  }
  else{
    for (int thisPin3 = qtd_linha/2; thisPin3 < qtd_linha; thisPin3++) {    
      Serial.print(ledPins[linha][thisPin3], DEC);
      Serial.print("\n");
      leitura = Serial.read();   
      if(brain.readAttention() > 70){
        inicio_led = qtd_linha/2;
        final_led = qtd_linha;
        verificar = 2;
        contador_nivel_3 = (qtd_linha/2);
        liga_linha2(linha, inicio_led, final_led);       
      }
      else{      
        digitalWrite(ledPins[linha][thisPin3], HIGH);        
      }
    }
    contador_nivel_2 = 0;
  }
  
}

void liga_linha2(int linha, int inicio, int intervalo){
  Serial.print("Ultimo Led: ");
  Serial.print(linha, DEC);
  Serial.print("\n");
  Serial.print("Inicio: ");
  Serial.print(inicio, DEC);
  Serial.print("\n");
  Serial.print("Intervalo: ");
  Serial.print(intervalo, DEC);
  Serial.print("\n");
  for (int thisPin4 = 0; thisPin4 < qtd_linha; thisPin4++) {    
    digitalWrite(ledPins[linha][thisPin4], LOW);
  } 

  if(contador_nivel_3 == intervalo){
    Serial.print("Contador: ");
    Serial.print(contador_nivel_3, DEC);  
    Serial.print("\n");    
    digitalWrite(ledPins[linha][inicio], HIGH);
    contador_nivel_3 = inicio+1;
  }
  else{
    Serial.print("Contador: ");
    Serial.print(contador_nivel_3, DEC);  
    Serial.print("\n");
    digitalWrite(ledPins[linha][contador_nivel_3], HIGH); 
    contador_nivel_3 = contador_nivel_3 + 1; 
  }
  /*for (int thisPin4 = inicio; thisPin4 < intervalo; thisPin4++) {    
      Serial.print(ledPins[linha][thisPin4], DEC);
      Serial.print("\n");
      Serial.print("Inicio: ");
      Serial.print(inicio, DEC);
      Serial.print(" / Intervalo: ");
      Serial.print(intervalo, DEC);
      Serial.print("\n");
      leitura = Serial.read();   
      if(brain.readAttention() > 70){
        Serial.print("LED Escolhido: ");
        Serial.print(ledPins[linha][thisPin4], DEC);
        Serial.print("\n");
        led_escolhido(linha, thisPin4);
      }
      else{      
        digitalWrite(ledPins[linha][thisPin4], HIGH);
        delay(1000);
        digitalWrite(ledPins[linha][thisPin4], LOW);
      }
  }*/  
}

void led_escolhido(int linha, int led){
  //for(int y=0; y<3; y++){
    digitalWrite(ledPins[linha][led], LOW);
    Serial.print("Letra Escolhida:");
    Serial.print(letras[linha][led]);
    Serial.print("\n");
    delay(1000);
    digitalWrite(ledPins[linha][led], HIGH);
    Serial.print("Letra Escolhida:");
    Serial.print(letras[linha][led]);
    Serial.print("\n");
    delay(1000);
  //}
    Serial.print("Letra Escolhida:");
    Serial.print(letras[linha][led]);
    Serial.print("\n");

  verificar = 0;
}


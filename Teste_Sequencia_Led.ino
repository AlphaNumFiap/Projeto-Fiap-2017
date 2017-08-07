// constants won't change. Used here to set a pin number:
const int ledPin =  13;// the number of the LED 
const int pinCount = 2;
const int qtd_linha = 4;
int ledPins[2][4] = {{2, 3, 4, 5}, {6, 7, 8, 9}};
int letras[2][4] = {{"A", "B", "C", "D"}, {"E", "F", "G", "H"}};

//Variáveis de Verificação
int verificar = 0;
int linha_escolhida = 0;
int inicio_led = 0;
int final_led = 0;

char leitura; 

int conta = 0;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)

bool teste_linha = false;

void setup() {
  Serial.begin(9600);
  
  // set the digital pin as output:
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    for (int thisPin1 = 0; thisPin1 < qtd_linha; thisPin1++) {
      pinMode(ledPins[thisPin][thisPin1], OUTPUT);
    }
  }  
}

void loop() {
  // here is where you'd put code that needs to be running all the time.  
    // check to see if it's time to blink the LED; that is, if the difference
    // between the current time and last time you blinked the LED is bigger than
    // the interval at which you want to blink the LED.
    unsigned long currentMillis = millis();      
    if(currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;  
      leitura = Serial.read();     

      switch(verificar){
        case 0:
          if (leitura == 'r'){
              liga_linha(conta-1);
              linha_escolhida = conta-1;
              verificar = 1;
          }            
          else{
            if(conta == pinCount){
              liga_led(0, conta-1);
              conta = 1;  
            }
            else{
              liga_led(conta, conta-1);
              conta += 1;  
            } 
          }
        break;
        case 1:
          if (leitura == 'r'){
              liga_linha2(linha_escolhida, 0, 2);              
              verificar = 2;
          }            
          else{
            liga_linha(linha_escolhida);
          }
        break;
        case 2:
          liga_linha2(linha_escolhida, inicio_led, final_led);
        break;
        default:
        break;
      }           
    }  
}

void liga_led(int num, int desliga){
  Serial.print("LED: ");
  Serial.print(num, DEC);
  Serial.print(" / LED Anterior: ");
  Serial.print(desliga, DEC);
  Serial.print("\n");
  int thisPin=num;
  for (int thisPin1 = 0; thisPin1 < qtd_linha; thisPin1++) {
     Serial.print(ledPins[thisPin][thisPin1], DEC);
     Serial.print("\n");
     digitalWrite(ledPins[thisPin][thisPin1], HIGH);
     digitalWrite(ledPins[desliga][thisPin1], LOW);
  }
}

void liga_linha(int linha){
  Serial.print("LED Escolhido: ");
  Serial.print(linha, DEC);
  Serial.print("\n");
  
  for (int thisPin3 = 0; thisPin3 < qtd_linha; thisPin3++) {    
    digitalWrite(ledPins[linha][thisPin3], LOW);
  }  
   
  for (int thisPin3 = 0; thisPin3 < qtd_linha/2; thisPin3++) {    
      Serial.print(ledPins[linha][thisPin3], DEC);
      Serial.print("\n");
      leitura = Serial.read();   
      if(leitura == 'r'){
        liga_linha2(linha, 0, (qtd_linha/2));
        inicio_led = 0;
        final_led = (qtd_linha/2);
        verificar = 2;
      }
      else{      
        digitalWrite(ledPins[linha][thisPin3], HIGH);        
      }
  }
  delay(1000); 
  for (int thisPin3 = 0; thisPin3 < qtd_linha/2; thisPin3++) {    
      Serial.print(ledPins[linha][thisPin3], DEC);
      Serial.print("\n");
      leitura = Serial.read();   
      if(leitura == 'r'){
        liga_linha2(linha, 2, qtd_linha);
        inicio_led = 0;
        final_led = (qtd_linha/2);
        verificar = 2;
      }
      else{      
        digitalWrite(ledPins[linha][thisPin3], LOW);        
      }
  } 
  for (int thisPin3 = 2; thisPin3 < qtd_linha; thisPin3++) {    
      Serial.print(ledPins[linha][thisPin3], DEC);
      Serial.print("\n");
      leitura = Serial.read();   
      if(leitura == 'r'){
        liga_linha2(linha, 2, qtd_linha);
        inicio_led = 2;
        final_led = qtd_linha;
        verificar = 2;
      }
      else{      
        digitalWrite(ledPins[linha][thisPin3], HIGH);        
      }
  }   
  delay(1000);
  for (int thisPin3 = 2; thisPin3 < qtd_linha; thisPin3++) {    
      Serial.print(ledPins[linha][thisPin3], DEC);
      Serial.print("\n");
      leitura = Serial.read();   
      if(leitura == 'r'){
        liga_linha2(linha, 2, qtd_linha);
        inicio_led = 2;
        final_led = qtd_linha;
        verificar = 2;
      }
      else{      
        digitalWrite(ledPins[linha][thisPin3], LOW);        
      }
  }     
}

void liga_linha2(int linha, int inicio, int intervalo){
  Serial.print("Ultimo Led: ");
  Serial.print(linha, DEC);
  Serial.print("\n");
  for (int thisPin4 = 0; thisPin4 < qtd_linha; thisPin4++) {    
    digitalWrite(ledPins[linha][thisPin4], LOW);
  }    
  for (int thisPin4 = inicio; thisPin4 < intervalo; thisPin4++) {    
      Serial.print(ledPins[linha][thisPin4], DEC);
      Serial.print("\n");
      Serial.print("Inicio: ");
      Serial.print(inicio, DEC);
      Serial.print(" / Intervalo: ");
      Serial.print(intervalo, DEC);
      Serial.print("\n");
      leitura = Serial.read();   
      if(leitura == 'r'){
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
    }  
}

void led_escolhido(int linha, int led){
  for(int y=0; y<3; y++){
    digitalWrite(ledPins[linha][led], HIGH);
    delay(500);
    digitalWrite(ledPins[linha][led], LOW);
  } 
}




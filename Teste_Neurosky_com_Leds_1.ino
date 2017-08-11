#define LED 13
#define BAUDRATE 57600
#define DEBUGOUTPUT 0
#define GREENLED1 3
#define GREENLED2 4
#define GREENLED3 5
#define YELLOWLED1 6
#define YELLOWLED2 7
#define YELLOWLED3 8
#define YELLOWLED4 9
#define REDLED1 10
#define REDLED2 11
#define REDLED3 12
#define powercontrol 10

// checksum variables
byte generatedChecksum = 0;
byte checksum = 0;
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;
byte wave = 0;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;

//Variáveis para os testes com o LED
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
int conta = 0;

char leitura; 

// Microprocessor Setup //
void setup() {
  pinMode(GREENLED1, OUTPUT);
  pinMode(GREENLED2, OUTPUT);
  pinMode(GREENLED3, OUTPUT);
  pinMode(YELLOWLED1, OUTPUT);
  pinMode(YELLOWLED2, OUTPUT);
  pinMode(YELLOWLED3, OUTPUT);
  pinMode(YELLOWLED4, OUTPUT);
  pinMode(REDLED1, OUTPUT);
  pinMode(REDLED2, OUTPUT);
  pinMode(REDLED3, OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(BAUDRATE); // USB

  // set the digital pin as output:
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    for (int thisPin1 = 0; thisPin1 < qtd_linha; thisPin1++) {
      pinMode(ledPins[thisPin][thisPin1], OUTPUT);
    }
  } 
}

// Read data from Serial UART //
byte ReadOneByte() {
int ByteRead;
while(!Serial.available());
  ByteRead = Serial.read();
  #if DEBUGOUTPUT
  Serial.print((char)ByteRead); // echo the same byte out the USB serial
  (for debug purposes)
  #endif
  return ByteRead;
}

//MAIN LOOP//
void loop() {
// Look for sync bytes
if(ReadOneByte() == 170) {      
  if(ReadOneByte() == 170) {
    payloadLength = ReadOneByte();
    if(payloadLength > 169) //Payload length can not
      return;
    generatedChecksum = 0;
  for(int i = 0; i < payloadLength; i++) {
    payloadData[i] = ReadOneByte(); //Read payload into
    generatedChecksum += payloadData[i];
  }
  checksum = ReadOneByte(); //Read checksum byte
  generatedChecksum = 255 - generatedChecksum; //Take one's
  if(checksum == generatedChecksum) {
    poorQuality = 200;
    attention = 0;
    meditation = 0;
    //wave = 0;
    for(int i = 0; i < payloadLength; i++) { // Parse the payload
      switch (payloadData[i]) {
        case 2:
          i++;
          poorQuality = payloadData[i];
          bigPacket = true;
          break;
        case 4:
          i++;
          attention = payloadData[i];
          break;
        case 5:
          i++;
          meditation = payloadData[i];
          break;
        case 0x80:
          wave = payloadData[i+2] * 256 + payloadData[i+3];
          i = i + 3;
          break;
        case 0x83:
          i = i + 25;
          break;
        default:
          break;
      } // switch
    } // for loop
#if !DEBUGOUTPUT

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
// *** Add your code here ***
if(bigPacket) {
if(poorQuality == 0)
digitalWrite(LED, HIGH);
else
digitalWrite(LED, LOW);
Serial.print("PoorQuality: ");
Serial.print(poorQuality, DEC);
Serial.print(" Attention: ");
Serial.print(attention, DEC);
Serial.print(" Meditation: ");
Serial.print(meditation, DEC);
Serial.print(" Wave: ");
Serial.print(wave, DEC);
Serial.print(" Time since last packet: ");
Serial.print(millis() - lastReceivedPacket, DEC);
lastReceivedPacket = millis();
Serial.print("\n");
switch(attention / 10) {
case 0:
digitalWrite(GREENLED1, HIGH);
digitalWrite(GREENLED2, LOW);
digitalWrite(GREENLED3, LOW);
digitalWrite(YELLOWLED1, LOW);
digitalWrite(YELLOWLED2, LOW);
digitalWrite(YELLOWLED3, LOW);
digitalWrite(YELLOWLED4, LOW);
digitalWrite(REDLED1, LOW);
digitalWrite(REDLED2, LOW);
digitalWrite(REDLED3, LOW);
break;
case 1:
digitalWrite(GREENLED1, HIGH);
digitalWrite(GREENLED2, HIGH);
digitalWrite(GREENLED3, LOW);
digitalWrite(YELLOWLED1, LOW);
digitalWrite(YELLOWLED2, LOW);
digitalWrite(YELLOWLED3, LOW);
digitalWrite(YELLOWLED4, LOW);
digitalWrite(REDLED1, LOW);
digitalWrite(REDLED2, LOW);
digitalWrite(REDLED3, LOW);
break;
case 2:
digitalWrite(GREENLED1, HIGH);
digitalWrite(GREENLED2, HIGH);
digitalWrite(GREENLED3, HIGH);
digitalWrite(YELLOWLED1, LOW);
digitalWrite(YELLOWLED2, LOW);
digitalWrite(YELLOWLED3, LOW);
digitalWrite(YELLOWLED4, LOW);
digitalWrite(REDLED1, LOW);
digitalWrite(REDLED2, LOW);
digitalWrite(REDLED3, LOW);
break;
case 3:
digitalWrite(GREENLED1, HIGH);
digitalWrite(GREENLED2, HIGH);
digitalWrite(GREENLED3, HIGH);
digitalWrite(YELLOWLED1, HIGH);
digitalWrite(YELLOWLED2, LOW);
digitalWrite(YELLOWLED3, LOW);
digitalWrite(YELLOWLED4, LOW);
digitalWrite(REDLED1, LOW);
digitalWrite(REDLED2, LOW);
digitalWrite(REDLED3, LOW);
break;
case 4:
digitalWrite(GREENLED1, HIGH);
digitalWrite(GREENLED2, HIGH);
digitalWrite(GREENLED3, HIGH);
digitalWrite(YELLOWLED1, HIGH);
digitalWrite(YELLOWLED2, HIGH);
digitalWrite(YELLOWLED3, LOW);
digitalWrite(YELLOWLED4, LOW);
digitalWrite(REDLED1, LOW);
digitalWrite(REDLED2, LOW);
digitalWrite(REDLED3, LOW);
break;
case 5:
digitalWrite(GREENLED1, HIGH);
digitalWrite(GREENLED2, HIGH);
digitalWrite(GREENLED3, HIGH);
digitalWrite(YELLOWLED1, HIGH);
digitalWrite(YELLOWLED2, HIGH);
digitalWrite(YELLOWLED3, HIGH);
digitalWrite(YELLOWLED4, LOW);
digitalWrite(REDLED1, LOW);
digitalWrite(REDLED2, LOW);
digitalWrite(REDLED3, LOW);
break;
case 6:
digitalWrite(GREENLED1, HIGH);
digitalWrite(GREENLED2, HIGH);
digitalWrite(GREENLED3, HIGH);
digitalWrite(YELLOWLED1, HIGH);
digitalWrite(YELLOWLED2, HIGH);
digitalWrite(YELLOWLED3, HIGH);
digitalWrite(YELLOWLED4, HIGH);
digitalWrite(REDLED1, LOW);
digitalWrite(REDLED2, LOW);
digitalWrite(REDLED3, LOW);
break;
case 7:
digitalWrite(GREENLED1, HIGH);
digitalWrite(GREENLED2, HIGH);
digitalWrite(GREENLED3, HIGH);
digitalWrite(YELLOWLED1, HIGH);
digitalWrite(YELLOWLED2, HIGH);
digitalWrite(YELLOWLED3, HIGH);
digitalWrite(YELLOWLED4, HIGH);
digitalWrite(REDLED1, HIGH);
digitalWrite(REDLED2, LOW);
digitalWrite(REDLED3, LOW);
break;
case 8:
digitalWrite(GREENLED1, HIGH);
digitalWrite(GREENLED2, HIGH);
digitalWrite(GREENLED3, HIGH);
digitalWrite(YELLOWLED1, HIGH);
digitalWrite(YELLOWLED2, HIGH);
digitalWrite(YELLOWLED3, HIGH);
digitalWrite(YELLOWLED4, HIGH);
digitalWrite(REDLED1, HIGH);
digitalWrite(REDLED2, HIGH);
digitalWrite(REDLED3, LOW);
break;
case 9:
digitalWrite(GREENLED1, HIGH);
digitalWrite(GREENLED2, HIGH);
digitalWrite(GREENLED3, HIGH);
digitalWrite(YELLOWLED1, HIGH);
digitalWrite(YELLOWLED2, HIGH);
digitalWrite(YELLOWLED3, HIGH);
digitalWrite(YELLOWLED4, HIGH);
digitalWrite(REDLED1, HIGH);
digitalWrite(REDLED2, HIGH);
digitalWrite(REDLED3, HIGH);
break;
case 10:
digitalWrite(GREENLED1, HIGH);
digitalWrite(GREENLED2, HIGH);
digitalWrite(GREENLED3, HIGH);
digitalWrite(YELLOWLED1, HIGH);
digitalWrite(YELLOWLED2, HIGH);
digitalWrite(YELLOWLED3, HIGH);
digitalWrite(YELLOWLED4, HIGH);
digitalWrite(REDLED1, HIGH);
digitalWrite(REDLED2, HIGH);
digitalWrite(REDLED3, HIGH);
break;
}
}
#endif
bigPacket = false;
}
else {
// Checksum Error
} // end if else for checksum
} // end if read 0xAA byte
} // end if read 0xAA byte
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


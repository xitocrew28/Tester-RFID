#include <SPI.h>        //<libreria para usar la libreria SPI
#include <MFRC522.h>    //Libreria que incluye el RFID-RC522

/* 
 Conexiones (MFRC522 hacia el ATMEGA328PU
 MFRC522 pin SDA  hacia DIG 9
 MFRC522 pin SCK  hacia DIG 13
 MFRC522 pin MOSI hacia DIG 11
 MFRC522 pin MISO hacia DIG 12
 MFRC522 pin GND  a TIERRA
 MFRC522 pin RST  hacia DIG 10
 MFRC522 pin 3.3V A 3.3. V
*/

// Se definen los pines para el modulo RFID-RC522
#define SDA_PIN  9
#define RST_PIN  10

MFRC522 mfrc522(SDA_PIN, RST_PIN);

#define bien    5
#define mal     7
#define estado  6
#define mus     3

int timer = 50;

void setup() 
{
  // Inicializamos los pines de salida
  pinMode(bien, OUTPUT);
  pinMode(mal, OUTPUT);
  pinMode(mus, OUTPUT);
  pinMode(estado, OUTPUT);  
  
  Serial.begin(115200);         // Iniciamos la comunicacion serial
  
  SPI.begin();                  // Iniciamos el SPI bus
  mfrc522.PCD_Init();       // Iniciamos el modulo MFRC522
  
  // Mensaje Inicial en Monitor Serial
  Serial.println("Aproxime su Tarjeta al Lector....");
  Serial.println();  
}

void loop() 
{
  // Aguardamos un momento para revisar si hay un TAG RFID
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Selecciona una de las tarjetas
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
    
  // Muestra el ID de la tarjeta
  Serial.print("CSN del TAG");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  //Serial.print("Mensaje : ");
  conteudo.toUpperCase();
  
    // Comprobamos si la tarjeta que se ha leido es la correcta
  if (conteudo.substring(1) == "04 5C 41 C2 09 4F 80" )
  {
    Serial.println("Supervisor");
    Serial.println();
    digitalWrite(bien, HIGH);
    beep(200);
    
    for ( int busy = 0 ; busy <= 60 ; busy++)
    {
      delay(timer);
    }
    digitalWrite(bien, LOW);
   } 

   // Comprobamos si la tarjeta que se ha leido es la correcta
  if (conteudo.substring(1) == "04 5A 4A C2 09 4F 80" )
  {
    Serial.println("Personal No Autorizado");
    Serial.println();
    digitalWrite(mal, HIGH);    
    beep(60);
    beep(60);
    beep(60);
    beep(60);
    beep(60);
    
    for ( int busy = 0 ; busy <= 60 ; busy++)
    {
      delay(timer);
    }
    digitalWrite(mal, LOW);
  }

    // Comprobamos si la tarjeta que se ha leido es la correcta
  if (conteudo.substring(1) == "04 40 10 AA 09 4F 80" )
  {
    Serial.println("Chofer");
    Serial.println();
    digitalWrite(estado, HIGH);
    beep(200);

    for (int busy = 0 ; busy <= 60 ; busy++)
    {
      delay(timer);
    }
    digitalWrite(estado, LOW);
  }

    // Comprobamos si la tarjeta que se ha leido es la correcta
  if (conteudo.substring(1) == "04 45 23 AA 09 4F 80" )
  {
    Serial.println("Inspector");
    Serial.println();
    digitalWrite(mal, HIGH);
    digitalWrite(bien, HIGH);
    beep(200);

    for (int busy = 0 ; busy <= 60 ; busy++)
    {
      delay(timer);
    }
    digitalWrite(mal, LOW);
    digitalWrite(bien, LOW);
  }

    // Comprobamos si la tarjeta que se ha leido es la correcta
  if (conteudo.substring(1) == "04 3E 30 C2 09 4F 80" )
  {
    Serial.println("Mantenimiento");
    Serial.println();
    digitalWrite(estado, HIGH);
    digitalWrite(bien, HIGH);
    beep(200);

    for (int busy = 0 ; busy <= 60 ; busy++)
    {
      delay(timer);
    }
    digitalWrite(estado, LOW);
    digitalWrite(bien, LOW);
  }

    // Comprobamos si la tarjeta que se ha leido es la correcta
  if (conteudo.substring(1) == "04 4B 67 C2 09 4F 80" )
  {
    Serial.println("Analista"); 
    Serial.println();
    digitalWrite(estado, HIGH);
    digitalWrite(mal, HIGH);
    beep(200);

    for (int busy = 0 ; busy <= 60 ; busy++)
    {
      delay(timer);
    }
    digitalWrite(estado, LOW);
    digitalWrite(mal, LOW);
  }
    
    // Si leemos otro CSN solamente avisara que no es correcto
  if (  conteudo.substring(1) != "04 4B 67 C2 09 4F 80"
        && conteudo.substring(1) != "04 3E 30 C2 09 4F 80" 
        && conteudo.substring(1) != "04 45 23 AA 09 4F 80"
        && conteudo.substring(1) != "04 40 10 AA 09 4F 80"
        && conteudo.substring(1) != "04 5A 4A C2 09 4F 80"
        && conteudo.substring(1) != "04 5C 41 C2 09 4F 80" )
  {
    Serial.println("Tag No Valido");
    Serial.println();
   
    for ( int intermitente = 0 ; intermitente <= 6 ; intermitente++)
    {
      //digitalWrite(bien, HIGH);
      digitalWrite(mal, HIGH);
      //digitalWrite(estado, HIGH);
      beep(60);
      delay(100);
      //digitalWrite(bien, LOW);
      digitalWrite(mal, LOW);
      //digitalWrite(estado, LOW);
      delay(100);
    }
  }    
}

void beep(unsigned int delayms) 
{
  digitalWrite(mus, 70);
  delay(delayms);
  digitalWrite(mus, 0);
  delay(delayms);
}


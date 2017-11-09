//Initialize the KNX TP-UART library on the Serial1 port of Arduino Mega
// 15,15,20 ist die Source-Adresse

// Fixwerte Festlegen
#include <KnxTelegram.h>
#include <KnxTpUart.h>
#define KNX_PHYSICAL_ADRESS                  	"1.1.121"
#define KNX_WRITE_GROUPADRESS0               	"0/9/0"
#define KNX_WRITE_GROUPADRESS1               	"13/9/5"
#define summer          				      	"0/9/2"
#define led             					    "0/9/1"
#define di1groupadd 							"0/9/3"
#define di2groupadd 							"0/9/4"
#define di3groupadd 							"0/9/5"

// Ausgaenge Festlegen
int SUMMER = 5;
int LED = 4;

//Eingaenge Festlegen
int di1 = 9;
int di2 = 8;
int di3 = 7;

// Initialize the KNX TP-UART library on the Serial1 port of ARDUINO
KnxTpUart knx(&Serial1, KNX_PHYSICAL_ADRESS);


//SETUP
void setup()
{
  //Serieller Ports//
  Serial.begin(9600);
  Serial.println("Hello World");

  //LED und SUMMER
  pinMode(LED, OUTPUT);
  pinMode(SUMMER, OUTPUT);
  digitalWrite(LED, HIGH);
  digitalWrite(SUMMER, HIGH);

  //Setup Hilfen
  WiegandSetup();
  diSetup();

  // KNX Initialisieren
  //Serial.begin(19200); // Arduino Nano
  Serial1.begin(19200); // Arduino Micro Pro
  UCSR1C = UCSR1C | B00100000; //Arduino Micro Pro
  //UCSR0C = UCSR0C | B00100000; // Arduino Nano

  // Uart Reset (muss immer)
  knx.uartReset();
  // Hörende Adressen Aktivieren
  knx.addListenGroupAddress(summer);
  knx.addListenGroupAddress(led);
  knx.addListenGroupAddress(di1groupadd);
  knx.addListenGroupAddress(di2groupadd);
  knx.addListenGroupAddress(di3groupadd);
  // Setup Beenden
  Serial.println("Setup Complet");

}


// Main Loop - Lesen der RFID´s und Senden
void loop()
{
  // Starte den KNX LOOP
  loopKNX();
  // Initialisieren 2 Variablen
  unsigned long ulCardCode, ulFacilityCode;

  // Sind Karten da?
  if (WiegandAvailable())
  {
    // Sind mehr als 0 Bit zum Senden Vorhanden
    if (WiegandBitCount() != 0)
    {
      ulFacilityCode = WiegandFacilityCode();
      ulCardCode = WiegandCardCode();
      String CardNo = String (ulCardCode, DEC) + String (ulFacilityCode, DEC);
      Serial.print(F("Read "));
      Serial.print(WiegandBitCount());
      Serial.print(F(" bits. "));
      Serial.print(F("FacilityCode,CardCode = "));
      Serial.print(ulFacilityCode);
      Serial.print(F(","));
      Serial.println(ulCardCode);
      Serial.print("CardNo");
      Serial.print(CardNo);

      knx.groupWrite14ByteText(KNX_WRITE_GROUPADRESS0, CardNo );

    }
    else
    {
      Serial.println(F("Cannot decode. Unknown format."));
    }

    // clean up and get ready for the next card
    WiegandReset();
  }

}

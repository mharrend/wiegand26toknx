#include <Bounce2.h>
//Eingangspin DeBouncing
Bounce dedi1 = Bounce ();
Bounce dedi2 = Bounce ();
Bounce dedi3 = Bounce ();
//Haben wir gesendet?
boolean di1haveSent = false;
boolean di2haveSent = false;
boolean di3haveSent = false;
//Was wurde gesendet?
boolean di1onSent = false;
boolean di2onSent = false;
boolean di3onSent = false;
//Zeit wann das letze mal gesendet wurde
unsigned long di1sendTime;
unsigned long di2sendTime;
unsigned long di3sendTime;
// Sendet beim Start Aktuellen Status
boolean startdi1 = true;
boolean startdi2 = true;
boolean startdi3 = true;




void diSetup ()
{
  pinMode(di1, INPUT_PULLUP);
  dedi1.attach(di1);
  dedi1.interval(50);
  pinMode(di2, INPUT_PULLUP);
  dedi2.attach(di2);
  dedi2.interval(50);
  pinMode(di3, INPUT_PULLUP);
  dedi3.attach(di3);
  dedi3.interval(50);
}


//Eingänge Senden
void loopKNX()
{
  dedi1.update();
  digital1();
  dedi2.update();
  digital2();
  dedi3.update();
  digital3();
}

// Loops der Eingänge
void digital1()
{
  if (dedi1.read() == LOW) {
    if (di1haveSent ==  false) {
      bool success = knx.groupWriteBool(di1groupadd, !di1haveSent);
      di1haveSent = true;
      di1sendTime = millis();
    }
  }
  if (dedi1.read() == HIGH) {
    if (di1haveSent ==  true) {
      bool success = knx.groupWriteBool(di1groupadd, !di1haveSent);
      di1haveSent = false;
      di1sendTime = millis();
    }
  }
  if (millis() - di1sendTime >= 600000){
    di1haveSent = !di1haveSent;
}		
}
void digital2()
{
  if (dedi2.read() == LOW) {
    if (di2haveSent ==  false) {
      bool success = knx.groupWriteBool(di2groupadd, !di2haveSent);
      di2haveSent = true;
      di2sendTime = millis();
    }
  }
  if (dedi2.read() == HIGH) {
    if (di2haveSent ==  true) {
      bool success = knx.groupWriteBool(di2groupadd, !di2haveSent);
      di2haveSent = false;
      di2sendTime = millis();
    }
  }
  if (millis() - di2sendTime >= 600000)
    di2haveSent = !di2haveSent;
}

void digital3()
{
  if (dedi3.read() == LOW) {
    if (di3haveSent ==  false) {
      bool success = knx.groupWriteBool(di3groupadd, !di3haveSent);
      di3haveSent = true;
      di3sendTime = millis();
    }
  }
  if (dedi3.read() == HIGH) {
    if (di3haveSent ==  true) {
      bool success = knx.groupWriteBool(di3groupadd, !di3haveSent);
      di3haveSent = false;
      di3sendTime = millis();
    }
  }
  if (millis() - di3sendTime >= 600000)
    di3haveSent = !di3haveSent;
}

// Hörende Gruppenadressen aktionen
void serialEvent1() {
  KnxTpUartSerialEventType eType = knx.serialEvent();
  if (eType == TPUART_RESET_INDICATION) {

  }
  else if (eType == UNKNOWN) {
    Serial.println("Event UNKNOWN");
  }
  else if (eType == KNX_TELEGRAM) {
    Serial.println("Event KNX_TELEGRAM");
    KnxTelegram* telegram = knx.getReceivedTelegram();
    // Telegrammauswertung auf KNX (bei Empfang immer notwendig)
    String target =
      String(0 + telegram->getTargetMainGroup())   + "/" +
      String(0 + telegram->getTargetMiddleGroup()) + "/" +
      String(0 + telegram->getTargetSubGroup());

    // Here you have the telegram and can do whatever you want
    if (telegram->getCommand() == KNX_COMMAND_WRITE) {
      // Auswertung des empfangenen KNX-Telegrammes mit Schreibbefehl (Flag) -> Aktion
      if (target == summer) {
        int received_13_1_1 = telegram->getBool();
        Serial.print("Empfangener wert");
        Serial.println(received_13_1_1);
        if (received_13_1_1) {
          digitalWrite(SUMMER, LOW);
        }
        else {
          digitalWrite(SUMMER, HIGH);
        }
      }
      if (target == led) {
        int received_13_1_2 = telegram->getBool();
        Serial.print("Empfangener wert");
        Serial.println(received_13_1_2);
        if (received_13_1_2) {
          digitalWrite(LED, LOW);
        }
        else {
          digitalWrite(LED, HIGH);
        }
      }
    }
	if (telegram->getCommand() == KNX_COMMAND_READ) { 
	// Auf leseanforderung Reagieren
		if (target == di1groupadd) {
		knx.groupAnswerBool(di1groupadd, !di1haveSent);
		}
		if (target == di2groupadd) {
		knx.groupAnswerBool(di2groupadd, !di2haveSent);
		}
		if (target == di3groupadd) {
		knx.groupAnswerBool(di3groupadd, !di3haveSent);
		}
	}
  }
}



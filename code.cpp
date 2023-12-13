#include "Adafruit_Fingerprint.h"
#include "SoftwareSerial.h"

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int cc = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("\n\nAdafruit finger detect test");

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      pinMode(1, OUTPUT);
      digitalWrite(1, LOW);
      pinMode(0, OUTPUT);
      digitalWrite(0, HIGH);
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains ");
  Serial.print(finger.templateCount);
  Serial.println(" templates");
  Serial.println("Waiting for a valid finger...");
}

void loop() {
  getFingerprintIDez();
  delay(50); // Don't need to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();

  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  p = finger.image2Tz();

  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  p = finger.fingerFastSearch();

  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);

  for (int i = 255; i > 1; i--) {
    analogWrite(9, i);
    delay(10);
  }

  if (cc % 2 == 0)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);

  cc++;

  return finger.fingerID;
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();

  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.image2Tz();

  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.fingerFastSearch();

  if (p != FINGERPRINT_OK)
    return -1;

  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);

  return finger.fingerID;
}

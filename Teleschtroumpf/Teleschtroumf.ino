#include <WiFi.h>
#include <webserver.h>

#define SSID "Teleschtroumpf"
void setup() {
  Serial.begin(115200);
  NimBLEDevice::init(ESP_NAME);

  NimBLEServer* pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  NimBLEService* pService = pServer->createService(SERVICE_UUID);

  pTxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_TX,
    NIMBLE_PROPERTY::NOTIFY
  );

  NimBLECharacteristic* pRxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_RX,
    NIMBLE_PROPERTY::WRITE
  );
  pRxCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();

  // Configuration explicite de l'advertising
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  
  NimBLEAdvertisementData scanResponse;
  scanResponse.setName(ESP_NAME);
  pAdvertising->setScanResponseData(scanResponse);
  
  pAdvertising->start();

  Serial.print("Services en ligne au nom de ");
  Serial.println(ESP_NAME);
  Serial.print("Adresse MAC : ");
  Serial.println(NimBLEDevice::getAddress().toString().c_str());
  Serial.println("En attente de connexion BLE...");

  Serial.println(NIMBLE_CPP_VERSION);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (deviceConnected) {
    // Envoyer des données vers l'app
    String msg = "Hello depuis ESP32-S3 : " + String(millis()) + "\n";
    pTxCharacteristic->setValue(msg.c_str());
    pTxCharacteristic->notify();
    delay(1000);
  }
/*
  if (SerialBT.available()) {
    morsecode = "";
    text = SerialBT.read();
    if (text.length() > 0){
      for (int i = 0; i < text.length(); i++){
        morsecode += convertCharToMorse(char(text[i]));
      }
    }
    Serial.println(morsecode);
  }*/
}

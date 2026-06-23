#include "Server_callbacks.h"

// Server_callbacks.ino
void MyServerCallbacks::onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) {
  deviceConnected = true;
  Serial.println("Connecté");
}

void MyServerCallbacks::onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) {
  deviceConnected = false;
  Serial.println("Déconnecté");
  NimBLEDevice::startAdvertising();
}

void MyCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
  std::string value = pCharacteristic->getValue();
  Serial.println(value.c_str());
}
#pragma once
#include <NimBLEDevice.h>

extern NimBLECharacteristic* pTxCharacteristic;
extern bool deviceConnected;

// Server_callbacks.h
class MyServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo);
  void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason);
};

class MyCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo);
};
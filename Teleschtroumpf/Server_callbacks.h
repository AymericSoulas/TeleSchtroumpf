#pragma once
#include <WebServer.h>

extern WebServer server;
extern SemaphoreHandle_t mutex_message;
extern String current_message;
extern volatile bool new_message;

void handleRoot();
void handleMorse();
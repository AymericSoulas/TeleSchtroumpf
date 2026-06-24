#include "Tasks.h"

void tacheWiFi(void* parameter) {
  // tourne en boucle sur Core 0
  for (;;) {
    server.handleClient();
    vTaskDelay(1 / portTICK_PERIOD_MS);  // yield obligatoire !
  }
}
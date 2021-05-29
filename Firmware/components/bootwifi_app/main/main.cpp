// Created by Oleksandra Baga on 07.03.18.
// Eptecon Berlin

#include "BootWiFi.h"
#include "sdkconfig.h"

extern "C" {
void app_main(void);
}

BootWiFi *boot;

void app_main(void) {
    boot = new BootWiFi();
    boot->boot();
}

//
// Created by Oleksandra Baga on 05.02.18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "driver/spi_master.h"


void set_vref_loop(spi_device_handle_t spi, int vref, int gain){

    // creating the CW + Reference Voltage
    uint16_t data = ((0x7 & 0xF) << 12) | (vref & 0xFFF);
    // solving the issue with big endian and little endian
    data = ((data & 0xFF) << 8) | ((data >> 8) & 0xFF);
    // calculating decimal value V_out, PIN 8, MCP4921
    double V_out = (2.3 * vref * gain) / 4096;


    printf("To transmit HEX: %x\n", data);
    printf("V_out = %.2f\n", V_out);

    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t)); //Zero out the transaction
    t.length = 16;                  //Len is in bytes, transaction length is in bits.
    t.rx_buffer = NULL;
    t.tx_buffer = &data;            //Data
    t.user = (void *) 1;

    // Transmit the CW
    ret = spi_device_transmit(spi, &t);
    assert(ret == ESP_OK);            //Should have had no issues.
    printf("Transmitted\n\n");
}

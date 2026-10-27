/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_types.h"
#include "xil_printf.h"
#include "xil_io.h"

#define RED1 *((volatile uint32_t *)0x44A00000)
#define GREEN1 *((volatile uint32_t *)0x44A00004)
#define BLUE1 *((volatile uint32_t *)0x44A00008)

#define SERVO *((uint32_t *)0x44A00018)

#define DIV *((volatile uint32_t *)0x44A0001C)
#define WID *((volatile uint32_t *)0x44A00020)

#define SW *((volatile uint32_t *)0x40000000)

void rgb_set_color(uint8_t red, uint8_t green, uint8_t blue)
{
    RED1 = red;
    GREEN1 = green;
    BLUE1 = blue;
}

uint8_t hex_char_to_val(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';       // '0'-'9' maps to 0-9
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;  // 'A'-'F' maps to 10-15
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;  // 'a'-'f' maps to 10-15
    }
    return 0;
}

int main()
{

    DIV = 5;
    WID = 1024;

    init_platform();

    print("Hello World\n\r");
    print("Successfully ran Hello World application\n");

    
    print("Enter a String: ");
    char buffer[32];
    int i = 0;

    while(i < sizeof(buffer) - 1)
    {
        char c = inbyte();

        if(c == '\r' || c == '\n') break;

        buffer[i++] = c;
    }


    buffer[i] = '\0';
    xil_printf("\nYour String: %s\n", buffer);

    while(1)
    {
        print("\nEnter RGB1 Values in Hex: ");
        i = 0;
        while(i < sizeof(buffer) - 1)
        {
            char c = inbyte();

            if(c == '\r' || c == '\n') break;

            buffer[i++] = c;
        }

        buffer[i] = '\0';

        uint8_t red, blue, green;

        red = (hex_char_to_val(buffer[0]) << 4) | hex_char_to_val(buffer[1]);
        green = (hex_char_to_val(buffer[2]) << 4) | hex_char_to_val(buffer[3]);
        blue = (hex_char_to_val(buffer[4]) << 4) | hex_char_to_val(buffer[5]);

        rgb_set_color(red, green, blue);
    }
        

    cleanup_platform();
    return 0;
}

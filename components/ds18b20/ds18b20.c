/* 
 * File:   DS18B20.c
 * Author: Jason McGuire (j.mcguire.2015@ieee.org)
 * Description: A library for the DS18B20 Temperature Sensor
 * Using the Dallas OneWire communication protocol.
 * See example.c for implementation of the library
 * Sample Product: https://www.sparkfun.com/products/11050
 * Created on April 20, 2016, 9:08 AM
 
License Information:
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 
 
 */
 
#include "ds18b20.h"
#include "driver/gpio.h"

#include "xtensa/hal.h"

#define ONEWIRE_GPIO 4

void __delay_us( unsigned int delay_ )
{
	unsigned int count_start, count_end;
	unsigned int delta = delay_ *  160;
	count_start = xthal_get_ccount();
	count_end = count_start;

	while ( (count_end - count_start ) < delta )
	{
		count_end = xthal_get_ccount();
	}
}

static uint8_t dsCRC8(const uint8_t *addr, uint8_t len)//begins from LS-bit of LS-byte (OneWire.h)
{
    uint8_t crc = 0;
    while (len--)
    {
        uint8_t inbyte = *addr++;
        for (uint8_t i = 8; i; i--)
        {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}

uint8_t OneWireTemp(float * readout){

    unsigned char scratchpad_mem[9];
    
    OneWireReset(); // Reset Pulse 
    OneWireWriteByte(0xCC); // Issue skip ROM command (CCh)
    OneWireWriteByte(0x44); // Convert T command (44h)
    while(!gpio_get_level(ONEWIRE_GPIO)); // DS will hold line low while making measurement
    OneWireReset(); // Start new command sequence 
    OneWireWriteByte(0xCC); // Issue skip ROM command 
    OneWireWriteByte(0xBE); // Read Scratchpad (BEh) - 15 bits
    for(unsigned int i=0; i<9; i++)
    {
        scratchpad_mem[i] = OneWireReadByte();
    }
    unsigned char LSB = scratchpad_mem[0];
    unsigned char MSB = scratchpad_mem[1];
    OneWireReset(); // Stop Reading 
    unsigned int data = MSB;
    float temperature = (data << 8) | LSB;

    if (dsCRC8(scratchpad_mem, 8) == scratchpad_mem[8]) 
    {
        *readout = temperature/16;
        return 0;
    }
    return 1; 
}

void OneWireHigh(){
    gpio_set_direction(ONEWIRE_GPIO, GPIO_MODE_INPUT);
}


void OneWireRelease(){
    gpio_set_direction(ONEWIRE_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(ONEWIRE_GPIO, 0);
}

unsigned char OneWireRead(){
    return gpio_get_level(ONEWIRE_GPIO);
}

unsigned int OneWireReset(){
    OneWireRelease();
    __delay_us(240); // 480uS Delay
    __delay_us(240);
    __delay_us(240);
    OneWireHigh();
    __delay_us(70); // wait 70 uS before reading
    __delay_us(70); // wait 70 uS before reading
    __delay_us(70); // wait 70 uS before reading
    unsigned int OW = OneWireRead(); // check for OneWire
    __delay_us(205); // 410 uS delay
    __delay_us(205);
    OneWireHigh(); // give bus back to OneWire
    return OW; 
}


void OneWireWriteBit(unsigned char b){
    if(b){
	OneWireRelease();
	__delay_us(6); // wait 6uS
	OneWireHigh();
	__delay_us(64); // wait 64uS
    }
    else{
	OneWireRelease();
	__delay_us(60); // wait 60uS
	OneWireHigh();
	__delay_us(10); // wait 10uS
    }
}

unsigned char OneWireReadBit(){
    OneWireRelease();
    __delay_us(6); // wait 6uS
    OneWireHigh();
    __delay_us(9); // wait 9uS
    unsigned char out = OneWireRead();
    __delay_us(55); // wait 55uS
    return out;
}

void OneWireWriteByte(unsigned char b){
    for(int i = 0; i < 8; i++){
	OneWireWriteBit(b & 0x01); // send LS bit first 
	b = b >> 1;
    }
}

unsigned char OneWireReadByte(void){
    unsigned char out=0; 
    for(int i = 0; i < 8; i++){ // read in LS bit first
	out = out >> 1; // get out ready for next bit
	if(OneWireReadBit() & 0x01) // if its a one 
	    out = out | 0x80; // place a 1 
    }
    return out;
}

void OneWireInit(void)
{
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO15/16
    io_conf.pin_bit_mask = 0x10;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}

/*
 * SPI.h
 *
 * Created: 11/2/2018 10:54:46 PM
 *  Author: jlarr
 */ 


#ifndef SPI_H_
#define SPI_H_


void HardwareReset(void);

void SpiInit(void);

void SPI_transmit_char(uint8_t data);

char SPI_receive_char(void);

uint8_t CommandSend(uint8_t data);

#endif /* SPI_H_ */
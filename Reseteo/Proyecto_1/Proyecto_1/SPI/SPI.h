#ifndef INCFILE1_H_
#define INCFILE1_H_

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

void HardwareReset(void);
void SpiInit(void);
uint8_t CommandSend(uint8_t data);
void SPI_transmit_char(uint8_t data);
char SPI_receive_char(void);

#endif /* INCFILE1_H_ */
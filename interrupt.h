#ifndef __TIMER_INTERRUPT_H__
#define __TIMER_INTERRUPT_H__
void timer0_init();
void UART_Init();
void Ext_Init();
void UART_SendByte(unsigned char Byte);
void UART_SendString(unsigned char *String);
#endif
#ifndef __TIMER_INTERRUPT_H__
#define __TIMER_INTERRUPT_H__
void timer0_init();
void UART_Init();
void Ext_Init();
void UART_SendByte(unsigned char Byte);
void UART_SendString(unsigned char *String);
void timer2_init();
void UART_main();
#endif
#ifndef __TIMER_INTERRUPT_H_V_
#define __TIMER_INTERRUPT_H_V_
extern volatile unsigned char pwmCount,pwmDuty;
#endif
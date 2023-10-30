

#ifndef INC_CORTEXMX_PORTING_H_
#define INC_CORTEXMX_PORTING_H_

extern u32 _estack;
extern u32 _eheap;
#define MAIN_STACK_SIZE 	3072
									/*Macros*/
#define OS_SET_PSP_ADDRESS(add) 		 __asm volatile("mov r0,%0 \n\t msr PSP,r0" : : "r" (add) )
#define OS_GET_PSP_ADDRESS(add) 		 __asm volatile("mrs r0,PSP \n\t mov %0,r0"   : "=r" (add) )
#define OS_SP_SHADOWS_PSP				 __asm volatile("mrs r0,CONTROL \n\t mov r1,#0x02 \n\t orr r0,r0,r1 \n\t msr CONTROL,r0")
#define OS_SP_SHADOWS_MSP		 		 __asm volatile("mrs r0,CONTROL \n\t mov r1,#0x05 \n\t and r0,r0,r1 \n\t msr CONTROL,r0")
#define OS_SWITCH_PRIVILIGE_MODE		 __asm volatile("MRS R3,CONTROL \n\t LSR R3,R3,#0x1 \n\t LSL R3,R3,#0x1 \n\t MSR CONTROL,R3")
#define OS_SWITCH_UNPRIVILIGE_MODE	 __asm volatile("MRS R3,CONTROL \n\t orr r3,r3,#0x01 \n\t MSR CONTROL,R3")

void CORTEXMXPORTING_voidInit();
void CORTEXMXPORTING_voidIssuePendSv(void);
void CORTEXMXPORTING_voidPendSvSetCallBack(void(*func)(void));
void CORTEXMXPORTING_voidSystickSetCallBack(void(*func)(void));
void CORTEXMXPORTING_voidStartSystick(u32 ticks);
#endif

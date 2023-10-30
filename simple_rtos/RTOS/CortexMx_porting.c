#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "CortexMx_porting.h"
#include "core_cm3.h"


void HardFault_Handler(void)
{
	while(1);
}
void MemManage_Handler(void)
{
	while(1);
}
void BusFault_Handler(void)
{
	while(1);
}
void UsageFault_Handler(void)
{
	while(1);
}


__attribute((naked)) void SVC_Handler(void)
{
	__asm("TST lr,#0x4 \n\t"
			"ITE EQ \n\t"
			"MRSEQ r0,MSP \n\t"
			"MRSNE r0,PSP \n\t"
			"B OS_voidSVC");
}






void CORTEXMXPORTING_voidIssuePendSv()
{
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void CORTEXMXPORTING_voidStartSystick(u32 ticks)
{
	SysTick_Config(ticks);
}

void CORTEXMXPORTING_voidInit()
{
	/*init clock tree (systick 8 MHZ) */

	/*make the pendsv priorty less than systick*/
	__NVIC_SetPriority(PendSV_IRQn, 15);

}

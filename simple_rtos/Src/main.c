

#include <STD_TYPES.h>
#include "BIT_MATH.h"
#include <string.h>
#include "Scheduler.h"

MUTEX_Control_t MUTEX1;
int arr[]={1,2,4};
u8 LED1,LED2,LED3,LED4;
TASK_Control_Block T1,T2,T3,T4;
void TASK1()
{

	while(1)
	{

		static u32 c2 =0;
		while(1)
		{
			if(c2==100)
			{
				OS_voidActivateTask(&T2);
			}
			else if(c2==200)
			{
				c2=0;
			}
			c2++;
			LED1^=1;
			__asm("NOP");

		}
	}
}
void TASK2()
{
	static u32 c2 =0;
	while(1)
	{
		c2++;
		LED2^=1;
		__asm("NOP");
		if(c2==100)
		{
			OS_voidActivateTask(&T3);
		}
		if(c2==200)
		{
			c2=0;
			OS_voidTerminateTask(&T2);
		}



	}
}
void TASK3()
{
	static u32 c2 =0;
	while(1)
	{
		c2++;
		LED3^=1;
		__asm("NOP");
		if(c2==100)
		{
			OS_voidActivateTask(&T4);
		}
		if(c2==200)
		{
			c2=0;
			OS_voidTerminateTask(&T3);
		}



	}
}

void TASK4()
{
	static u32 c2 =0;
	while(1)
	{
		c2++;
		LED4^=1;
		__asm("NOP");
		if(c2==200)
		{
			c2=0;
			OS_voidTerminateTask(&T4);
		}

	}
}



int main()
{
	OS_Init();


	T1.TASK_Config.TASK_Priority=4;
	T1.TASK_Config.TASK_Stack_Size=300;
	T1.TASK_Config.TASK_AutoStart=0;
	T1.TASK_Config.TASK_Entry = TASK1;

	strcpy((char*)T2.TASK_Config.TASK_Name,"T2");
	T2.TASK_Config.TASK_Priority=3;
	T2.TASK_Config.TASK_Stack_Size=300;
	T2.TASK_Config.TASK_AutoStart=0;
	T2.TASK_Config.TASK_Entry = TASK2;

	strcpy((char*)T3.TASK_Config.TASK_Name,"T3");
	T3.TASK_Config.TASK_Priority=2;
	T3.TASK_Config.TASK_Stack_Size=300;
	T3.TASK_Config.TASK_AutoStart=0;
	T3.TASK_Config.TASK_Entry = TASK3;

	strcpy((char*)T4.TASK_Config.TASK_Name,"T4");
	T4.TASK_Config.TASK_Priority=1;
	T4.TASK_Config.TASK_Stack_Size=300;
	T4.TASK_Config.TASK_AutoStart=0;
	T4.TASK_Config.TASK_Entry = TASK4;

	OS_voidCreateTask(&T1);
	OS_voidCreateTask(&T2);
	OS_voidCreateTask(&T3);
	OS_voidCreateTask(&T4);
	OS_voidActivateTask(&T1);

	OS_voidStart();
	while(1);

	return 0;
}

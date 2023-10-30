#include <Fifo.h>
#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include <string.h>
#include "CortexMx_porting.h"
#include "Fifo.h"
#include "Scheduler.h"
#include "Scheduler_private.h"


static OS_Control_t OS_Control_Block;
static Fifo_Buffer_t OS_Ready_Queue;
static TASK_Control_Block *OS_Ready[100];
static TASK_Control_Block OS_IdleTask;


static void OS_voidBubblSort(void)
{
	u32 i,j;
	TASK_Control_Block *temp;
	for(i=0;i<(OS_Control_Block.OS_NoOfTasks-1);i++)
	{
		for(j=0;j<(OS_Control_Block.OS_NoOfTasks-1);j++)
		{
			if((OS_Control_Block.OS_Tasks[j]->TASK_Config.TASK_Priority)>(OS_Control_Block.OS_Tasks[j+1]->TASK_Config.TASK_Priority))
			{
				temp = OS_Control_Block.OS_Tasks[j];
				OS_Control_Block.OS_Tasks[j]=OS_Control_Block.OS_Tasks[j+1];
				OS_Control_Block.OS_Tasks[j+1]=temp;
			}
		}
	}
}


static void OS_voidUpdateSchedulerTable(void)
{
	u8 LOC_GetOneFlag = 0;
	u8 LOC_TempPriority;
	u32 i=0;
	void* LOC_pDummyData;
	//bubble sort the OS_Tasks array
	OS_voidBubblSort();
	//empty the ready queue
	while(FIFO_dequeue(&OS_Ready_Queue,&LOC_pDummyData)!=FIFO_EMPTY);
	//fill the ready queue
	while(i<(OS_Control_Block.OS_NoOfTasks))
	{

		if((OS_Control_Block.OS_Tasks[i]->TASK_State!=TASK_Suspended)&&(LOC_GetOneFlag==0))
		{
			FIFO_enqueue(&OS_Ready_Queue,(void*)OS_Control_Block.OS_Tasks[i]);
			OS_Control_Block.OS_Tasks[i]->TASK_State=TASK_Ready;
			LOC_TempPriority = OS_Control_Block.OS_Tasks[i]->TASK_Config.TASK_Priority;
			LOC_GetOneFlag = 1;
		}
		else if((LOC_GetOneFlag==1)&&(OS_Control_Block.OS_Tasks[i]->TASK_Config.TASK_Priority==LOC_TempPriority))
		{
			if((OS_Control_Block.OS_Tasks[i]->TASK_State!=TASK_Suspended))
			{
				FIFO_enqueue(&OS_Ready_Queue,(void*)OS_Control_Block.OS_Tasks[i]);
				OS_Control_Block.OS_Tasks[i]->TASK_State=TASK_Ready;
			}
		}
		else if((LOC_GetOneFlag==1)&&(OS_Control_Block.OS_Tasks[i]->TASK_Config.TASK_Priority>LOC_TempPriority))
		{
			break;
		}

		i++;
	}
}

static void OS_voidNextTask(void)
{
	if(OS_Ready_Queue.counter==0&&OS_Control_Block.OS_Current_Task->TASK_State!=TASK_Suspended)
	{
		FIFO_enqueue(&OS_Ready_Queue,(void*)OS_Control_Block.OS_Current_Task);
		OS_Control_Block.OS_Next_Task = OS_Control_Block.OS_Current_Task;

	}
	else
	{
		FIFO_dequeue(&OS_Ready_Queue,(void**)&OS_Control_Block.OS_Next_Task);
		OS_Control_Block.OS_Next_Task->TASK_State = TASK_Running;
		if((OS_Control_Block.OS_Next_Task->TASK_Config.TASK_Priority)==(OS_Control_Block.OS_Current_Task->TASK_Config.TASK_Priority)&&(OS_Control_Block.OS_Current_Task->TASK_State!=TASK_Suspended))
		{
			OS_Control_Block.OS_Current_Task->TASK_State=TASK_Ready;
			FIFO_enqueue(&OS_Ready_Queue,(void*)OS_Control_Block.OS_Current_Task);
		}

	}
}

void OS_voidUpdateTasksWaitingTime(void)
{
	u32 i =0;
	for(i=0;i<(OS_Control_Block.OS_NoOfTasks);i++)
	{
		if(OS_Control_Block.OS_Tasks[i]->TASK_State==TASK_Suspended)
		{
			if(OS_Control_Block.OS_Tasks[i]->TASK_Waiting_State.TASK_Blocking_State==TASK_BLOCKING_EN)
			{
				OS_Control_Block.OS_Tasks[i]->TASK_Waiting_State.TASK_Blocking_Ticks_Count--;
				if(OS_Control_Block.OS_Tasks[i]->TASK_Waiting_State.TASK_Blocking_Ticks_Count==0)
				{
					OS_Control_Block.OS_Tasks[i]->TASK_Waiting_State.TASK_Blocking_State=TASK_BLOCKING_DIS;
					OS_Control_Block.OS_Tasks[i]->TASK_State=TASK_Waiting;
					OS_voidSvcSetService(SVC_WAIT);
				}
			}
		}

	}
}


u8 systickled=0;
void SysTick_Handler(void)
{
	systickled=1;
	OS_voidUpdateTasksWaitingTime();
	OS_voidNextTask();
	CORTEXMXPORTING_voidIssuePendSv();
	systickled=0;
}


void OS_voidSVC(u32 *stackframe)
{
	u32 service= *((u8*)((u8*)stackframe[6])-2);
	switch (service)
	{
	case SVC_ACTIVATE:
	case SVC_TERMINATE:
		OS_voidUpdateSchedulerTable();
		/*check if th os in the running mode*/
		if(OS_Control_Block.OS_State==OS_Running)
		{
			if(strcmp((char*)OS_Control_Block.OS_Current_Task->TASK_Config.TASK_Name,"IDLE_TASK")!=0)
			{
				/*decide what next*/
				OS_voidNextTask();

				/*issue pendsv*/
				CORTEXMXPORTING_voidIssuePendSv();
			}
		}

		break;
	case SVC_WAIT:
		//systick is waiting svc to updae the table then systick will run the appropriate task
		OS_voidUpdateSchedulerTable();
		break;
	}

}


/*still Thread mode*/
static void OS_voidSvcSetService(u32 Copy_ServiceId)
{
	switch(Copy_ServiceId)
	{
	case SVC_ACTIVATE:
		__asm("SVC #0x00");
		break;
	case SVC_TERMINATE:
		__asm("SVC #0x01");
		break;
	case SVC_WAIT:
		__asm("SVC #0x02");
		break;
	}

}


__attribute((naked)) void PendSV_Handler(void)
{
	/************************************************/
	/******save the context of the current task******/
	/************************************************/
	/*1-get the psp of the current task*/
	OS_GET_PSP_ADDRESS((OS_Control_Block.OS_Current_Task->_C_TASK_Psp));
	/*2-context saveof the register that haven't been saved in the context (r4....r11)*/
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp--;
	__asm("mov %0,r4":"=r"(*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp--;
	__asm volatile("mov %0,r5":"=r"(*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp--;
	__asm volatile("mov %0,r6":"=r"(*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp--;
	__asm volatile("mov %0,r7":"=r"(*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp--;
	__asm volatile("mov %0,r8":"=r"(*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp--;
	__asm volatile("mov %0,r9":"=r"(*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp--;
	__asm volatile("mov %0,r10":"=r"(*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp--;
	__asm volatile("mov %0,r11":"=r"(*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));

	if(OS_Control_Block.OS_Next_Task!=NULL)
	{
		OS_Control_Block.OS_Current_Task =  OS_Control_Block.OS_Next_Task;
		OS_Control_Block.OS_Next_Task=NULL;
	}
	/***************************************************/
	/******restore the context of the next task*********/
	/***************************************************/
	/*1-*/
	__asm volatile("mov r11,%0 " : : "r" (*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp++ ;
	__asm volatile("mov r10,%0 " : : "r" (*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp++ ;
	__asm volatile("mov r9,%0 " : : "r" (*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp++ ;
	__asm volatile("mov r8,%0 " : : "r" (*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp++ ;
	__asm volatile("mov r7,%0 " : : "r" (*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp++ ;
	__asm volatile("mov r6,%0 " : : "r" (*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp++ ;
	__asm volatile("mov r5,%0 " : : "r" (*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp++ ;
	__asm volatile("mov r4,%0 " : : "r" (*(OS_Control_Block.OS_Current_Task->_C_TASK_Psp)));
	OS_Control_Block.OS_Current_Task->_C_TASK_Psp++ ;
	OS_SET_PSP_ADDRESS((OS_Control_Block.OS_Current_Task->_C_TASK_Psp));

	__asm volatile("BX LR");
}

u8 IDLEled;
static void OS_voidIdleTask(void)
{
	IDLEled=0;
	while(1)
	{
		IDLEled^=1;
		__asm("wfe");
	}

}

static void OS_voidCreateMainStack(void)
{
	OS_Control_Block._S_OS_Msp =(u32)&_estack;
	OS_Control_Block._E_OS_Msp = (OS_Control_Block._S_OS_Msp - MAIN_STACK_SIZE);
	OS_Control_Block.OS_Locator = (OS_Control_Block._E_OS_Msp - 8);

}




static void OS_voidCreateTaskStack(TASK_Control_Block* Copy_pTcb)
{
	/*Task Frame
	 * ======
	 * XPSR
	 * PC (Next Task Instruction which should be Run)
	 * LR (return register)
	 * r12
	 * r4
	 * r3
	 * r2
	 * r1
	 * r0
	 *====
	 *r5, r6 , r7 ,r8 ,r9, r10,r11 (Saved/Restore)Manual
	 */
	Copy_pTcb->_C_TASK_Psp =(u32*)Copy_pTcb->_S_TASK_Psp;

	Copy_pTcb->_C_TASK_Psp--;
	/*push dummy value for XPSR*/
	*(Copy_pTcb->_C_TASK_Psp) = 0x01000000; /*T bit should be 1 (thumb2)*/

	Copy_pTcb->_C_TASK_Psp--;
	/*push pc which is the pointer to the task*/
	*(Copy_pTcb->_C_TASK_Psp) =(u32) Copy_pTcb->TASK_Config.TASK_Entry;

	Copy_pTcb->_C_TASK_Psp--;
	/*push lr with dummy value*/
	*(Copy_pTcb->_C_TASK_Psp) = 0xFFFFFFFD;

	/*push other registers values with 0*/
	for (int  j=0 ; j< 13 ; j++ )
	{
		Copy_pTcb->_C_TASK_Psp--;
		*(Copy_pTcb->_C_TASK_Psp) = 0;
	}

}


OS_Error_t OS_voidActivateTask(TASK_Control_Block* Copy_pTcb)
{
	OS_Error_t OS_voidActivateTaskState = OS_NO_ERROR;
	if(Copy_pTcb->TASK_State!=TASK_Suspended)
	{
		OS_voidActivateTaskState = OS_TASK_ALREADY_ACTIVE;
	}
	else
	{
		Copy_pTcb->TASK_State =TASK_Waiting;
		OS_voidSvcSetService(SVC_ACTIVATE);
	}

	return OS_voidActivateTaskState;
}


OS_Error_t OS_voidTerminateTask(TASK_Control_Block* Copy_pTcb)
{
	OS_Error_t OS_voidTerminateTaskState = OS_NO_ERROR;
	if(Copy_pTcb->TASK_State==TASK_Suspended)
	{
		OS_voidTerminateTaskState = OS_TASK_ALREADY_TERMINATED;
	}
	else
	{
		Copy_pTcb->TASK_State =TASK_Suspended;
		OS_voidSvcSetService(SVC_TERMINATE);
	}


	return  OS_voidTerminateTaskState;
}


void OS_voidWaitTask(TASK_Control_Block* Copy_pTcb,u32 Copy_NumberOfMs)
{
	Copy_pTcb->TASK_State= TASK_Suspended;
	Copy_pTcb->TASK_Waiting_State.TASK_Blocking_State = TASK_BLOCKING_EN;
	Copy_pTcb->TASK_Waiting_State.TASK_Blocking_Ticks_Count = Copy_NumberOfMs;
	OS_voidSvcSetService(SVC_TERMINATE);
}

OS_Error_t OS_voidCreateTask(TASK_Control_Block* Copy_pTcb)
{
	OS_Error_t OS_voidCreateTaskState = OS_NO_ERROR;
	/*check if exceed the stack size*/
	if((OS_Control_Block.OS_Locator - Copy_pTcb->TASK_Config.TASK_Stack_Size) < (u32)&_eheap)
	{
		OS_voidCreateTaskState = OS_TASK_EXCEEDED_STACK_SIZE;
	}
	else
	{
		/*
		 * 	_S_PSP_TASK
		 *-------------
		 *	TASK
		 *-------------
		 *	_E_PSP_TASK
		 *_-----------
		 *	_OS_Locator
		 */


		/*set start and end of psp */
		Copy_pTcb->_S_TASK_Psp = OS_Control_Block.OS_Locator;
		Copy_pTcb->_E_TASK_Psp = (Copy_pTcb->_S_TASK_Psp - Copy_pTcb->TASK_Config.TASK_Stack_Size);

		/*update the locator with the new position*/
		OS_Control_Block.OS_Locator = (Copy_pTcb->_E_TASK_Psp - 8);

		/*init psp of the task*/
		OS_voidCreateTaskStack(Copy_pTcb);
		Copy_pTcb->TASK_State = TASK_Suspended;

		/*add the task to scheduler table*/
		OS_Control_Block.OS_Tasks[OS_Control_Block.OS_NoOfTasks++] = Copy_pTcb;
	}
	return OS_voidCreateTaskState;
}


OS_Error_t OS_voidAcquireMutex(TASK_Control_Block *Copy_pTcb,MUTEX_Control_t *Copy_pMutex)
{
	OS_Error_t OS_voidAcquireMutexState = OS_NO_ERROR;
	if(Copy_pMutex->MUTEX_CurrentTask==NULL)
	{
		/*Not used*/
		Copy_pMutex->MUTEX_CurrentTask = Copy_pTcb;
	}
	else
	{
		if(Copy_pMutex->MUTEX_NextTask==NULL)
		{
			/*Task is using it but there's a place to wait on it*/
			Copy_pMutex->MUTEX_NextTask =  Copy_pTcb;
			Copy_pTcb->TASK_State=TASK_Suspended;
			OS_voidSvcSetService(SVC_TERMINATE);

		}
		else
		{
			OS_voidAcquireMutexState = OS_MUTEX_FULL;
		}
	}
	return  OS_voidAcquireMutexState;

}
OS_Error_t OS_voidReleaseMutex(TASK_Control_Block *Copy_pTcb,MUTEX_Control_t *Copy_pMutex)
{
	OS_Error_t OS_voidReleaseMutexState = OS_NO_ERROR;
	if(Copy_pMutex->MUTEX_CurrentTask==Copy_pTcb)
	{
		Copy_pMutex->MUTEX_CurrentTask = Copy_pMutex->MUTEX_NextTask;
		Copy_pMutex->MUTEX_NextTask = NULL;
		Copy_pMutex->MUTEX_CurrentTask->TASK_State = TASK_Waiting;
		OS_voidSvcSetService(SVC_ACTIVATE);
	}
	else
	{
		OS_voidReleaseMutexState = OS_MUTEX_OWNED_BY_OTHER_TASK;
	}
	return OS_voidReleaseMutexState;

}




void OS_voidStart(void)
{
	OS_Control_Block.OS_State=OS_Running;
	OS_Control_Block.OS_Current_Task=&OS_IdleTask;
	OS_voidActivateTask(&OS_IdleTask);

	/*after 1ms generate systick interrupt*/
	CORTEXMXPORTING_voidStartSystick(8000);
	OS_SET_PSP_ADDRESS(OS_Control_Block.OS_Current_Task->_C_TASK_Psp);
	OS_SP_SHADOWS_PSP;
	OS_SWITCH_UNPRIVILIGE_MODE;
	OS_Control_Block.OS_Current_Task->TASK_Config.TASK_Entry();
}



OS_Error_t OS_Init(void)
{
	OS_Error_t OS_Init_State = OS_NO_ERROR;

	/*init hw*/
	CORTEXMXPORTING_voidInit();

	/*init os mode with suspended*/
	OS_Control_Block.OS_State = OS_Suspended;

	/*init msp for os*/
	OS_voidCreateMainStack();

	/*Create Ready Queue*/
	if(FIFO_init(&OS_Ready_Queue,(void*)OS_Ready,100)!=FIFO_NO_ERROR)
	{
		OS_Init_State = OS_QUEUE_ERROR;
	}

	/*Config Idle Task*/
	strcpy((char*)OS_IdleTask.TASK_Config.TASK_Name,"IDLE_TASK");
	OS_IdleTask.TASK_Config.TASK_Priority=255;
	OS_IdleTask.TASK_Config.TASK_Stack_Size=300;
	OS_IdleTask.TASK_Config.TASK_Entry=OS_voidIdleTask;
	OS_Init_State+=OS_voidCreateTask(&OS_IdleTask);

	return OS_Init_State;
}




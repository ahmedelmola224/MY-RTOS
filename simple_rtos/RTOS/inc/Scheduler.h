

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

typedef enum
{
	OS_NO_ERROR,
	OS_QUEUE_ERROR,
	OS_TASK_EXCEEDED_STACK_SIZE,
	OS_TASK_ALREADY_ACTIVE,
	OS_TASK_ALREADY_TERMINATED,
	OS_MUTEX_FULL,
	OS_MUTEX_OWNED_BY_OTHER_TASK

}OS_Error_t;


typedef struct
{
	struct
	{
		u8 TASK_Name[30];
		void(*TASK_Entry)(void);
		u32 TASK_Stack_Size;
		u8 TASK_Priority;
		u8 TASK_AutoStart;
	}TASK_Config;

	u32 _S_TASK_Psp;//start
	u32 _E_TASK_Psp;//end
	u32* _C_TASK_Psp; //current
	enum
	{
		TASK_Suspended,
		TASK_Running,
		TASK_Waiting,
		TASK_Ready
	}TASK_State;
	struct
	{
		enum
		{
			TASK_BLOCKING_DIS,
			TASK_BLOCKING_EN
		}TASK_Blocking_State;
		u32 TASK_Blocking_Ticks_Count;
	}TASK_Waiting_State;
}TASK_Control_Block;

typedef struct
{
	struct
	{
		u8 *MUTEX_Ppayload;
		u8 MUTEX_Name[30];
		u32 MUTEX_Payloadsize;
	}MUTEX_Config;
	TASK_Control_Block *MUTEX_CurrentTask;
	TASK_Control_Block *MUTEX_NextTask;
}MUTEX_Control_t;


OS_Error_t OS_Init(void);
void OS_voidStart(void);
OS_Error_t OS_voidCreateTask(TASK_Control_Block* Copy_pTcb);
OS_Error_t OS_voidActivateTask(TASK_Control_Block* Copy_pTcb);
OS_Error_t OS_voidTerminateTask(TASK_Control_Block* Copy_pTcb);
void OS_voidWaitTask(TASK_Control_Block* Copy_pTcb,u32 Copy_NumberOfMs);
OS_Error_t OS_voidAcquireMutex(TASK_Control_Block *Copy_pTcb,MUTEX_Control_t *Copy_pMutex);
OS_Error_t OS_voidReleaseMutex(TASK_Control_Block *Copy_pTcb,MUTEX_Control_t *Copy_pMutex);

#endif

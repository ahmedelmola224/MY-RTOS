/*
 * Scheduler_prvate.h
 *
 *  Created on: Oct 15, 2023
 *      Author: ahmed
 */

#ifndef INC_SCHEDULER_PRIVATE_H_
#define INC_SCHEDULER_PRVIATE_H_



typedef enum
{
	SVC_ACTIVATE,
	SVC_TERMINATE,
	SVC_WAIT
}SVC_Status_t;

typedef struct
{
	TASK_Control_Block* OS_Tasks[100];
	u32 _S_OS_Msp;
	u32 _E_OS_Msp;
	u32  OS_Locator;
	u32 OS_NoOfTasks ;
	TASK_Control_Block* OS_Current_Task;
	TASK_Control_Block* OS_Next_Task;
	enum
	{
		OS_Suspended,
		OS_Running
	}OS_State;
}OS_Control_t;

void OS_voidSVC(u32 *stackframe);
static void OS_voidSvcSetService(u32 Copy_ServiceId);
static void OS_voidCreateMainStack(void);
static void OS_voidCreateTaskStack(TASK_Control_Block* Copy_pTcb);
static void OS_voidIdleTask(void);
__attribute((naked)) static void OS_voidPendSv();

static void OS_voidNextTask(void);
static void OS_voidUpdateSchedulerTable(void);
static void OS_voidBubblSort(void);
#endif /* INC_SCHEDULER_PRVATE_H_ */

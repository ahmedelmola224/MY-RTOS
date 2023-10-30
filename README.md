
# Round Robin-based Real Time Operating System 

In this project I used CMSIS_V5 from ARM for porting the required hardware







## Support

Feel Free to contact me, just message me at

linkedin: linkedin.com/in/ahmed-elmola.


## MY-RTOS Features

- Support The basic operation of any RTOS.
- Scheduler based on priority - Highest Priority Should Run First-
- Support Round-Robin Scheduling when two or more tasks have the same priority.
- The IDLE task enter sleep mode and waiting for an event.
- Support Mutex mechanism .


## User API Reference

#### OS_Init
- The first function needed to be run
- This function is used to initialize the os by porting the required hardware, creating the main stack for the os and creating the idle task with it is stack.

| Parameter | Type     | Description                |
| :-------- | :------- | :------------------------- |
| `Nothing` | `void` |  |

#### OS_voidStart
- The second function needed to be run
- This function is used to to update the os state, start the systick time, activate the idle task and make the sp to shadow to psp of idle task instead of msp.

| Parameter | Type     | Description                       |
| :-------- | :------- | :-------------------------------- |
| `Nothing`      | `void` | |

#### OS_voidCreateTask

- This function is used to to create the task by creating a TCB for the task ,add the TCB to OS table and reserve the required stack for the task.

| Parameter | Type     | Description                       |
| :-------- | :------- | :-------------------------------- |
| `Copy_pTcb`      | `TASK_Control_Block*` |The user need to conigure some properties of the task and pass the TASK_Control_Block as a pointer  |

#### OS_voidActivateTask
After creating the task you need to activate it ,this function update the task state ,and make a svc call then the svc update scheduling table based on priority and determine the next task to be run then the dispatcher make the appropriate task run using the pendsv interrupt and making all the required context switch. 

| Parameter | Type     | Description                       |
| :-------- | :------- | :-------------------------------- |
| `Copy_pTcb`      | `TASK_Control_Block*` |  |

#### OS_voidTerminateTask
This funcion is used to terminate task by updating the task state ,and make a svc call then the svc update scheduling table based on priority and the state then determine the next task to be run then the dispatcher make the appropriate task run using the pendsv interrupt and making all the required context switch.  

| Parameter | Type     | Description                       |
| :-------- | :------- | :-------------------------------- |
| `Copy_pTcb`      | `TASK_Control_Block*` |  |


#### OS_voidWaitTask
This funcion is used to make the task for a number of mile seconds by updating the task state ,enabling the blocking property ,set the number of ticks the task will wait then systick is responsible to enable the task again after the number of ticks determined.   

| Parameter | Type     | Description                       |
| :-------- | :------- | :-------------------------------- |
| `Copy_pTcb`      | `TASK_Control_Block*` |  |
| `Copy_NumberOfMs`      | `u32` | The number of mile-seconds the task need to wait |


#### OS_voidAcquireMutex
This funcion is used to give the mutex to the task  if it is available ,if there is a place to wait it updates the task state and update the mutex so that when the mutex is freed the task take it.  

| Parameter | Type     | Description                       |
| :-------- | :------- | :-------------------------------- |
| `Copy_pTcb`      | `TASK_Control_Block*` |  |
| `Copy_pMutex`      | `MUTEX_Control_t*` | The user need to configure things in the mutex and pass it  |

#### OS_voidReleaseMutex
This funcion is used to release the mutex ,if there is another task waiting for the mutex it gives the mutex to the next task.  

| Parameter | Type     | Description                       |
| :-------- | :------- | :-------------------------------- |
| `Copy_pTcb`      | `TASK_Control_Block*` |  |
| `Copy_pMutex`      | `MUTEX_Control_t*` |  |


## Task Priority Example
Task1 priority < Task2 priority < Task3 priority < Task4 priority

![Task Priority](https://github.com/ahmedelmola224/MY-RTOS/blob/main/Examples/priorities.png)

## Round Robin Example
Task1 priority == Task2 priority == Task3 priority == Task4 priority  

![Round Robin](https://github.com/ahmedelmola224/MY-RTOS/blob/main/Examples/roundrobin.png)

## Priority Inversion Example
Task4 is the highest piority it run for a while then waited until task1 freed the mutex which is the lowest task priority so there is unbounded Priority Inversion

![Priority Inversion ](https://github.com/ahmedelmola224/MY-RTOS/blob/main/Examples/priority_inversion.png)

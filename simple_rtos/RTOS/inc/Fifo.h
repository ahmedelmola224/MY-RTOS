

#ifndef INC_FIFO_H_
#define INC_FIFO_H_
#include "STD_TYPES.h"
#define element_t  void*

typedef struct
{
	unsigned int counter;
	element_t* head;
	element_t* tail;
	element_t* base;
	unsigned int  length;
}Fifo_Buffer_t;

typedef enum
{
	FIFO_NO_ERROR,
	FIFO_FULL,
	FIFO_EMPTY,
	FIFO_NULL,
}Fifo_Buffer_Status;


Fifo_Buffer_Status FIFO_init(Fifo_Buffer_t* fBuf,element_t* arr,u32 length);
Fifo_Buffer_Status FIFO_enqueue(Fifo_Buffer_t* fBuf,element_t item);
Fifo_Buffer_Status FIFO_dequeue(Fifo_Buffer_t* fBuf,element_t *item);
Fifo_Buffer_Status FIFO_isfull(Fifo_Buffer_t* fBuf);


#endif

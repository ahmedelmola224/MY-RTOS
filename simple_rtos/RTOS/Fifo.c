
#include "Fifo.h"

Fifo_Buffer_Status FIFO_init(Fifo_Buffer_t* fifo,element_t* buff,u32 length)
{
	if(!buff )
			return FIFO_NULL;

		fifo->base = buff ;
		fifo->head = fifo->base ;
		fifo->tail = fifo->base ;
		fifo->length = length;
		fifo->counter=0;

		return FIFO_NO_ERROR;
}

Fifo_Buffer_Status FIFO_enqueue(Fifo_Buffer_t* fifo,element_t item)
{
	/* fifo null*/

		if (!fifo->base || !fifo->length)
			return FIFO_NULL;
		/*fifo is full*/

		/* fifo full */
		if ((fifo->head == fifo->tail) && (fifo->counter == fifo->length))
			return FIFO_FULL;

		*(fifo->tail)=item;
		fifo->counter++;

		/*for circular fifo again */

		/* circular enqueue */
		if (fifo->tail == (((u32)fifo->base + (4*fifo->length )) - 4 ))
			fifo->tail = fifo->base;
		else
			fifo->tail++;

		return FIFO_NO_ERROR;

}

Fifo_Buffer_Status FIFO_dequeue(Fifo_Buffer_t* fifo,element_t *item)
{
	/* check fifo valid */
		if (!fifo->base || !fifo->length)
			return FIFO_NULL;

		/* fifo empty */
		if (fifo->head == fifo->tail)
			return FIFO_EMPTY;



		*item = *(fifo->head);
		fifo->counter--;

		/* circular dequeue */
		if (fifo->head == (((unsigned int)fifo->base + (4*fifo->length )) - 4 ))
			fifo->head = fifo->base;
		else
			fifo->head++;

		return FIFO_NO_ERROR;

}

Fifo_Buffer_Status FIFO_isfull(Fifo_Buffer_t* fifo)
{
	if(!fifo->head || !fifo->base || !fifo->tail)
			return FIFO_NULL;
		if(fifo->counter == fifo->length)
			return FIFO_FULL;

		return FIFO_NO_ERROR;
}



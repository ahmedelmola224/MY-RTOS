/********************************************************************/
/* Author       : Ahmed Essam                                       */
/* Date         :                                                   */
/* File Name	: BIT_MATH.h		                                */
/* Type	        : library	                                        */
/* Version      : V01                                               */
/********************************************************************/

#ifndef BIT_MATH_H
#define BITH_MATH_H

		/*Manipulate 1bit*/
#define SET_BIT(VAR,BIT)   VAR|= (1 << (BIT))
#define CLR_BIT(VAR,BIT)   VAR&= (~(1 << (BIT)))
#define TOG_BIT(VAR,BIT)   VAR^= (1 << (BIT))
#define GET_BIT(VAR,BIT)   ((VAR >> BIT) & 1)
#define WRITE_BIT(VAR,BIT,BIT_VALUE) VAR=(VAR&(~(1 << (BIT))))|((BIT_VALUE)<<(BIT))

		/*Manipulate 4bit*/
#define SET_NIBBLE(VAR,STARTING_BIT)   VAR|= (0b1111 << (STARTING_BIT))
#define CLR_NIBBLE(VAR,STARTING_BIT)   VAR&= (~(0b1111 << (STARTING_BIT)))
#define TOG_NIBBLE(VAR,STARTING_BIT)   VAR^= (0b1111 << (STARTING_BIT))
#define GET_NIBBLE(VAR,STARTING_BIT)   (VAR >> STARTING_BIT) & 0b1111
#define WRITE_NIBBLE(VAR,STARTING_BIT,NIBBLE_VALUE) VAR=(VAR&(~(0b1111 << (STARTING_BIT))))|(NIBBLE_VALUE<<STARTING_BIT)

		/*Manipulate 8bit*/
#define SET_BYTE(VAR,STARTING_BIT)   VAR|= (0b11111111 << (STARTING_BIT))
#define CLR_BYTE(VAR,STARTING_BIT)   VAR&= (~(0b11111111 << (STARTING_BIT)))
#define TOG_BYTE(VAR,STARTING_BIT)   VAR^= (0b11111111 << (STARTING_BIT))
#define GET_BYTE(VAR,STARTING_BIT)   (VAR >> STARTING_BIT) & 0b11111111
#define WRITE_BYTE(VAR,STARTING_BIT,BYTE_VALUE) VAR=(VAR&(~(0b11111111 << (STARTING_BIT))))|(BYTE_VALUE<<STARTING_BIT)

#endif

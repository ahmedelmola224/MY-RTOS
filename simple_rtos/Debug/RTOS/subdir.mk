################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RTOS/CortexMx_porting.c \
../RTOS/Fifo.c \
../RTOS/Scheduler.c 

OBJS += \
./RTOS/CortexMx_porting.o \
./RTOS/Fifo.o \
./RTOS/Scheduler.o 

C_DEPS += \
./RTOS/CortexMx_porting.d \
./RTOS/Fifo.d \
./RTOS/Scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
RTOS/CortexMx_porting.o: ../RTOS/CortexMx_porting.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos/RTOS/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos/CMSIS_V5" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos/RTOS" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"RTOS/CortexMx_porting.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
RTOS/Fifo.o: ../RTOS/Fifo.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos/RTOS/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos/CMSIS_V5" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos/RTOS" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"RTOS/Fifo.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
RTOS/Scheduler.o: ../RTOS/Scheduler.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos/RTOS/inc" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos/CMSIS_V5" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos/RTOS" -I"C:/Users/ahmed/STM32CubeIDE/workspace_1.4.0/simple_rtos" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"RTOS/Scheduler.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"


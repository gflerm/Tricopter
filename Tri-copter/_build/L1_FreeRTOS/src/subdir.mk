################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L1_FreeRTOS/src/croutine.c \
../L1_FreeRTOS/src/event_groups.c \
../L1_FreeRTOS/src/list.c \
../L1_FreeRTOS/src/queue.c \
../L1_FreeRTOS/src/tasks.c \
../L1_FreeRTOS/src/timers.c 

OBJS += \
./L1_FreeRTOS/src/croutine.o \
./L1_FreeRTOS/src/event_groups.o \
./L1_FreeRTOS/src/list.o \
./L1_FreeRTOS/src/queue.o \
./L1_FreeRTOS/src/tasks.o \
./L1_FreeRTOS/src/timers.o 

C_DEPS += \
./L1_FreeRTOS/src/croutine.d \
./L1_FreeRTOS/src/event_groups.d \
./L1_FreeRTOS/src/list.d \
./L1_FreeRTOS/src/queue.d \
./L1_FreeRTOS/src/tasks.d \
./L1_FreeRTOS/src/timers.d 


# Each subdirectory must supply rules for building sources it contributes
L1_FreeRTOS/src/%.o: ../L1_FreeRTOS/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -I"D:\projects\Tri-copter" -I"D:\projects\Tri-copter\newlib" -I"D:\projects\Tri-copter\L0_LowLevel" -I"D:\projects\Tri-copter\L1_FreeRTOS" -I"D:\projects\Tri-copter\L1_FreeRTOS\include" -I"D:\projects\Tri-copter\L1_FreeRTOS\portable" -I"D:\projects\Tri-copter\L2_Drivers" -I"D:\projects\Tri-copter\L2_Drivers\base" -I"D:\projects\Tri-copter\L3_Utils" -I"D:\projects\Tri-copter\L3_Utils\tlm" -I"D:\projects\Tri-copter\L4_IO" -I"D:\projects\Tri-copter\L4_IO\fat" -I"D:\projects\Tri-copter\L4_IO\wireless" -I"D:\projects\Tri-copter\L5_Application" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



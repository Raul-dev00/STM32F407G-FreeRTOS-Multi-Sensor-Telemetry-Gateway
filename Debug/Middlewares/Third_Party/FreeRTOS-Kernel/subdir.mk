################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS-Kernel/croutine.c \
../Middlewares/Third_Party/FreeRTOS-Kernel/event_groups.c \
../Middlewares/Third_Party/FreeRTOS-Kernel/list.c \
../Middlewares/Third_Party/FreeRTOS-Kernel/queue.c \
../Middlewares/Third_Party/FreeRTOS-Kernel/stream_buffer.c \
../Middlewares/Third_Party/FreeRTOS-Kernel/tasks.c \
../Middlewares/Third_Party/FreeRTOS-Kernel/timers.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS-Kernel/croutine.o \
./Middlewares/Third_Party/FreeRTOS-Kernel/event_groups.o \
./Middlewares/Third_Party/FreeRTOS-Kernel/list.o \
./Middlewares/Third_Party/FreeRTOS-Kernel/queue.o \
./Middlewares/Third_Party/FreeRTOS-Kernel/stream_buffer.o \
./Middlewares/Third_Party/FreeRTOS-Kernel/tasks.o \
./Middlewares/Third_Party/FreeRTOS-Kernel/timers.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS-Kernel/croutine.d \
./Middlewares/Third_Party/FreeRTOS-Kernel/event_groups.d \
./Middlewares/Third_Party/FreeRTOS-Kernel/list.d \
./Middlewares/Third_Party/FreeRTOS-Kernel/queue.d \
./Middlewares/Third_Party/FreeRTOS-Kernel/stream_buffer.d \
./Middlewares/Third_Party/FreeRTOS-Kernel/tasks.d \
./Middlewares/Third_Party/FreeRTOS-Kernel/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS-Kernel/%.o Middlewares/Third_Party/FreeRTOS-Kernel/%.su Middlewares/Third_Party/FreeRTOS-Kernel/%.cyclo: ../Middlewares/Third_Party/FreeRTOS-Kernel/%.c Middlewares/Third_Party/FreeRTOS-Kernel/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"/home/raul/freertos_telemetry_gateway/Middlewares/Third_Party/FreeRTOS-Kernel/include" -I"/home/raul/freertos_telemetry_gateway/Middlewares/Third_Party/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2d-Kernel

clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2d-Kernel:
	-$(RM) ./Middlewares/Third_Party/FreeRTOS-Kernel/croutine.cyclo ./Middlewares/Third_Party/FreeRTOS-Kernel/croutine.d ./Middlewares/Third_Party/FreeRTOS-Kernel/croutine.o ./Middlewares/Third_Party/FreeRTOS-Kernel/croutine.su ./Middlewares/Third_Party/FreeRTOS-Kernel/event_groups.cyclo ./Middlewares/Third_Party/FreeRTOS-Kernel/event_groups.d ./Middlewares/Third_Party/FreeRTOS-Kernel/event_groups.o ./Middlewares/Third_Party/FreeRTOS-Kernel/event_groups.su ./Middlewares/Third_Party/FreeRTOS-Kernel/list.cyclo ./Middlewares/Third_Party/FreeRTOS-Kernel/list.d ./Middlewares/Third_Party/FreeRTOS-Kernel/list.o ./Middlewares/Third_Party/FreeRTOS-Kernel/list.su ./Middlewares/Third_Party/FreeRTOS-Kernel/queue.cyclo ./Middlewares/Third_Party/FreeRTOS-Kernel/queue.d ./Middlewares/Third_Party/FreeRTOS-Kernel/queue.o ./Middlewares/Third_Party/FreeRTOS-Kernel/queue.su ./Middlewares/Third_Party/FreeRTOS-Kernel/stream_buffer.cyclo ./Middlewares/Third_Party/FreeRTOS-Kernel/stream_buffer.d ./Middlewares/Third_Party/FreeRTOS-Kernel/stream_buffer.o ./Middlewares/Third_Party/FreeRTOS-Kernel/stream_buffer.su ./Middlewares/Third_Party/FreeRTOS-Kernel/tasks.cyclo ./Middlewares/Third_Party/FreeRTOS-Kernel/tasks.d ./Middlewares/Third_Party/FreeRTOS-Kernel/tasks.o ./Middlewares/Third_Party/FreeRTOS-Kernel/tasks.su ./Middlewares/Third_Party/FreeRTOS-Kernel/timers.cyclo ./Middlewares/Third_Party/FreeRTOS-Kernel/timers.d ./Middlewares/Third_Party/FreeRTOS-Kernel/timers.o ./Middlewares/Third_Party/FreeRTOS-Kernel/timers.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2d-Kernel


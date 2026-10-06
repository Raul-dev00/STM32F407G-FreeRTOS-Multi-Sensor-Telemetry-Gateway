################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/main.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/main.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/main.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/%.o Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/%.su Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/%.cyclo: ../Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/%.c Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"/home/raul/freertos_telemetry_gateway/Middlewares/Third_Party/FreeRTOS-Kernel/include" -I"/home/raul/freertos_telemetry_gateway/Middlewares/Third_Party/FreeRTOS-Kernel/portable/GCC/ARM_CM4F" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2d-Kernel-2f-examples-2f-cmake_example

clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2d-Kernel-2f-examples-2f-cmake_example:
	-$(RM) ./Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/main.cyclo ./Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/main.d ./Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/main.o ./Middlewares/Third_Party/FreeRTOS-Kernel/examples/cmake_example/main.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2d-Kernel-2f-examples-2f-cmake_example


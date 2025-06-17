################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/microsoft/STM32Cube/Repository/STM32Cube_FW_H7_V1.12.1/Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.c 

OBJS += \
./Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.o 

C_DEPS += \
./Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.o: C:/Users/microsoft/STM32Cube/Repository/STM32Cube_FW_H7_V1.12.1/Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.c Drivers/BSP/STM32H7xx_Nucleo/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -DUSE_NUCLEO_64 -c -I../Core/Inc -IC:/Users/microsoft/STM32Cube/Repository/STM32Cube_FW_H7_V1.12.1/Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/microsoft/STM32Cube/Repository/STM32Cube_FW_H7_V1.12.1/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -IC:/Users/microsoft/STM32Cube/Repository/STM32Cube_FW_H7_V1.12.1/Drivers/CMSIS/Device/ST/STM32H7xx/Include -IC:/Users/microsoft/STM32Cube/Repository/STM32Cube_FW_H7_V1.12.1/Drivers/CMSIS/Include -IC:/Users/microsoft/STM32Cube/Repository/STM32Cube_FW_H7_V1.12.1/Drivers/BSP/STM32H7xx_Nucleo -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32H7xx_Nucleo

clean-Drivers-2f-BSP-2f-STM32H7xx_Nucleo:
	-$(RM) ./Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.cyclo ./Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.d ./Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.o ./Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32H7xx_Nucleo


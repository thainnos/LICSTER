################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"


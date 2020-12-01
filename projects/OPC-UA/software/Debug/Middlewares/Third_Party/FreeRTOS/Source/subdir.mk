################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
../Middlewares/Third_Party/FreeRTOS/Source/list.c \
../Middlewares/Third_Party/FreeRTOS/Source/queue.c \
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
../Middlewares/Third_Party/FreeRTOS/Source/timers.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/croutine.o \
./Middlewares/Third_Party/FreeRTOS/Source/event_groups.o \
./Middlewares/Third_Party/FreeRTOS/Source/list.o \
./Middlewares/Third_Party/FreeRTOS/Source/queue.o \
./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.o \
./Middlewares/Third_Party/FreeRTOS/Source/tasks.o \
./Middlewares/Third_Party/FreeRTOS/Source/timers.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/croutine.d \
./Middlewares/Third_Party/FreeRTOS/Source/event_groups.d \
./Middlewares/Third_Party/FreeRTOS/Source/list.d \
./Middlewares/Third_Party/FreeRTOS/Source/queue.d \
./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.d \
./Middlewares/Third_Party/FreeRTOS/Source/tasks.d \
./Middlewares/Third_Party/FreeRTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/croutine.o: ../Middlewares/Third_Party/FreeRTOS/Source/croutine.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FreeRTOS/Source/croutine.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/FreeRTOS/Source/event_groups.o: ../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FreeRTOS/Source/event_groups.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/FreeRTOS/Source/list.o: ../Middlewares/Third_Party/FreeRTOS/Source/list.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FreeRTOS/Source/list.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/FreeRTOS/Source/queue.o: ../Middlewares/Third_Party/FreeRTOS/Source/queue.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FreeRTOS/Source/queue.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.o: ../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/FreeRTOS/Source/tasks.o: ../Middlewares/Third_Party/FreeRTOS/Source/tasks.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FreeRTOS/Source/tasks.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/FreeRTOS/Source/timers.o: ../Middlewares/Third_Party/FreeRTOS/Source/timers.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FreeRTOS/Source/timers.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"


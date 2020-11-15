################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/src/api/api_lib.c \
../Middlewares/Third_Party/LwIP/src/api/api_msg.c \
../Middlewares/Third_Party/LwIP/src/api/err.c \
../Middlewares/Third_Party/LwIP/src/api/if_api.c \
../Middlewares/Third_Party/LwIP/src/api/netbuf.c \
../Middlewares/Third_Party/LwIP/src/api/netdb.c \
../Middlewares/Third_Party/LwIP/src/api/netifapi.c \
../Middlewares/Third_Party/LwIP/src/api/sockets.c \
../Middlewares/Third_Party/LwIP/src/api/tcpip.c 

OBJS += \
./Middlewares/Third_Party/LwIP/src/api/api_lib.o \
./Middlewares/Third_Party/LwIP/src/api/api_msg.o \
./Middlewares/Third_Party/LwIP/src/api/err.o \
./Middlewares/Third_Party/LwIP/src/api/if_api.o \
./Middlewares/Third_Party/LwIP/src/api/netbuf.o \
./Middlewares/Third_Party/LwIP/src/api/netdb.o \
./Middlewares/Third_Party/LwIP/src/api/netifapi.o \
./Middlewares/Third_Party/LwIP/src/api/sockets.o \
./Middlewares/Third_Party/LwIP/src/api/tcpip.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/src/api/api_lib.d \
./Middlewares/Third_Party/LwIP/src/api/api_msg.d \
./Middlewares/Third_Party/LwIP/src/api/err.d \
./Middlewares/Third_Party/LwIP/src/api/if_api.d \
./Middlewares/Third_Party/LwIP/src/api/netbuf.d \
./Middlewares/Third_Party/LwIP/src/api/netdb.d \
./Middlewares/Third_Party/LwIP/src/api/netifapi.d \
./Middlewares/Third_Party/LwIP/src/api/sockets.d \
./Middlewares/Third_Party/LwIP/src/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/src/api/api_lib.o: ../Middlewares/Third_Party/LwIP/src/api/api_lib.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/api/api_lib.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/api/api_msg.o: ../Middlewares/Third_Party/LwIP/src/api/api_msg.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/api/api_msg.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/api/err.o: ../Middlewares/Third_Party/LwIP/src/api/err.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/api/err.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/api/if_api.o: ../Middlewares/Third_Party/LwIP/src/api/if_api.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/api/if_api.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/api/netbuf.o: ../Middlewares/Third_Party/LwIP/src/api/netbuf.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/api/netbuf.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/api/netdb.o: ../Middlewares/Third_Party/LwIP/src/api/netdb.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/api/netdb.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/api/netifapi.o: ../Middlewares/Third_Party/LwIP/src/api/netifapi.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/api/netifapi.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/api/sockets.o: ../Middlewares/Third_Party/LwIP/src/api/sockets.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/api/sockets.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/api/tcpip.o: ../Middlewares/Third_Party/LwIP/src/api/tcpip.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/api/tcpip.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"


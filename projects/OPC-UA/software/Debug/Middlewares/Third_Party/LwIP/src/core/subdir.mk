################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/src/core/altcp.c \
../Middlewares/Third_Party/LwIP/src/core/altcp_alloc.c \
../Middlewares/Third_Party/LwIP/src/core/altcp_tcp.c \
../Middlewares/Third_Party/LwIP/src/core/def.c \
../Middlewares/Third_Party/LwIP/src/core/dns.c \
../Middlewares/Third_Party/LwIP/src/core/inet_chksum.c \
../Middlewares/Third_Party/LwIP/src/core/init.c \
../Middlewares/Third_Party/LwIP/src/core/ip.c \
../Middlewares/Third_Party/LwIP/src/core/mem.c \
../Middlewares/Third_Party/LwIP/src/core/memp.c \
../Middlewares/Third_Party/LwIP/src/core/netif.c \
../Middlewares/Third_Party/LwIP/src/core/pbuf.c \
../Middlewares/Third_Party/LwIP/src/core/raw.c \
../Middlewares/Third_Party/LwIP/src/core/stats.c \
../Middlewares/Third_Party/LwIP/src/core/sys.c \
../Middlewares/Third_Party/LwIP/src/core/tcp.c \
../Middlewares/Third_Party/LwIP/src/core/tcp_in.c \
../Middlewares/Third_Party/LwIP/src/core/tcp_out.c \
../Middlewares/Third_Party/LwIP/src/core/timeouts.c \
../Middlewares/Third_Party/LwIP/src/core/udp.c 

OBJS += \
./Middlewares/Third_Party/LwIP/src/core/altcp.o \
./Middlewares/Third_Party/LwIP/src/core/altcp_alloc.o \
./Middlewares/Third_Party/LwIP/src/core/altcp_tcp.o \
./Middlewares/Third_Party/LwIP/src/core/def.o \
./Middlewares/Third_Party/LwIP/src/core/dns.o \
./Middlewares/Third_Party/LwIP/src/core/inet_chksum.o \
./Middlewares/Third_Party/LwIP/src/core/init.o \
./Middlewares/Third_Party/LwIP/src/core/ip.o \
./Middlewares/Third_Party/LwIP/src/core/mem.o \
./Middlewares/Third_Party/LwIP/src/core/memp.o \
./Middlewares/Third_Party/LwIP/src/core/netif.o \
./Middlewares/Third_Party/LwIP/src/core/pbuf.o \
./Middlewares/Third_Party/LwIP/src/core/raw.o \
./Middlewares/Third_Party/LwIP/src/core/stats.o \
./Middlewares/Third_Party/LwIP/src/core/sys.o \
./Middlewares/Third_Party/LwIP/src/core/tcp.o \
./Middlewares/Third_Party/LwIP/src/core/tcp_in.o \
./Middlewares/Third_Party/LwIP/src/core/tcp_out.o \
./Middlewares/Third_Party/LwIP/src/core/timeouts.o \
./Middlewares/Third_Party/LwIP/src/core/udp.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/src/core/altcp.d \
./Middlewares/Third_Party/LwIP/src/core/altcp_alloc.d \
./Middlewares/Third_Party/LwIP/src/core/altcp_tcp.d \
./Middlewares/Third_Party/LwIP/src/core/def.d \
./Middlewares/Third_Party/LwIP/src/core/dns.d \
./Middlewares/Third_Party/LwIP/src/core/inet_chksum.d \
./Middlewares/Third_Party/LwIP/src/core/init.d \
./Middlewares/Third_Party/LwIP/src/core/ip.d \
./Middlewares/Third_Party/LwIP/src/core/mem.d \
./Middlewares/Third_Party/LwIP/src/core/memp.d \
./Middlewares/Third_Party/LwIP/src/core/netif.d \
./Middlewares/Third_Party/LwIP/src/core/pbuf.d \
./Middlewares/Third_Party/LwIP/src/core/raw.d \
./Middlewares/Third_Party/LwIP/src/core/stats.d \
./Middlewares/Third_Party/LwIP/src/core/sys.d \
./Middlewares/Third_Party/LwIP/src/core/tcp.d \
./Middlewares/Third_Party/LwIP/src/core/tcp_in.d \
./Middlewares/Third_Party/LwIP/src/core/tcp_out.d \
./Middlewares/Third_Party/LwIP/src/core/timeouts.d \
./Middlewares/Third_Party/LwIP/src/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/src/core/altcp.o: ../Middlewares/Third_Party/LwIP/src/core/altcp.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/altcp.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/altcp_alloc.o: ../Middlewares/Third_Party/LwIP/src/core/altcp_alloc.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/altcp_alloc.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/altcp_tcp.o: ../Middlewares/Third_Party/LwIP/src/core/altcp_tcp.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/altcp_tcp.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/def.o: ../Middlewares/Third_Party/LwIP/src/core/def.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/def.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/dns.o: ../Middlewares/Third_Party/LwIP/src/core/dns.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/dns.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/inet_chksum.o: ../Middlewares/Third_Party/LwIP/src/core/inet_chksum.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/inet_chksum.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/init.o: ../Middlewares/Third_Party/LwIP/src/core/init.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/init.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/ip.o: ../Middlewares/Third_Party/LwIP/src/core/ip.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/ip.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/mem.o: ../Middlewares/Third_Party/LwIP/src/core/mem.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/mem.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/memp.o: ../Middlewares/Third_Party/LwIP/src/core/memp.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/memp.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/netif.o: ../Middlewares/Third_Party/LwIP/src/core/netif.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/netif.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/pbuf.o: ../Middlewares/Third_Party/LwIP/src/core/pbuf.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/pbuf.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/raw.o: ../Middlewares/Third_Party/LwIP/src/core/raw.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/raw.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/stats.o: ../Middlewares/Third_Party/LwIP/src/core/stats.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/stats.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/sys.o: ../Middlewares/Third_Party/LwIP/src/core/sys.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/sys.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/tcp.o: ../Middlewares/Third_Party/LwIP/src/core/tcp.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/tcp.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/tcp_in.o: ../Middlewares/Third_Party/LwIP/src/core/tcp_in.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/tcp_in.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/tcp_out.o: ../Middlewares/Third_Party/LwIP/src/core/tcp_out.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/tcp_out.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/timeouts.o: ../Middlewares/Third_Party/LwIP/src/core/timeouts.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/timeouts.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Middlewares/Third_Party/LwIP/src/core/udp.o: ../Middlewares/Third_Party/LwIP/src/core/udp.c
	arm-none-eabi-gcc "$<" -std=gnu11 -g3 -DUA_ENABLE_AMALGAMATION -DUA_ARCHITECTURE_FREERTOSLWIP -DUA_ARCHITECTURE_FREERTOSLWIP_POSIX_CLOCK -c -IMiddlewares/Third_Party/open62541 -I"/home/matt/Downloads/software/Middlewares/Third_Party/LwIP/src/include/lwip" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/LwIP/src/core/udp.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"


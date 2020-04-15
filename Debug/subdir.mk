################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DeviceGVCP.cpp \
../DeviceGVSP.cpp \
../Socket.cpp \
../StreamConverter.cpp \
../VirtualDevice.cpp \
../VirtualGEVCam.cpp \
../ini.cpp 

OBJS += \
./DeviceGVCP.o \
./DeviceGVSP.o \
./Socket.o \
./StreamConverter.o \
./VirtualDevice.o \
./VirtualGEVCam.o \
./ini.o 

CPP_DEPS += \
./DeviceGVCP.d \
./DeviceGVSP.d \
./Socket.d \
./StreamConverter.d \
./VirtualDevice.d \
./VirtualGEVCam.d \
./ini.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/local/include/opencv2 -I/usr/local/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" -lpthread
	@echo 'Finished building: $<'
	@echo ' '



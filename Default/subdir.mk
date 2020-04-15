################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DeviceGVCP.cpp \
../DeviceGVSP.cpp \
../Socket.cpp \
../VirtualDevice.cpp \
../VirtualGEVCam.cpp \
../ini.cpp \
../yuvit.cpp 

OBJS += \
./DeviceGVCP.o \
./DeviceGVSP.o \
./Socket.o \
./VirtualDevice.o \
./VirtualGEVCam.o \
./ini.o \
./yuvit.o 

CPP_DEPS += \
./DeviceGVCP.d \
./DeviceGVSP.d \
./Socket.d \
./VirtualDevice.d \
./VirtualGEVCam.d \
./ini.d \
./yuvit.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/thread/BoostThread.cpp \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/thread/Mutex.cpp \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/thread/MutexLock.cpp \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/thread/Thread.cpp \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/thread/ThreadId.cpp 

OBJS += \
./jointcoding-sources/core/thread/BoostThread.o \
./jointcoding-sources/core/thread/Mutex.o \
./jointcoding-sources/core/thread/MutexLock.o \
./jointcoding-sources/core/thread/Thread.o \
./jointcoding-sources/core/thread/ThreadId.o 

CPP_DEPS += \
./jointcoding-sources/core/thread/BoostThread.d \
./jointcoding-sources/core/thread/Mutex.d \
./jointcoding-sources/core/thread/MutexLock.d \
./jointcoding-sources/core/thread/Thread.d \
./jointcoding-sources/core/thread/ThreadId.d 


# Each subdirectory must supply rules for building sources it contributes
jointcoding-sources/core/thread/BoostThread.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/thread/BoostThread.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

jointcoding-sources/core/thread/Mutex.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/thread/Mutex.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

jointcoding-sources/core/thread/MutexLock.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/thread/MutexLock.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

jointcoding-sources/core/thread/Thread.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/thread/Thread.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

jointcoding-sources/core/thread/ThreadId.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/sources/core/thread/ThreadId.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



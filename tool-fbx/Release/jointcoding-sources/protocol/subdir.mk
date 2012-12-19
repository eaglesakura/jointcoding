################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen/protocol/jcTouchEventProtocol.cpp \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen/protocol/jcUriProtocol.cpp \
/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen/protocol/ndkPixelFormatProtocol.cpp 

OBJS += \
./jointcoding-sources/protocol/jcTouchEventProtocol.o \
./jointcoding-sources/protocol/jcUriProtocol.o \
./jointcoding-sources/protocol/ndkPixelFormatProtocol.o 

CPP_DEPS += \
./jointcoding-sources/protocol/jcTouchEventProtocol.d \
./jointcoding-sources/protocol/jcUriProtocol.d \
./jointcoding-sources/protocol/ndkPixelFormatProtocol.d 


# Each subdirectory must supply rules for building sources it contributes
jointcoding-sources/protocol/jcTouchEventProtocol.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen/protocol/jcTouchEventProtocol.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

jointcoding-sources/protocol/jcUriProtocol.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen/protocol/jcUriProtocol.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

jointcoding-sources/protocol/ndkPixelFormatProtocol.o: /Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen/protocol/ndkPixelFormatProtocol.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBUILD_MaxOSX=1 -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/include" -I"/Applications/Autodesk/FBXSDK/2013.3//include" -I"/Users/yamashitatakeshi/workspaces/eclipse/github-public/jointcoding/sources/gen" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include jointcoding-sources/protocol/subdir.mk
-include jointcoding-sources/core/util/subdir.mk
-include jointcoding-sources/core/ui/subdir.mk
-include jointcoding-sources/core/thread/subdir.mk
-include jointcoding-sources/core/system/subdir.mk
-include jointcoding-sources/core/mem/subdir.mk
-include jointcoding-sources/core/libjpeg/subdir.mk
-include jointcoding-sources/core/io/subdir.mk
-include jointcoding-sources/core/graphics/jpeg/subdir.mk
-include jointcoding-sources/core/graphics/subdir.mk
-include jointcoding-sources/core/boost/thread/pthread/subdir.mk
-include jointcoding-sources/core/boost/system/subdir.mk
-include jointcoding-sources/core-fbx/node/subdir.mk
-include jointcoding-sources/core-fbx/convert/subdir.mk
-include jointcoding-sources/core-fbx/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(MM_DEPS)),)
-include $(MM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: fbx-converter

# Tool invocations
fbx-converter: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MacOS X C++ Linker'
	g++ -L"/Applications/Autodesk/FBXSDK/2013.3//lib/gcc4/ub/" -o "fbx-converter" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(MM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) fbx-converter
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
attiny13_electronic_load.lss \

FLASH_IMAGE += \
attiny13_electronic_load.hex \

EEPROM_IMAGE += \
attiny13_electronic_load.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: attiny13_electronic_load.elf secondary-outputs

# Tool invocations
attiny13_electronic_load.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,attiny13_electronic_load.map -mmcu=attiny13 -o "attiny13_electronic_load.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

attiny13_electronic_load.lss: attiny13_electronic_load.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S attiny13_electronic_load.elf  >"attiny13_electronic_load.lss"
	@echo 'Finished building: $@'
	@echo ' '

attiny13_electronic_load.hex: attiny13_electronic_load.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex attiny13_electronic_load.elf  "attiny13_electronic_load.hex"
	@echo 'Finished building: $@'
	@echo ' '

attiny13_electronic_load.eep: attiny13_electronic_load.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex attiny13_electronic_load.elf  "attiny13_electronic_load.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: attiny13_electronic_load.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=attiny13 attiny13_electronic_load.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) attiny13_electronic_load.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets

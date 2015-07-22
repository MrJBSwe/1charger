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
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables
LSS += \
1CHG.lss \

FLASH_IMAGE += \
1CHG.hex \

EEPROM_IMAGE += \
1CHG.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 1CHG.elf secondary-outputs

# Tool invocations
1CHG.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,1CHG.map -mmcu=attiny861 -o"1CHG.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '

1CHG.lss: 1CHG.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 1CHG.elf  >"1CHG.lss"
	@echo 'Finished building: $@'
	@echo ' '

1CHG.hex: 1CHG.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex 1CHG.elf  "1CHG.hex"
	@echo 'Finished building: $@'
	@echo ' '

1CHG.eep: 1CHG.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 1CHG.elf  "1CHG.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 1CHG.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=attiny861 1CHG.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_DEPS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) 1CHG.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

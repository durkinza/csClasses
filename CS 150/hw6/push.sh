avr-as -mmcu=atmega328p hw6.asm
avr-ld -m avr5 -o hw6.elf a.out
avr-objcopy -O ihex -R .eeprom hw6.elf hw6.hex
./ldino-lin -P hw6.hex

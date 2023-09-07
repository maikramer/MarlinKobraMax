# Kobra Max Updated Firmware
I've been working to bring the latest Marlin to the Kobra Max, the project is in active development and I didn't test everything yet.

# Please note
### What is Working:
- Automatic Bed Leveling
- LCD Screen
- Printing Fine
- Settings Working
- Can be built with Platformio and Vs Code

### What is not Working
- Resume from pause not working
- Filament Sensor (Sensor is Working, but it's not resuming, probably related to prior problem)
- LCD Keypad not working
- Some LCD Bugs
- You Tell me

### What was removed (For Simplicity)(Will not be in Release)
- Power Loss Recovery (Removed Code, won't work if enabled in Configuration)

# Warning
Unfortunately I cannot give any promises or guarantees, I just have my one printer to try things on, so I cannot test for any other circumstances or variables. Which means that you are fully responsible for what happens when using this firmware.

# Credits
The following sources have been invaluable
- https://github.com/jojos38/anycubic-kobra-improved-firmware
- https://github.com/shadow578/Marlin-H32
- https://github.com/wabbitguy/Kobra_Max
- https://github.com/malebuffy/Kobra-Max-Cobra-Kai-UI-and-Firmware
- https://github.com/JoyceKimberly/Kobra-updated
- https://github.com/shadow578/platform-hc32f46x

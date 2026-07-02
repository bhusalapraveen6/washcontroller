# WashController

A PIC16F877A-based washing machine controller simulation built for PicsimLab. This embedded C project implements a complete state-machine-driven washing machine interface with 12 wash programs, 5 water levels, real-time LCD display, door safety interlock with buzzer, countdown timer, and program execution logic.

---

## Features

- **12 Wash Programs**: Daily, Heavy, Delicates, Whites, Stain Wash, Ecocottons, Woolens, Bed Sheets, Rinse+Dry, Dry Only, Wash Only, Aqua Store
- **5 Water Levels**: Auto, Low, Medium, High, Max
- **Real-time LCD Display**: 4-line CLCD showing program name, water level, timer countdown, and current function (Wash / Rinse / Spin)
- **Door Safety Interlock**: RB0 door sensor — machine pauses and alarms if door opens during operation
- **Buzzer Alerts**: Beeps on program completion and door-open events
- **Timer-based Operation**: Countdown timer driven by Timer2 interrupt; total time auto-calculated per program and water level
- **State Machine Architecture**: Clean separation of screens — Power On, Program Selection, Water Level Selection, Start/Stop, and Run
- **Pause / Resume**: SW5 to start/resume, SW6 to pause
- **MPLAB X IDE Project**: Ready to build with XC8 compiler

---

## Hardware

| Component | Details |
|-----------|---------|
| Microcontroller | PIC16F877A |
| LCD | 20×4 Character LCD (HD44780-compatible) |
| Keypad | Digital keypad (switches SW4–SW6) |
| Door Sensor | RB0 input (active-low) |
| Buzzer | RC1 output |
| Motor/Fan | RC2 output |
| Timer | Timer2 (internal) |
| Simulation | PicsimLab (PICGenios board) |

### Pin Mapping

| Pin | Function |
|-----|----------|
| RB0 | Door status input (0 = open) |
| RC1 | Buzzer output |
| RC2 | Motor / Fan output |

---

## Project Structure

```
washcontroller/
├── main.c                          # Main loop & state machine
├── main.h                          # Configuration, pin defs, mode constants
├── newmain.c                       # Alternate / legacy main entry
├── washing_machine_functions_def.c # Core logic: screens, timer setup, run program, door check
├── washing_machine_header.h        # Function prototypes
├── clcd.c / clcd.h                 # Character LCD driver
├── digital_keypad.c / digital_keypad.h  # Keypad scan & debounce
├── timers.c / timers.h             # Timer2 initialization
├── isr.c                           # Interrupt service routine (Timer2)
├── Makefile                        # Build configuration
├── build/                          # Build artifacts
├── dist/                           # Distribution / HEX output
└── nbproject/                      # MPLAB X project metadata
```

---

## Wash Programs

| # | Program | Description |
|---|---------|-------------|
| 0 | Daily | Regular daily wash |
| 1 | Heavy | Heavy-duty cleaning |
| 2 | Delicates | Gentle cycle for delicate fabrics |
| 3 | Whites | Optimized for white garments |
| 4 | Stain Wash | Intensive stain removal |
| 5 | Ecocottons | Eco-friendly cotton cycle |
| 6 | Woolens | Specialized wool care |
| 7 | Bed Sheets | Large item cycle |
| 8 | Rinse+Dry | Rinse and spin dry only |
| 9 | Dry Only | Spin drying only |
| 10 | Wash Only | Wash without rinse/spin |
| 11 | Aqua Store | Water storage mode |

---

## State Machine

```
POWER ON
    │
    ▼
WASHING_PROGRAM_SCREEN  ──SW4──►  WATER_LEVEL_SCREEN
    │                                    │
    │                                    ▼
    │                           START_STOP_SCREEN
    │                            (SW5=START / SW6=STOP)
    │                                    │
    └────────────────────────────────────┘
                                         │ SW5
                                         ▼
                                    START_SCREEN
                                    (Run Program)
                                         │
                                    SW6 ─┘
                                         ▼
                                       PAUSE
```

---

## How It Works

1. **Power On**: Press **SW5** to boot the controller. A loading animation plays on the LCD.
2. **Select Program**: Use **SW4** to scroll through 12 programs. Press and hold **SW4** (LPSW4) to confirm and proceed.
3. **Select Water Level**: Use **SW4** to choose among Auto / Low / Medium / High / Max. Press and hold **SW4** to confirm.
4. **Start / Stop Screen**: Displays total time. Press **SW5** to start or **SW6** to go back.
5. **Running**: The LCD shows the current function (Wash → Rinse → Spin) and a live countdown timer.
   - **SW5**: Resume (from pause)
   - **SW6**: Pause
6. **Door Safety**: If the door opens (RB0 = 0), the motor stops, buzzer sounds, and display shows "Door is open". Operation resumes automatically when the door closes.
7. **Completion**: When timer reaches 00:00, the buzzer beeps, "Program complete" is shown, and the machine returns to program selection.

---

## Time Allocation per Cycle

For programs 0–7 (full wash cycles):
- **Wash**: ~46% of total time
- **Rinse**: ~12% of total time
- **Spin**: ~42% of total time

Special programs:
- **Rinse+Dry** (8): 40% rinse, 60% spin
- **Dry Only** (9): 100% spin
- **Wash Only** (10): 100% wash
- **Aqua Store** (11): 100% wash

Total time is determined by the selected program and water level (ranging from ~6 seconds to ~1 minute 36 seconds in simulation scale).

---

## Build & Run

### Requirements
- [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
- [XC8 Compiler](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers)
- [PicsimLab](https://github.com/lcgamboa/picsimlab) (simulator)

### Build
1. Open **MPLAB X IDE**.
2. Select **File → Open Project** and choose the `washcontroller` folder.
3. Right-click the project → **Build** (or press `F11`).
4. Locate the generated `.hex` file in `dist/default/production/`.

### Simulate in PicsimLab
1. Open **PicsimLab**.
2. Select **Board → PICGenios** and **Microcontroller → PIC16F877A**.
3. Load the HEX file: **File → Load HEX** → select the generated `.hex`.
4. Run the simulation and interact with the on-board switches.

---

## Key Controls

| Switch | Action |
|--------|--------|
| **SW4** | Scroll / Select (short press = next item, long press = confirm) |
| **SW5** | Power ON / START / RESUME |
| **SW6** | STOP / PAUSE / Go Back |

---

## License

This project is provided for educational and demonstration purposes.

---

## Author

**Praveen Bhusal** — [bhusalapraveen6](https://github.com/bhusalapraveen6)

---

## Acknowledgements

- [PicsimLab](https://github.com/lcgamboa/picsimlab) — Real-time PIC simulator
- [Microchip](https://www.microchip.com) — PIC16F877A & MPLAB ecosystem

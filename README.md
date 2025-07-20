# ASU RTOS Tasks

A comprehensive collection of Real-Time Operating Systems (RTOS) laboratory assignments from Ain Shams University's Computer and Systems Engineering program. This repository contains practical implementations of RTOS concepts using ARM Cortex-M4 microcontrollers (TM4C123GH6PM).

## Repository Structure

```
ASU_RTOS_Tasks/
├── docs/                           # Course documentation
│   └── Labs_Questions.pdf          # Official lab requirements and questions
├── Lab01_Manual_Tasks_Switch/      # Lab 1: Basic cooperative task switching
│   ├── main.c                      # Source code implementation
│   ├── tm4c.h                      # TM4C123 register definitions
│   ├── lab1.uvprojx               # Keil µVision project file
│   └── RTE/                        # Runtime Environment (Keil-generated)
├── .gitignore                      # Git ignore rules for build artifacts
├── LICENSE                         # Project license
└── README.md                       # This file
```

## Hardware Requirements

- **Development Board**: TM4C123GH6PM LaunchPad Evaluation Kit
- **Microcontroller**: ARM Cortex-M4F TM4C123GH6PM
- **IDE**: Keil µVision MDK-ARM or Code Composer Studio
- **Debugger**: Stellaris In-Circuit Debug Interface (ICDI)

## Course Progression

This repository follows a structured approach to learning RTOS concepts:

### Labs Content
- **Lab01**: Manual task switching and cooperative scheduling
- **Lab02**: Creating Task Stack for switching


## Getting Started

### Prerequisites
1. Install Keil µVision MDK-ARM (recommended) or Code Composer Studio
2. Install TM4C123 device support pack
3. Connect TM4C123GH6PM LaunchPad to your development machine

### Building and Running
1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/ASU_RTOS_Tasks.git
   cd ASU_RTOS_Tasks
   ```

2. Navigate to the desired lab directory
3. Open the `.uvprojx` file in Keil µVision
4. Build the project (F7)
5. Flash and debug (Ctrl+F5)

### Repository Guidelines
- Each lab is self-contained with its own project files
- Build artifacts (`Objects/`, `Listings/`) are automatically ignored
- Refer to `docs/Labs_Questions.pdf` for detailed requirements
- Follow the coding standards established in Lab01

## Lab Descriptions

### Lab01: Manual Tasks Switch
**Objective**: Implement basic cooperative task switching without using an RTOS kernel.

**Key Concepts**:
- Task function pointers
- Cooperative scheduling
- LED control and timing
- Basic task state management

**Hardware Used**: On-board LEDs (Red, Blue, Green)

*[Additional labs will be added as they are completed]*

## Development Environment

### Recommended Setup
- **IDE**: Keil µVision MDK-ARM 5.37+
- **Compiler**: ARM Compiler 6 (ARMCLANG)
- **Debugger**: CMSIS-DAP Debug Unit
- **Target Device**: TM4C123GH6PM

### Project Configuration
- **CPU Clock**: 16 MHz (default)
- **Optimization**: -O1 (balanced)
- **Debug**: Full debug information enabled
- **Memory Model**: Small (default)

## Contributing

This repository is part of academic coursework. If you're a student:

1. **DO NOT** copy solutions directly
2. Use this as a reference and learning tool
3. Understand the concepts before implementation
4. Follow your institution's academic integrity policies

For improvements or bug fixes:
1. Fork the repository
2. Create a feature branch
3. Submit a pull request with detailed description

## Course Information

- **University**: Ain Shams University
- **Faculty**: Faculty of Engineering
- **Department**: Computer and Systems Engineering
- **Course**: Real-Time Operating Systems
- **Target Audience**: Senior undergraduate students

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Course instructors and TAs at ASU Computer and Systems Engineering
- ARM Cortex-M documentation and examples
- Texas Instruments TM4C123 documentation
- Keil development tools and examples

---

⚠️ **Academic Integrity Notice**: This repository is intended for educational purposes. Students should use it as a learning resource while adhering to their institution's academic integrity policies.

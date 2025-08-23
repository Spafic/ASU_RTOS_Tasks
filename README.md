# ASU RTOS Tasks

<div align="center">

![License](https://img.shields.io/badge/license-MIT-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-ARM%20Cortex--M4-0b8f84?style=flat-square)
![IDE](https://img.shields.io/badge/IDE-Keil%20%C2%B5Vision-ffb86b?style=flat-square)
![FreeRTOS](https://img.shields.io/badge/FreeRTOS-v11.2.0-e55347?style=flat-square)
![University](https://img.shields.io/badge/University-ASU-6f42c1?style=flat-square)
[![Stars](https://img.shields.io/github/stars/Spafic/ASU_RTOS_Tasks?style=flat-square)](https://github.com/Spafic/ASU_RTOS_Tasks/stargazers)

**A comprehensive collection of Real-Time Operating Systems (RTOS) laboratory assignments**  
_From Ain Shams University's Computer and Systems Engineering Program_

Progressive Learning • Hands‑On Experience • Educational Focus

</div>

---

This repository contains practical implementations of RTOS concepts using ARM Cortex-M4 microcontrollers (TM4C123GH6PM), progressing from basic manual task switching to advanced FreeRTOS implementations.

> [!CAUTION]
> **KEIL µVision FreeRTOSConfig.h Version Issue**: KEIL5 may install FreeRTOSConfig.h version 2021 by default, which can cause problems and system hangs. It is strongly recommended to use the 2019 version of FreeRTOSConfig.h for stable operation.
 
> [!WARNING]
> **Academic Integrity Notice**: This repository is intended for educational purposes. Students should use it as a learning resource while adhering to their institution's academic integrity policies.

> [!NOTE]
> **Getting Started**: Each lab builds upon previous concepts. It's recommended to complete labs in sequence for optimal learning progression.

## Course Progression

This repository follows a structured approach to learning RTOS concepts, starting from fundamental task switching concepts and progressing to full RTOS implementation:

### Laboratory Assignments

| Lab                                                  | Title                    | Focus Area                               |
| ---------------------------------------------------- | ------------------------ | ---------------------------------------- |
| [Lab01](./Lab01_Manual_Tasks_Switch/)                | Manual Tasks Switch      | Cooperative scheduling fundamentals      |
| [Lab02](./Lab02_Creating_Task_Stacks_for_Switching/) | Creating Task Stacks     | ARM context switching & stack management |
| [Lab03](./Lab03_Introduction_to_FreeRtos/)           | Introduction to FreeRTOS | RTOS kernel basics & task creation       |
| [Lab04](./Lab04_Multiple_Tasks/)                     | Multiple Tasks           | Advanced multi-tasking & synchronization |
| [Lab05](./Lab05_Adding_Multiple_Tasks/)              | Adding Multiple Tasks    | Task management & scheduling techniques  |
| [Lab06](./Lab06_Queues_in_FreeRTOS/)                 | Queues in FreeRTOS       | Inter-task communication using queues    |
| [Lab07](./Lab07_Semaphores_in_FreeRtos/)             | Semaphores in FreeRTOS   | Task synchronization & resource management |
| [Lab08](./Lab08_Mutex_in_FreeRTOS/)                  | Mutex in FreeRTOS        | Mutual exclusion & priority inheritance  |

### Unupdated Labs

Some labs may not have full documentation or updated instructions yet. If you encounter missing details, refer to the code and comments within each lab folder. Contributions to improve documentation are welcome!

## Quick Start Guide

### Prerequisites

<details>
<summary>Click to expand setup requirements</summary>

1. **Development Environment**:

    - Install Keil µVision MDK-ARM or Code Composer Studio
    - Download TM4C123 device support pack
    - Install Windows drivers for TM4C123 LaunchPad

2. **Hardware Setup**:
    - Connect LaunchPad via USB
    - Verify device recognition in Device Manager
    - Test with basic LED blink program

</details>

### Getting Started

```bash
# 1. Clone the repository
git clone https://github.com/Spafic/ASU_RTOS_Tasks.git
cd ASU_RTOS_Tasks

# 2. Choose your lab (start with Lab01)
cd Lab01_Manual_Tasks_Switch
```

Open `.uvprojx` → Build (F7) → Flash & Debug (Ctrl+F5) → Verify LED behavior

### Repository Guidelines

- Each lab is self-contained with its own project files
- Build artifacts are ignored by Git
- Follow coding standards from Lab01
- Refer to `docs/` for detailed requirements

## Development Environment

### Recommended Setup

- **IDE**: Keil µVision MDK-ARM 5.37 or later
- **Compiler**: ARM Compiler 6 (ARMCLANG) for modern C99/C11 support
- **Debugger**: CMSIS-DAP Debug Unit with live variable monitoring
- **Target Device**: TM4C123GH6PM with 80MHz ARM Cortex-M4F core

### Project Configuration Standards

- **CPU Clock**: 16 MHz default (can be increased to 80 MHz for advanced labs)
- **Optimization**: -O1 (balanced performance/debug) for learning
- **Debug**: Full debug information enabled for educational purposes
- **Memory Model**: Small model (default for TM4C123)
- **FPU**: Enabled for Labs 3+ (floating-point unit utilization)

### Common Development Workflow

1. **Analysis**: Read lab requirements and understand objectives
2. **Design**: Plan your implementation approach
3. **Implementation**: Write code following established patterns
4. **Testing**: Use debugger to verify functionality
5. **Documentation**: Comment your code and understand the concepts

## Educational Value

### Learning Progression

This course provides a structured path through embedded systems and RTOS concepts:

1. **Foundational Knowledge** (Lab 1): Basic task switching and timing
2. **Low-Level Understanding** (Lab 2): ARM architecture and stack management
3. **RTOS Introduction** (Lab 3): Transition to real-time kernel usage
4. **Advanced Concepts** (Lab 4+): Multi-tasking and synchronization

### Skills Developed

- **Embedded Programming**: C programming for microcontrollers
- **Real-Time Systems**: Understanding of timing constraints and deterministic behavior
- **Debugging Techniques**: Advanced use of IDE debugging tools
- **System Architecture**: Knowledge of ARM Cortex-M architecture
- **RTOS Concepts**: Task management, scheduling, and inter-task communication

## Troubleshooting

### Common Issues and Solutions

#### Build Errors

- **Missing Device Pack**: Install TM4C123 support pack from Keil
- **Compiler Errors**: Ensure ARM Compiler 6 is selected
- **Include Path Issues**: Verify all header files are in project directory

#### Hardware Issues

- **Board Not Detected**: Check USB cable and Windows drivers
- **Flash Programming Fails**: Ensure no other debugger sessions are active
- **LED Behavior Incorrect**: Verify GPIO initialization and pin assignments

#### FreeRTOS Specific (Labs 3+)

- **System Hangs**: Check heap size and stack overflow settings
- **Tasks Don't Run**: Verify scheduler is started with `vTaskStartScheduler()`
- **Timing Issues**: Confirm tick rate configuration matches expected behavior

### Debug Tips

- Use breakpoints strategically to understand program flow
- Monitor register values to verify hardware configuration
- Utilize memory windows to inspect stack and variable contents
- Enable serial output for runtime debugging information

## Contributing and Academic Use

### For Students

> [!IMPORTANT]
> This repository is designed as a **learning tool**, not a solution bank:

1. **Study the concepts** before looking at implementations
2. **Understand the code** rather than copying it directly
3. **Follow your institution's** academic integrity policies
4. **Use this as reference** to verify your own implementations

### For Educators

- Feel free to fork and adapt for your own courses
- Contributions that improve educational value are welcome
- Please maintain the academic integrity guidelines

### Contributing Guidelines

1. **Fork the repository** and create a feature branch
2. **Maintain code quality** and documentation standards
3. **Test thoroughly** on actual hardware before submitting
4. **Submit pull requests** with detailed descriptions of improvements

## Resources and References

### Official Documentation

- [TM4C123GH6PM Datasheet](https://www.ti.com/product/TM4C123GH6PM)
- [ARM Cortex-M4 Programming Manual](https://developer.arm.com/documentation/dui0553/latest/)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)
- [Keil µVision User Guide](https://www.keil.com/support/man/docs/uv4/)

### Educational Resources

- Real-Time Systems textbooks and course materials
- ARM Cortex-M architecture guides
- Embedded systems design principles
- RTOS fundamentals and best practices

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

This permissive license allows for educational use, modification, and distribution while maintaining attribution to the original work.

## Acknowledgments

- **Course instructors and TAs** at ASU Computer and Systems Engineering Department ([Shahd-AbouHashem](https://github.com/Shahd-AbouHashem))
- **ARM and Texas Instruments** for comprehensive documentation and development tools
- **FreeRTOS community** for excellent RTOS implementation and educational resources
- **Keil/ARM** for providing industry-standard development tools
- **Student contributors** who have helped improve and test these implementations

Last Updated: August 2025 | Maintained by:  
[Spafic](https://github.com/Spafic), [Lucifer3224](https://github.com/Lucifer3224), [ShahdAshraff](https://github.com/ShahdAshraff)

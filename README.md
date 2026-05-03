
## MK Sorter - Hardware Based Sorting Algorithm Visualizer

This project implements a hardware based sorting algorithm visualizer by writing software for the bare metal Nios V processor in C. Key features of the project include:
   
- PS/2 Mouse Support
- Fully Interrupt Driven Memory Mapped IO interactions
- 4 different sorting algorithms to visualize
- 4 different options for the # of bars to display 


## Demo
https://github.com/user-attachments/assets/f926f3b4-2a45-473c-b317-26929681e8ac
## Install and Run

To run this application, the following pieces of hardware are needed:

- DE1-SoC or DE10-Lite board
- PS/2 mouse
- VGA monitor 

The appropriate tools for the Nios V processor must be downloaded including the:

- Quartus Prime Programmer & Tools (preferrably 24.1.0)
- Ashling RiscFree IDE which includes the C compiler and support for terminal GDB

This can be simply done by following **Section 2** from the official GDB tutorial from the FPGAcademy called **Using GDB with Nios V** under the Computer *Organization System Design* section. 
The tutorial can be found at this [link](https://fpgacademy.org/tutorials.html)

Once the appropriate tools are installed, download the project and unzip into any folder of choice. Then CD into the **build_home** directory and open three terminals. 

In Terminal 1, run the commands:

```
./gmake DE1-SoC
./gmake GDB_SERVER
```

In Terminal 2, run the commands:

```
./gmake TERMINAL
```

In Terminal 3, run the commands:

```
./gmake COMPILE
./gmake GDB_CLIENT
```

Follow through the prompts on the CLI debugger and then type `continue` to enter the main program loop. Now you should be able to see the display on the monitor and use the mouse to interact with the program.
Use the KEY [0-3] buttons on the board to select the number of bars to display.

# Bus System Simulation

**Description:**  
This project is a simulation of a bus system where threads are used to represent buses and passengers. The objective is to implement synchronization functions that ensure the orderly loading of passengers onto buses. The program should manage threads efficiently to prevent conflicts and ensure buses depart only when fully loaded or no more waiting passengers are left.

---

## Table of Contents
- [Project Overview](#project-overview)
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

---

## Project Overview
In this project, you will simulate a bus transportation system. The key responsibilities include:

- Implementing a structure called `bus` to handle synchronization between buses and passengers.
- Using synchronization functions to manage the process of boarding passengers onto buses. This involves:
  - Ensuring that buses do not depart until they are either full or no additional passengers are waiting.
  - Keeping track of available seats and signaling when a passenger has boarded.
- Writing functions that synchronize threads to handle loading and waiting states without deadlocks or busy waiting.

The project requires the use of `Pintos` synchronization primitives (`CV` and `lock`) in C. Semaphores are not allowed.

---

## Installation
To set up the project locally, ensure that you have the required dependencies installed and follow these steps:

```bash
# Clone the repository
git clone https://github.com/gnozadi/bus-system-simulation.git

# Navigate to the project directory
cd bus-system-simulation

# Compile the code
make
```

## Usage
Once the code is compiled, you can use the provided `bus-simulator.c` file to test your implementation with randomized inputs.

```bash
# Run the bus simulator to test the synchronization functions
./bus-simulator
```

## Features
The project implements the following features:

- **Bus Synchronization**: Controls the loading of buses, ensuring they only depart when fully loaded or no additional passengers are present.
- **Passenger Synchronization**: Manages passenger threads to board buses efficiently, preventing conflicts and ensuring correct seat assignment.
- **Simultaneous Passenger Boarding**: Allows multiple passengers to board the bus concurrently, improving simulation performance.

## Project Structure
- **bus.c**: Contains the structure and functions related to the `station` object and synchronization.
- **bus-sim.c**: Provides a testing environment to verify that the synchronization mechanisms work correctly.


## Contributing
Contributions are welcome! Here’s how you can contribute:

1. Fork the repository
2. Create a new branch (`git checkout -b feature-branch`)
3. Make your changes
4. Push to the branch (`git push origin feature-branch`)
5. Open a Pull Request

## License
This project is licensed under the MIT License.

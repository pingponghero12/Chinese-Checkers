# Chinese Checkers - Client-Server Implementation

A modern client-server implementation of the classic Chinese Checkers game with GUI interface and network multiplayer support.

**Developed by:** Manfred Gawlas and Tomasz Piotrowski

## Description
This project implements a scalable and modular client-server architecture for Chinese Checkers, utilizing modern design patterns to ensure flexibility and maintainability.

## Features

- 🌐 Network multiplayer gameplay
- 🖥️ Graphical user interface (GUI) using PyQt5
- 🤖 Potential bot integration (see server/bot.hpp)
- 💾 Game history tracking with MariaDB database
- ⚙️ High-performance core logic in C++ with Python bindings
- 🐳 Dockerized server deployment
- 📜 Move history and game state tracking

## Design Patterns

This project follows several design patterns to ensure modularity, maintainability, and scalability. The Command Pattern is used for handling game commands (AbstractCommand), where concrete commands like CmdAddGame and CmdExitGame encapsulate specific actions, making the system extensible. The Factory Method Pattern allows ServerController to dynamically create and register game instances, providing flexible game management. The Strategy Pattern is evident in the Board class, which supports multiple board-filling strategies such as fill_top_triangle and fill_bottom_triangle, enabling different game configurations without modifying core logic. The Proxy Pattern is implemented in ServerController, which acts as an intermediary between the Server and game logic, abstracting direct access and handling network communication. Dependency Injection is utilized in ServerController, which receives dependencies like DbConnector through its constructor, promoting loose coupling and testability. The Observer Pattern is used as ServerController listens for server events and updates player statuses dynamically, ensuring efficient communication between components. These patterns contribute to a robust and scalable architecture for the Chinese Checkers multiplayer experience.

## System Architecture

### Component Diagram
![System diagram](uml/system.png)

### Client-Server Communication
![Activity diagram](uml/activitydiagram.png)

### Prerequisites
- **Nix Package Manager** (for dependency management)
- CMake (≥3.15)
- C++17 compiler
- Python 3.10+
- Docker and docker-compose
- Qt5 development libraries
- MariaDB client libraries

### Development Environments
This project uses Nix shells for dependency management:
- **General dependencies**: `shell.nix` (root directory)
- **Server-specific dependencies**: `server/shell.nix`

Enter the development environments with:
```bash
# For general development
nix-shell shell.nix

# For server development
cd server
nix-shell shell.nix

### Building from Source

**Client Application:**
```bash
cd client/build
cmake ..
make
# Run the client user interface
python3 ../gui/main.py

**Server Application:**
``bash
cd server/build
cmake ..
make
# Run the server
./server
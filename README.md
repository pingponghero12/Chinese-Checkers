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
Following design patterns were utilized:
1. Server as Facade
2. BoardFactory as Factory method
3. Commands
4. Dependency Injection of ServerController
5. Db_Connector as Facade
6. Boards as strategy

Beside that, ServerController could be made to also utilize State, Proxy and Observer, but we choose otherwise.

## System Architecture

### Component Diagram
![System diagram](uml/system.png)

### Client-Server Communication
![Activity diagram](uml/activitydiagram.png)

### Prerequisites
- **Nix Package Manager** (for dependency management)
- CMake (≥3.15)
- C++11 compiler
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
# For client development
nix-shell --run bash

# For server development
cd server
nix-shell --run bash
```

Build and run client
```bash
# Client
cd client
bash run.sh
# Run the client user interface
python3 gui/main.py

```
Build and run server
```bash
cd server/build
cmake ..
make

cd ../docker

# Esure running docker daemon
sudo dockerd &

# Reset and compose server
sudo docker-compose down -v
sudo docker-compose build --no-cache
sudo docker-compose up -d

# Test server 
sudo docker-compose exec db mariadb -u admin -p papers_db

# In case of error
sudo docker-compose logs --tail=50 -f db

# Run server(DB needs to be up)
./build/server

```

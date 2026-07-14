# La Poción Primordial (C++ roguelike game)

<img width="3840" height="2160" alt="pocion-primordial" src="https://github.com/user-attachments/assets/1597395f-ee39-4c46-88ab-bc9f40ca5aed" />
<img width="3840" height="2160" alt="game-idea" src="https://github.com/user-attachments/assets/8021d7bf-f902-4b6c-943f-1e6abc9f0ab5" />

A roguelike, top-down arena game written in C++ using the SFML multimedia library. Defeat waves of enemies, collect various gameplay-enhancing potions, and defeat the elemental level bosses to survive!

## Features

<img width="3840" height="2160" alt="features" src="https://github.com/user-attachments/assets/2795b8fa-acfe-49bd-9a1d-a312bffffd44" />


- **Top-Down Combat**: Move, aim, and shoot using responsive keyboard and mouse controls.
- **Dynamic Potion System**: Collect potions spawned around the arena to buff your stats:
  - **Health Potion**: Restores player health.
  - **Speed Potion**: Boosts movement speed temporarily.
  - **Fire Rate Potion**: Increases shooting speed temporarily.
- **Multiple Game States**: Smooth transitions between the main menu, gameplay, pause menu, victory screens, and game over screens.
- **Enemy & Boss Logic**: Face standard enemies and specialized Level Bosses with unique attack patterns and custom health bars.
- **Immersive Sound**: Retro sound effects and background music tailored to elemental themes (Water, Fire).

Two elemental realms, infinite hordes. Can you survive the water and the fire?

<img width="3840" height="2160" alt="first-level" src="https://github.com/user-attachments/assets/20a69724-b139-4f32-8547-8ecc170736bb" />
<img width="3840" height="2160" alt="second-level" src="https://github.com/user-attachments/assets/0e369f2f-7d67-4e2d-903e-70029ffc9b21" />



---

## Folder Structure

- **`src/pro/pocionPrimordial`**: The main game project folder containing:
  - **`main.cpp`**: Main entry point initializing the game loop.
  - **`CMakeLists.txt`**: CMake build configuration.
  - **`include/`**: Project headers and config variables.
  - **`modules/`**: Game architecture modules (States, Entities, Map, HealthBar, Systems, Camera, etc.).
  - **`resources/`**: All visual sprites, maps, font files, audio clips, and music.
- **`proyecto.code-workspace`**: VS Code workspace configurations for convenient development setup.

---

## Prerequisites

To build and run the game, you need:

1. **C++ Compiler**: A compiler supporting C++14 or newer (e.g., GCC, Clang, or MSVC).
2. **CMake**: Version 2.8 or higher.
3. **SFML Library (>= 2.5)**: Graphic, window, system, network, and audio modules.

### Installing SFML

- **Linux (Ubuntu/Debian)**:
  ```bash
  sudo apt-get update
  sudo apt-get install libsfml-dev cmake clang
  ```
- **macOS (via Homebrew)**:
  ```bash
  brew install sfml cmake
  ```
- **Windows (via WSL - Windows Subsystem for Linux)**:
  If you use WSL, you can install the Linux dependencies inside your WSL distribution (e.g., Ubuntu):
  ```bash
  sudo apt-get update
  sudo apt-get install libsfml-dev cmake g++
  ```
- **Windows (Native)**:
  Download the appropriate SFML package matching your compiler from the [official SFML website](https://www.sfml-dev.org/download.php) and configure your system variables to point to it, or install via package managers like `vcpkg`.

---

## How to Compile and Run

1. Navigate to the game directory:
   ```bash
   cd src/pro/pocionPrimordial
   ```
2. Create a build directory and enter it:
   ```bash
   mkdir build
   cd build
   ```
3. Generate the build files:
   - On Linux / macOS / WSL:
     ```bash
     cmake ..
     ```
   - On Windows (Native):
     Generate build files specifying your compiler (e.g., Visual Studio or MinGW).
4. Compile the project:
   - **Linux / macOS**:
     ```bash
     make
     ```
   - **Windows (WSL)**:
     Run the compilation inside WSL from your Windows terminal:
     ```bash
     wsl sh -c "cd build && make"
     ```
   - **Windows (MinGW/GCC)**:
     ```bash
     mingw32-make
     ```
   - **Windows (Visual Studio)**:
     Open the generated `.sln` solution file in Visual Studio and build the target.
5. Run the game:
   - On Linux/macOS:
     ```bash
     ./MiJuego
     ```
   - On Windows (WSL):
     ```bash
     wsl ./MiJuego
     ```
   - On Windows (Native):
     ```cmd
     MiJuego.exe
     ```

---

## Authors and Credits

This project was developed as part of the **Fundamentos de los Videojuegos** course for the **Grado en Ingeniería Multimedia** at the **Escuela Politécnica Superior, Universidad de Alicante**.

* **Álvaro Sánchez** - [@asp161](https://github.com/asp161)
* **Álvaro Aldea** - [@aag166](https://github.com/aag166)
* **Jorge Román** - [@jorgeromangil](https://github.com/jorgeromangil)
* **Claudia Garias** - [@cga112](https://github.com/cga112)

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

# Hotel Management System

A cyberpunk-themed hotel management system built with C++20 and SFML 3.

![Project Status](https://img.shields.io/badge/status-active-brightgreen)
![Language](https://img.shields.io/badge/language-C%2B%2B20-blue)
![Framework](https://img.shields.io/badge/framework-SFML%203.0.2-purple)
![License](https://img.shields.io/badge/license-Unlicensed-gray)

## Overview

This project is a GUI-based hotel management application that ditches standard windows forms for a custom, high-contrast neon interface. It was built to demonstrate how to create interactive data-driven applications in C++ using only a graphics library (SFML) rather than a full GUI framework like Qt or ImGui.

It solves the problem of managing basic hotel operations—booking rooms, tracking guests, and calculating revenue—while wrapping it in a visually engaging "Cyberpunk v1.0" aesthetic. It's designed for developers looking for examples of custom UI widget implementation (buttons, text inputs, checkboxes) from scratch in SFML.

## Features

### Core Management
*   **Guest Directory**: Add new guests with validation for phone numbers (digits only) and email formats. View the full list of registered guests.
*   **Room Visualization**: View the status of all rooms (Standard, Deluxe, Suite). Occupied rooms are visually distinct (Red) from available ones (Green).
*   **Reservation System**: 
    *   Link guests to specific rooms.
    *   Calculate total cost based on room type, duration, and meal plans.
    *   Automatic conflict detection (prevents double-booking).
*   **Check-in / Check-out**: Manage the lifecycle of a guest's stay.
*   **Statistics Dashboard**: Real-time view of occupancy rates and simple metrics.

### Technical & UI
*   **Custom UI Engine**: All widgets are built from primitive SFML shapes, not external libraries.
    *   `NeonButton`: Hover effects, glow animations, and click handling.
    *   `NeonTextInput`: Focus state, blinking cursor, and text entry handling.
    *   `RoundedRectangleShape`: Custom vertex array implementation for smooth rounded corners.
*   **Input Validation**: robust handling of text input events to prevent invalid data entry.
*   **State Management**: Simple state machine pattern to switch between views (Home, Add Guest, Reservations, etc.).

## Demo

<!-- Add screenshots or GIFs here -->
![Main Menu](screenshots/main_menu.png)
*Placeholder: A screenshot showing the main dashboard with the neon glow effect.*

## Getting Started

### Prerequisites

*   **Windows 10/11**
*   **Visual Studio 2022** (with "Desktop development with C++" workload)
*   **SFML 3.0.2**
    *   This project is configured to look for SFML at `C:\SFML-3.0.2\`. If you installed it elsewhere, you'll need to update the project properties.

### Installation

1.  **Clone the repository**
    ```bash
    git clone https://github.com/yourusername/neon-hotel-management.git
    cd neon-hotel-management
    ```

2.  **Verify SFML Installation**
    Ensure you have the SFML 3.0.2 SDK extracted to `C:\SFML-3.0.2\`. 
    *   It should contain `include/` and `lib/` folders.

3.  **Open the Project**
    Double-click `ConsoleApplication1.sln` (or `.slnx`) to open in Visual Studio.

4.  **Build and Run**
    *   Select **x64** and **Debug** (or Release) from the toolbar.
    *   Press `F5` or click **Local Windows Debugger**.
    *   *Note: Ensure the SFML DLLs (`sfml-graphics-3.dll`, etc.) are in the output directory or your system PATH.*

### Quick Start

Once the app is running:
1.  Click **"Add Guest"** in the sidebar.
2.  Enter details (Name: "John Doe", Phone: "1234567890") and click **Submit**.
3.  Go to **"Reserve"**.
4.  Enter the Guest ID (found in **"Guests"** tab) and a Room Number (e.g., "101").
5.  Click **Make Reservation**.
6.  Check the **"Rooms"** tab to see Room 101 turn Red (Occupied).

## Configuration

The project uses a `Config` namespace in `ConsoleApplication1.cpp` for easy tweaking of global settings. You can modify these values and recompile:

| Variable | Description | Default |
| :--- | :--- | :--- |
| `WINDOW_WIDTH` | Application window width | `1000` |
| `WINDOW_HEIGHT` | Application window height | `700` |
| `NEON_CYAN` | Primary accent color | `(0, 240, 255)` |
| `PANEL_BG` | Background color for panels | `(18, 18, 26, 240)` |

## Project Structure

```text
projectcpp hotel/
├── ConsoleApplication1/
│   ├── ConsoleApplication1.cpp       # Main source file (Entry point & Logic)
│   ├── ConsoleApplication1.vcxproj   # Visual Studio Project file
│   └── ...                           # VS configuration files
├── ConsoleApplication1.sln           # Solution file
└── README.md                         # This file
```

## How it works

The application runs a standard game loop (Input -> Update -> Render).

1.  **Event Polling**: `sf::Event` is polled to handle window closing and text input.
2.  **Update Phase**:
    *   Mouse position is tracked.
    *   `NeonButton::update()` checks for hover/click states.
    *   `NeonTextInput` handles focus and character injection.
3.  **Render Phase**:
    *   The window is cleared with `BG_DARK`.
    *   The Sidebar and Header are drawn.
    *   Based on `AppState` (an enum), the specific "page" (inputs, lists, stats) is rendered.
    *   `window.display()` flips the buffer.

The data model (`HotelSystem`, `Guest`, `Room`) is kept entirely in memory using `std::vector` and `std::unique_ptr`.

## Built With

*   [C++20](https://en.cppreference.com/w/cpp/20) - The language used.
*   [SFML 3.0.2](https://www.sfml-dev.org/) - Simple and Fast Multimedia Library for graphics and windowing.
*   [Visual Studio 2026](https://visualstudio.microsoft.com/) - IDE and Compiler (MSVC).

## Contributing

Contributions are welcome! If you want to add a database backend or improve the UI:

1.  Fork the project.
2.  Create your feature branch (`git checkout -b feature/AmazingFeature`).
3.  Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4.  Push to the branch (`git push origin feature/AmazingFeature`).
5.  Open a Pull Request.

## License

This project is currently unlicensed. A license file will be added soon.

## Acknowledgments

*   Inspired by the Cyberpunk 2077 UI aesthetic.
*   Thanks to the SFML community for the excellent documentation.

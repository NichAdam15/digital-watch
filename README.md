# Digital Watch

This is a firmware for a custom digital watch based on the ESP32-C3 Super Mini and a round 1.28" GC9A01 LCD display. It features a clean, modern interface with live weather data.

## Features

*   **Main Watch Face:**
    *   Large, easy-to-read 12-hour time with AM/PM indicator.
    *   Current date (e.g., "Sat, Jun 21").
    *   Current weather conditions, including an icon, temperature, and location.
    *   Battery level indicator.
*   **Dedicated Weather Page:**
    *   A full-screen view with more detailed weather information.
    *   Dynamic background color that changes based on the weather (Orange for clear, Grey for clouds, Blue for rain).
    *   Large current temperature with a custom-drawn degree symbol.
    *   High and low temperatures for the day.
    *   Location and current date.
*   **Connectivity:**
    *   Connects to WiFi to sync time with an NTP server.
    *   Fetches live weather data from the OpenWeatherMap API.
*   **User Interface:**
    *   A single button allows the user to toggle between the main watch face and the weather page.
    *   Robust startup sequence that displays the status of WiFi, time sync, and weather fetching.

## Hardware

*   **Microcontroller:** ESP32-C3 Super Mini
*   **Display:** 240x240 Round GC9A01 LCD
*   **Input:** A single tactile push-button for interaction (connected to GPIO 6).

## Software & Libraries

This project is built using the Arduino framework with PlatformIO.

### Key Libraries:
*   `TFT_eSPI`: For driving the GC9A01 display.
*   `WiFi`: For network connectivity.
*   `HTTPClient`: For making API requests to OpenWeatherMap.
*   `Arduino_JSON`: For parsing the weather data returned by the API.

### Code Structure

The firmware is designed with a modular structure for clarity and maintainability.

*   `main.cpp`: The main entry point and high-level coordinator of the application.
*   `network.cpp`/`.h`: Manages WiFi connection and NTP time synchronization.
*   `weather.cpp`/`.h`: Manages fetching and parsing data from the OpenWeatherMap API.
*   `button.cpp`/`.h`: Handles all button press and debounce logic.
*   `time_utils.cpp`/`.h`: Provides helper functions for formatting dates and times.
*   **UI Modules:**
    *   `startup_ui.cpp`/`.h`: Draws the initial startup and status screen.
    *   `main_screen.cpp`/`.h`: Responsible for drawing the main watch face.
    *   `weather_screen.cpp`/`.h`: Responsible for drawing the detailed weather page.

## Configuration

Before flashing, you must configure two files:

1.  **WiFi Credentials:**
    *   Open `firmware/src/network.cpp`.
    *   Set your `ssid` and `password`.
    ```cpp
    const char* ssid = "YOUR_WIFI_SSID";
    const char* password = "YOUR_WIFI_PASSWORD";
    ```

2.  **OpenWeatherMap API Key:**
    *   Open `firmware/src/weather.cpp`.
    *   Set your `api_key` and `city`.
    *   You can get a free API key by signing up at [OpenWeatherMap](https://openweathermap.org/appid).
    ```cpp
    const char* city = "Union,KY,US"; // Your city
    const char* api_key = "YOUR_API_KEY";
    ```

## How to Build and Upload

1.  **Install PlatformIO:** Make sure you have the [PlatformIO IDE extension](https://platformio.org/platformio-ide) installed in VS Code.
2.  **Open Project:** Open the `firmware` directory in PlatformIO.
3.  **Clean (Optional but Recommended):** Run the "Clean" project task to remove old build files.
4.  **Upload:** Connect your ESP32-C3 board and run the "Upload" project task. If PlatformIO doesn't find the port automatically, you may need to specify it using the `--upload-port` flag (e.g., `pio run --target upload --upload-port COM4`).


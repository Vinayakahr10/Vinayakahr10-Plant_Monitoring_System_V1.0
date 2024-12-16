# Server-Based Plant Monitoring System

## Overview
This project is a **Server-Based Plant Monitoring System** using an **ESP32 microcontroller**. It integrates multiple sensors to monitor soil moisture, temperature, and humidity levels in real time. The data is displayed on a web-based dashboard hosted by the ESP32, allowing users to access it from any browser.

## Features
- **Real-time Sensor Monitoring**:
  - Soil Moisture
  - Temperature
  - Humidity
- **Web Dashboard**:
  - Interactive, responsive, and user-friendly.
  - Automatic and manual refresh options.
- **API Endpoint**:
  - `/get-sensor-data` provides JSON-formatted sensor readings for integration with other applications.
- **Error Handling**:
  - Detects and handles invalid sensor readings gracefully.

---

## Requirements
### Hardware:
- ESP32 microcontroller
- DHT22 Sensor
- Soil Moisture Sensor
- Breadboard and connecting wires

### Software:
- Arduino IDE or PlatformIO
- ESP32 board support in Arduino IDE
- Libraries:
  - [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
    

---

## Installation

### Step 1: Clone the Repository
```bash
git clone https://github.com/yourusername/plant-monitoring-system_V1.0.git
cd plant-monitoring-system_V1.0
````
### Step 2: Configure WiFi Credentials
1. Open the `Plant_Monitoring_system_V1.0.ino` file in the Arduino IDE.
2. Replace the placeholder WiFi credentials in the following lines:
   ```cpp
   const char* ssid = "YOUR_SSID";       // Replace with your WiFi SSID
   const char* password = "YOUR_PASSWORD"; // Replace with your WiFi password
### Step 3: Set Up the Hardware
1. **Connect the DHT22 Sensor**:
   - Connect the **Data Pin** of the DHT22 to **GPIO 15** on the ESP32.
   - Connect the **VCC Pin** to **5V** and **GND Pin** to **GND**.

2. **Connect the Soil Moisture Sensor**:
   - Connect the **Analog Output Pin (AO)** of the soil moisture sensor to **GPIO A0** (or another ADC-capable pin on the ESP32).
   - Connect the **VCC Pin** to **5V** and **GND Pin** to **GND**.

3. **Power the Circuit**:
   - Use the USB cable to connect the ESP32 to your computer for both power and programming.

4. Double-check the wiring to ensure all connections are secure and correct.

### Step 4: Access the Dashboard
1. After uploading the code to the ESP32, open the **Serial Monitor** in the Arduino IDE.
   - Ensure the baud rate is set to **115200**.
2. Once the ESP32 connects to your WiFi network, it will display the **IP address** assigned by the router on the Serial Monitor. For example:`http://192.168.1.100`

3. Open a web browser on any device connected to the same WiFi network.
4. Enter the ESP32's IP address in the browser's address bar (e.g., `http://192.168.1.100`).
5. You will see the **Sensor Dashboard**, which displays:
- Humidity
- Temperature
- Soil Moisture
6. Click the **Refresh** button on the dashboard to manually update the sensor readings, or wait for the data to auto-refresh every 5 seconds.
## Usage
### Dashboard Features:
1. **Real-Time Monitoring**:
   - Displays the latest readings for:
     - **Humidity** (in percentage)
     - **Temperature** (in °C)
     - **Soil Moisture** (in percentage)
   - Automatically refreshes every 5 seconds or can be refreshed manually using the "Refresh Data" button.

2. **Error Alerts**:
   - If sensor data cannot be read, the dashboard will display an error message:  
     *"Error fetching data!"*

3. **REST API**:
   - Access raw sensor data in JSON format by navigating to `/get-sensor-data`.
   - Example API response:
     ```json
     {
       "humidity": 55.5,
       "temperature": 22.3,
       "soilMoisture": 72
     }
     ```
   - This can be used for further integration with other IoT applications or systems.

---

## Customization
You can modify the project as follows:
1. **Change Dashboard Appearance**:
   - Edit the `getPage()` function in the code to customize the HTML/CSS for the web dashboard.

2. **Sensor Calibration**:
   - If the soil moisture sensor readings are inaccurate, adjust the calculation in the `loop()` function:
     ```cpp
     moisture = 100 - ((sensor_analog / 4095.0) * 100);
     ```
   - Replace `4095.0` with the maximum analog value specific to your sensor.

3. **Add More Sensors**:
   - You can connect additional sensors (e.g., light sensors, pH sensors) and extend the dashboard by updating the `getPage()` function and `/get-sensor-data` endpoint.

---

## Known Issues
1. **WiFi Connection Failure**:
   - If the ESP32 cannot connect to WiFi after 10 seconds, it will restart automatically.
   - Ensure your WiFi credentials are correct and the network is active.

2. **Invalid Sensor Readings**:
   - If the DHT22 or soil moisture sensor returns `NaN`, check the sensor connections and power supply.

3. **Browser Compatibility**:
   - The dashboard is tested on modern browsers like Chrome, Firefox, and Edge. Older browsers may not render the design correctly.

---

## Future Enhancements
Planned improvements for this project:
1. **Data Logging**:
   - Store sensor readings on an SD card or cloud platform for historical analysis.
2. **Graphical Dashboard**:
   - Add live charts for visualizing sensor data trends.
3. **Mobile App Integration**:
   - Develop a companion app for mobile devices.
4. **Power Optimization**:
   - Implement deep sleep mode on the ESP32 to reduce power consumption.
5. **Notification Alerts**:
   - Send alerts via email or SMS when soil moisture drops below a threshold.

---

## License
This project is licensed under the **MIT License**. You are free to use, modify, and distribute it, provided proper attribution is given. See the `LICENSE` file for more details.

---

## Contributing
Contributions are welcome! Here’s how you can help:
1. **Report Issues**: Found a bug? Open an issue in the repository.
2. **Improve Documentation**: Help enhance this `README.md`.
3. **Add Features**: Fork the repository, implement a feature, and submit a pull request.

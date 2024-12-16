
// Program written by Vinayaka HR
// Server-based Plant Monitoring System v.1

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <DHT.h>


// Define the DHT sensor pin and type
#define DHTPIN 15             // Pin connected to the DHT sensor
#define DHTTYPE DHT22         // DHT 22 (AM2302)

// Initialize the DHT sensor
DHT dhtSensor(DHTPIN, DHTTYPE);

WebServer server(80);

int moisture = 0, sensor_analog;
const int sensor_pin = A0;  /* Soil moisture sensor O/P pin */

const char* ssid = ""YOUR_SSID";         /* Enter Your SSID */
const char* password = "YOUR_PASSWORD";    /* Enter Your Password */

/* HTML dashboard page */
String getPage() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Sensor Dashboard</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 0;
            background: linear-gradient(to bottom right, #1e3c72, #2a5298);
            color: #333;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
        }
        header {
            width: 100%;
            text-align: center;
            padding: 1rem 0;
            background: rgba(0, 0, 0, 0.3);
            color: white;
            font-size: 2rem;
            font-weight: bold;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.6);
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.2);
        }
        .container {
            max-width: 900px;
            width: 90%;
            margin: 2rem auto;
            padding: 1rem;
            display: grid;
            gap: 1.5rem;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
        }
        .card {
            background: #f0f4f8;
            border-radius: 12px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
            text-align: center;
            padding: 1.5rem 1rem;
            transition: transform 0.2s ease, box-shadow 0.2s ease;
        }
        .card:hover {
            transform: translateY(-5px);
            box-shadow: 0 6px 12px rgba(0, 0, 0, 0.3);
        }
        .card strong {
            font-size: 1.2rem;
            color: #1e3c72;
        }
        .card p {
            font-size: 2rem;
            margin: 0.5rem 0;
            color: #333;
        }
        .refresh {
            grid-column: 1/-1;
            background: #1e88e5;
            color: white;
            font-size: 1rem;
            border: none;
            padding: 1rem;
            cursor: pointer;
            border-radius: 6px;
            text-transform: uppercase;
            letter-spacing: 1px;
            transition: background 0.3s ease, transform 0.2s ease;
        }
        .refresh:hover {
            background: #1565c0;
            transform: translateY(-3px);
        }
        .error-message {
            color: red;
            font-weight: bold;
            grid-column: 1/-1;
            text-align: center;
            margin-top: 1rem;
        }
        footer {
            margin-top: 2rem;
            font-size: 0.9rem;
            color: white;
            text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.5);
        }
    </style>
</head>
<body>
    <header>
        ESP32 Sensor Dashboard
    </header>
    <div class="container">
        <div class="card" aria-label="Humidity reading">
            <strong>Humidity</strong>
            <p id="humidity">Loading...</p>
        </div>
        <div class="card" aria-label="Temperature reading">
            <strong>Temperature</strong>
            <p id="temperature">Loading...</p>
        </div>
        <div class="card" aria-label="Soil Moisture reading">
            <strong>Soil Moisture</strong>
            <p id="soilMoisture">Loading...</p>
        </div>
        <button class="refresh" onclick="fetchSensorData()">Refresh Data</button>
        <div id="errorMessage" class="error-message" style="display: none;">Error fetching data!</div>
    </div>
    <footer>
        <p>Real-time Monitoring Powered by ESP32</p>
    </footer>
    <script>
        async function fetchSensorData() {
            const errorMessage = document.getElementById('errorMessage');
            errorMessage.style.display = 'none';

            try {
                const response = await fetch('/get-sensor-data');
                if (!response.ok) throw new Error("Network response was not ok");

                const data = await response.json();
                document.getElementById('humidity').innerText = data.humidity + '%';
                document.getElementById('temperature').innerText = data.temperature + '°C';
                document.getElementById('soilMoisture').innerText = data.soilMoisture + '%';
            } catch (error) {
                errorMessage.style.display = 'block';
            }
        }

        setInterval(fetchSensorData, 5000);
    </script>
</body>
</html>
)rawliteral";
  return page;
}

void MainPage() {
  String html = getPage();
  server.send(200, "text/html", html);
}

void SoilMoisture() {
  float humidity = dhtSensor.readHumidity();
  float temperature = dhtSensor.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    server.send(500, "application/json", "{\"error\":\"Failed to read sensor data\"}");
    return;
  }

  String data = "{\"humidity\":" + String(humidity) + ",\"temperature\":" + String(temperature) + ",\"soilMoisture\":" + String(moisture) + "}";
  server.send(200, "application/json", data);
}

void setup() {
  Serial.begin(115200);

  dhtSensor.begin();  // Initialize the DHT sensor

  WiFi.begin(ssid, password);
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) { // Try for 10 seconds
    delay(500);
    Serial.print(".");
    retry++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi. Restarting...");
    ESP.restart();
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", MainPage);
  server.on("/get-sensor-data", SoilMoisture);
  server.begin();
}

void loop() {
  sensor_analog = analogRead(sensor_pin);
  if (sensor_analog < 0 || sensor_analog > 4095) {
    Serial.println("Invalid soil moisture sensor reading.");
    return;
  }
  moisture = 100 - ((sensor_analog / 4095.0) * 100);

  server.handleClient();

  float temperature = dhtSensor.readTemperature();
  float humidity = dhtSensor.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("Temp: " + String(temperature, 2) + "°C");
  Serial.println("Humidity: " + String(humidity, 1) + "%");
  Serial.println("Soil Moisture: " + String(moisture) + "%");
  Serial.println("---");
}

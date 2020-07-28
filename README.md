# ESP8266 Environment Sensor Exporter

A simple sketch that will return Prometheus formatted metrics for the BME280
humidity and pressure sensor.

## Example Output

```
# HELP esp8266_temperature_celsius Current temperature in Celsius.
# TYPE esp8266_temperature_celsius gauge
esp8266_temperature_celsius{sensor="1"} 29.32
# HELP esp8266_pressure_hpa Current pressure in hPa.
# TYPE esp8266_pressure_hpa gauge
esp8266_pressure_hpa{sensor="1"} 1001.09
# HELP esp8266_relative_humidity Current relative humidity.
# TYPE esp8266_relative_humidity gauge
esp8266_relative_humidity{sensor="1"} 56.87
```

## Building

Please follow the [Arduino Guide](https://www.arduino.cc/en/Guide/HomePage) for
specific instructions for your platform. You will need to rename the `arduino_secrets.h.example`
file to `arduino_secrets.h` and add your WiFi SSID and password to the relevant
fields. If you have multiple sensors you will want to change the `SECRET_SENSOR`
value to another ID and you will want to change the hostname as well.

import requests

url = "http://<your_esp8266_ip_address>/get-sensor"

# Specify the desired sensor values in the payload
payload = {
    "sensor1": "1",  # Include this parameter to request sensor 1 value
    "sensor2": "1"   # Include this parameter to request sensor 2 value
}

response = requests.post(url, data=payload)

if response.status_code == 200:
    print(f"Sensor Values:\n{response.text}")
else:
    print("Failed to fetch sensor values.")

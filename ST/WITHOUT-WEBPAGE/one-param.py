import requests

url = "http://<your_esp8266_ip_address>/toggle"
params = {'command': 'on'}  # Change to 'off' to turn off the LED
response = requests.post(url, params=params)

if response.status_code == 200:
    print("LED toggled successfully!")
else:
    print("Failed to toggle LED.")

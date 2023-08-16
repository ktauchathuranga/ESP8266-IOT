import requests

url = "http://<your_esp8266_ip_address>/toggle"
response = requests.post(url)

if response.status_code == 200:
    print("LED toggled successfully!")
else:
    print("Failed to toggle LED.")

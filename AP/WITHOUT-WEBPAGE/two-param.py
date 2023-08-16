import requests

url = "http://<your_esp8266_ip_address>/control"
params = {'command': 'on', 'brightness': '150'}  # Change values as needed
response = requests.post(url, params=params)

if response.status_code == 200:
    print("LED control successful!")
else:
    print("Failed to control LED.")

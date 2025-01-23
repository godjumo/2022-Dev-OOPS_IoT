import requests
import json
import yaml
import os

config_path = os.path.dirname(os.path.abspath(__file__)) + "/config.yaml"

with open(config_path) as f:
    config = yaml.load(f, Loader=yaml.FullLoader)

url = config['esp32_url']

def request_commend(be_data : str) :

    if (be_data == "activate") :
        message = {"commend": "activate"}
        requests.post(url, data=message)
        for i in range(10) :
            print(f"activate {i}");
    elif (be_data == "deactivate") :
        message = {"commend": "deactivate"}
        requests.post(url, data=message)
        for j in range(10) :
            print(f"activate {j}");



# import requests
# import json

# # Commend init
# commend = ""

# url = "***"

# def request_commend(be_data : str) :

#     def commend_activate(commend: dict):
#         # Request
#         response = requests.post(url, data=commend)
        
#         # Response
#         print(response.text)

#     # Input Commend
#     while commend != "exit":
#         commend = input("commend >> ")
#         if (commend == "activate"):
#             message = {"commend": "activate"}
#             commend_activate(message)
#             continue
#         elif (commend == "deactivate"):
#             message = {"commend": "deactivate"}
#             commend_activate(message)
#             continue
#         else:
#             continue

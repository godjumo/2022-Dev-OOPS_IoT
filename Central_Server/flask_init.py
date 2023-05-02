from flask import Flask, request
import ESP32_request
import yaml
import os

config_path = os.path.dirname(os.path.abspath(__file__)) + "/config.yaml"

with open(config_path) as f:
    config = yaml.load(f, Loader=yaml.FullLoader)

host = config["flask_config"]["host"]
port = config["flask_config"]["port"]

web_gui = Flask(__name__)

@web_gui.route('/')
def main_page():
    return "hello, flask"
    

@web_gui.route('/data', methods=['POST'])
def receive_data():

    data = request.get_json()

    for i in data :
        if i['wallAlertActivation'] and not i['wallAlertDeactivation'] :
            ESP32_request.request_commend('activate')
            print("activate")
        elif not i['wallAlertActivation'] and i['wallAlertDeactivation'] :
            ESP32_request.request_commend('deactivate')
            print("deactivate")

    return 'OK'

if __name__ == "__main__":
    print("시작합니다.")
    web_gui.run(host=host, debug=True, port=port)
    
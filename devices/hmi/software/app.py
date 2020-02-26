"""
This is the starting point of the flask web application. It defines the routes that available to the HMI.
"""

from flask import Flask, jsonify, render_template, redirect, json, request

from plcconnectors.plc import Plc
from plcconnectors.modbusTCP.connector import ModbusTCPPlcConnector

# The flask application instance.
app = Flask(__name__)

plc = {}


@app.errorhandler(404)
def page_not_found(e):
    """
    # The default error handler route for when a site is called that has no route provided for.
    :param e: The error message
    :return: The text that will be displayed in the browser on an error.
    """
    return jsonify(error=404, text=str(e)), 404


@app.route('/orders/<count>', methods=['GET'])
def set_order(count):
    """
    Initiate an order.
    :param count: The amount of times the process is supposed to be executed.
    :return: Empty HTTP 200 response
    """
    plc.set_order(int(count))
    return "", 200


@app.route('/orders', methods=['GET'])
def get_order():
    """

    :return:
    """
    return json.dumps(plc.get_orders())


@app.route('/set-motor/<motor>/<motor_state>', methods=['GET'])
def set_motor_manual(motor, motor_state):
    """
    Turn a motor, defined by <motor>, on or off, according to <motor_state>.
    :param motor_state: On (1) or off (0).
    :param motor: The string that identifies one of the four motors on the process.
    :return: Empty HTTP 200 response
    """
    plc.set_motor(motor, motor_state)
    return "", 200


@app.route('/process/values', methods=['GET'])
def get_values():
    """
    Query the current state of the motor controls and the sensors.
    :return: A dictionary of the state of the motor controls and the state of the sensors.
    """
    return jsonify(plc.get_process_values())


@app.route('/application/state', methods=['GET'])
def get_application_state():
    """
    Get the current application state.
    :return: A string containing the application state
    """
    return json.dumps(plc.get_application_state())


@app.route("/process/state", methods=['GET'])
def get_process_state():
    """
    Retrieve the current state of the running process.
    :return: A dictionary representation of the ProcessState
    """
    return json.dumps(plc.get_process_state())


@app.route('/application/state/<new_state>', methods=['GET'])
def set_application_state(new_state):
    """

    :param new_state: String of the desired state.
    :return: The current application state.
    """
    plc.set_state(new_state)
    return json.dumps(plc.get_application_state())


@app.route('/application/reset', methods=['GET'])
def set_reset():
    """
    Initiate a reset to get the PLC out of the emergency stop state.
    :return: The current application state.
    """
    plc.set_reset()
    return json.dumps(plc.get_application_state())


@app.route('/order')
def order():
    """
    Here, process execution can be ordered.
    :return: The order.html view.
    """
    application_state = plc.get_application_state()
    process_state = plc.get_process_state()
    return render_template('order.html', process_state=process_state, application_state=application_state, order=True)


@app.route('/manual')
def manual():
    """
    Manual view. Here, manual controls can be triggered.
    :return: The manual.html view.
    """
    application_state = plc.get_application_state()
    process_state = plc.get_process_state()
    return render_template('manual.html', process_state=process_state, application_state=application_state, manual=True)


@app.route('/view')
def view():
    """
    Default view. Monitoring the process.
    :return: The view.html view
    """
    application_state = plc.get_application_state()
    process_state = plc.get_process_state()
    return render_template('view.html', process_state=process_state, application_state=application_state, view=True)


@app.route('/')
def index():
    """
    Default view.
    :return: Redirect to the default view.
    """
    return redirect('/view')


@app.before_request
def is_plc_connected():
    if not plc.is_connected():
        plc.plc_connector.modbus_client.connect()
        application_state = plc.get_application_state()
        return render_template('base.html', application_state=application_state, endpoint="/" + request.endpoint, disconnected=True)


# Starting point.
if __name__ == '__main__':
    while True:
        plc = Plc(ModbusTCPPlcConnector, '192.168.0.30', timeout=1)
        if plc.is_connected():
            break

    # Run the webserver.
    app.run(host="0.0.0.0", port=8080)

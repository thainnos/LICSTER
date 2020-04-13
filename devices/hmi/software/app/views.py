import functools

from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for, jsonify, json
)
from werkzeug.security import check_password_hash, generate_password_hash

from app.db import get_db

from plcconnectors.plc import Plc
from plcconnectors.modbusTCP.connector import ModbusTCPPlcConnector

bp = Blueprint('views', __name__, template_folder='templates', static_folder='static')

plc = Plc(ModbusTCPPlcConnector, '192.168.0.30', timeout=1)

@bp.before_app_request
def is_plc_connected():
    if request.endpoint in ["views.login", "views.reset_password", "views.add_user", "views.admin", "views.logout"]:
        pass
    else:
        if request.endpoint and request.endpoint != "static" and not plc.is_connected():
            endpoint = "/" + request.endpoint if request.endpoint in ["view", "manual", "order"] else "/view"
            plc.plc_connector.modbus_client.connect()
            application_state = plc.get_application_state()
            return render_template('base.html', application_state=application_state, endpoint=endpoint, disconnected=True)


@bp.before_app_request
def load_logged_in_user():
    """
    Checks if client was logged in with every request
    """
    user_id = session.get('user_id')

    if user_id is None:
        g.user = None
    else:
        g.user = get_db().execute(
            'SELECT * FROM user WHERE id = ?', (user_id,)
        ).fetchone()

def login_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None:
            return redirect(url_for('views.login'))

        return view(**kwargs)

    return wrapped_view


@bp.route('/orders/<count>', methods=['GET'])
def set_order(count):
    """
    Initiate an order.
    :param count: The amount of times the process is supposed to be executed.
    :return: Empty HTTP 200 response
    """
    plc.set_order(int(count))
    return "", 200


@bp.route('/orders', methods=['GET'])
def get_order():
    """

    :return:
    """
    return json.dumps(plc.get_orders())


@bp.route('/set-motor/<motor>/<motor_state>', methods=['GET'])
def set_motor_manual(motor, motor_state):
    """
    Turn a motor, defined by <motor>, on or off, according to <motor_state>.
    :param motor_state: On (1) or off (0).
    :param motor: The string that identifies one of the four motors on the process.
    :return: Empty HTTP 200 response
    """
    plc.set_motor(motor, motor_state)
    return "", 200


@bp.route('/process/values', methods=['GET'])
def get_values():
    """
    Query the current state of the motor controls and the sensors.
    :return: A dictionary of the state of the motor controls and the state of the sensors.
    """
    return jsonify(plc.get_process_values())


@bp.route('/application/state', methods=['GET'])
def get_application_state():
    """
    Get the current application state.
    :return: A string containing the application state
    """
    return json.dumps(plc.get_application_state())


@bp.route("/process/state", methods=['GET'])
def get_process_state():
    """
    Retrieve the current state of the running process.
    :return: A dictionary representation of the ProcessState
    """
    return json.dumps(plc.get_process_state())


@bp.route('/application/state/<new_state>', methods=['GET'])
def set_application_state(new_state):
    """

    :param new_state: String of the desired state.
    :return: The current application state.
    """
    plc.set_state(new_state)
    return json.dumps(plc.get_application_state())


@bp.route('/application/reset', methods=['GET'])
def set_reset():
    """
    Initiate a reset to get the PLC out of the emergency stop state.
    :return: The current application state.
    """
    plc.set_reset()
    return json.dumps(plc.get_application_state())


@bp.route('/order')
def order():
    """
    Here, process execution can be ordered.
    :return: The order.html view.
    """
    application_state = plc.get_application_state()
    process_state = plc.get_process_state()
    return render_template('order.html', process_state=process_state, application_state=application_state, order=True)


@bp.route('/manual')
def manual():
    """
    Manual view. Here, manual controls can be triggered.
    :return: The manual.html view.
    """
    application_state = plc.get_application_state()
    process_state = plc.get_process_state()
    return render_template('manual.html', process_state=process_state, application_state=application_state, manual=True)


@bp.route('/view')
def view():
    """
    Default view. Monitoring the process.
    :return: The view.html view
    """
    application_state = plc.get_application_state()
    process_state = plc.get_process_state()
    return render_template('view.html', process_state=process_state, application_state=application_state, view=True)


@bp.route('/')
def index():
    """
    Default view.
    :return: Redirect to the default view.
    """
    return redirect(url_for('views.view'))


@bp.route('/login', methods=['GET', 'POST'])
def login():
    """
    Login view.
    :return: The login.html view
    :return after form validation: the index.html
    """
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        db = get_db()
        error = None
        user = db.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()

        if user is None:
            error = 'Incorrect username.'
        elif not check_password_hash(user['password'], password):
            error = 'Incorrect password.'

        if error is None:
            session.clear()
            session['user_id'] = user['id']
            return redirect(url_for('views.index'))

        flash(error)
    return render_template('login.html')


@bp.route('/logout')
@login_required
def logout():
    session.clear()
    return redirect(url_for('views.index'))


@bp.route('/reset_password')
@login_required
def reset_password():
    """
    Password reset view.
    :return: The password_reset.html view
    Note: Has no functionality yet
    """
    return render_template('reset_password.html')


@bp.route('/admin')
@login_required
def admin():
    """
    Admin view.
    :return: The admin.html view
    Note: Has no functionality yet
    """
    return render_template('admin.html')


# add role check
@bp.route('/admin/add_user', methods=('GET', 'POST'))
@login_required
def add_user():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        db = get_db()
        error = None

        if not username:
            error = 'Username is required.'
        elif not password:
            error = 'Password is required.'
        elif db.execute(
            'SELECT id FROM user WHERE username = ?', (username,)
        ).fetchone() is not None:
            error = 'User {} is already registered.'.format(username)

        if error is None:
            db.execute(
                'INSERT INTO user (username, password) VALUES (?, ?)',
                (username, generate_password_hash(password))
            )
            db.commit()
            return redirect(url_for('views.login'))

        flash(error)

    return render_template('add_user.html')
        
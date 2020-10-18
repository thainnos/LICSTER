import functools
from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for
)
from werkzeug.security import check_password_hash, generate_password_hash
from app.db import get_db
from app.forms import LoginForm, SetPasswordForm, HmiLoginForm

auth = Blueprint(
    'auths',
    __name__,
    template_folder='templates/auths',
    static_folder='static')


def login_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None:
            return redirect(url_for('auths.login'))

        return view(**kwargs)

    return wrapped_view


def logout_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user:
            return redirect(url_for('auths.logout'))

        return view(**kwargs)

    return wrapped_view


@auth.before_app_request
def load_logged_in_user():
    ''' Checks if client was logged in with every request
    '''
    user_id = session.get('user_id')

    if user_id is None:
        g.user = None
    else:
        g.user = get_db().execute(
            'SELECT * FROM user WHERE id = ?', (user_id,)
        ).fetchone()


def save_ip_address_if_not_saved(userId):
    db = get_db()
    ipaddr = db.execute(
        'SELECT * FROM ipaddr WHERE userid = ?', (userId,)
    ).fetchall()
    ipSet = False
    if ipaddr:
        # The user already has at least one IP Adress in the database
        for ip in ipaddr:
            if ip['ipaddress'] == request.remote_addr:
                ipSet = True
                if ipSet:
                    break

    if not ipSet:
        # The current Ip adress is not in the database
        db.execute(
            'INSERT INTO ipaddr (userid, ipaddress) VALUES (?, ?)',
            (session['user_id'], request.remote_addr)
        )
        db.commit()


@auth.route('/login', methods=['GET', 'POST'])
@logout_required
# flake8: noqa: C901
def login():
    isHmi = False
    if request.remote_addr == "127.0.0.1":
        isHmi = True
    # Formen generieren
    if isHmi:
        form = HmiLoginForm()
    else:
        form = LoginForm()    

    # Form validieren
    if form.validate_on_submit():
        # get username, password from form
        # get user with db query
        # check if user exists in db and password hash matches
        if isHmi:
            username = 'hmilocal'
        else:
            username = form.username.data
        password = form.password.data
        db = get_db()
        user = db.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()

        error = None
        if user is None:
            error = 'Incorrect username.'
        elif not check_password_hash(user['password'], password):
            error = 'Incorrect password.'

        if error is None:
            # User exists in db and password hash matches
            # User ID und Rolle der Session hinzufügen
            session.clear()
            session['user_id'] = user['id']
            session['user_role'] = user['user_role']

            save_ip_address_if_not_saved(user['id'])

            if isHmi:
                return redirect(url_for('views.index'))
            # Nur für nicht HMI User:
            # Falls nötig zum Passwort setzen weiterleiten
            if user['first_login'] == 1:
                return redirect(url_for('auths.set_password'))
            # Falls Admin zum Dashboard weiterleiten
            if session['user_role'] == 'admin':
                return redirect(url_for('admins.dashboard'))          
            return redirect(url_for('views.index'))

    # Template rendern
    if isHmi:
        return render_template('loginhmi.html', form=form)
    else:
        return render_template('login.html', form=form)

@auth.route('/remote_test_login', methods=['GET', 'POST'])
@logout_required
# flake8: noqa: C901
def remote_test_login():
    isHmi = False
    form = LoginForm()    

    # Form validieren
    if form.validate_on_submit():
        # get username, password from form
        # get user with db query
        # check if user exists in db and password hash matches
        if isHmi:
            username = 'hmilocal'
        else:
            username = form.username.data
        password = form.password.data
        db = get_db()
        user = db.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()

        error = None
        if user is None:
            error = 'Incorrect username.'
        elif not check_password_hash(user['password'], password):
            error = 'Incorrect password.'

        if error is None:
            # User exists in db and password hash matches
            # User ID und Rolle der Session hinzufügen
            session.clear()
            session['user_id'] = user['id']
            session['user_role'] = user['user_role']

            save_ip_address_if_not_saved(user['id'])

            if isHmi:
                return redirect(url_for('views.index'))
            # Nur für nicht HMI User:
            # Falls nötig zum Passwort setzen weiterleiten
            if user['first_login'] == 1:
                return redirect(url_for('auths.set_password'))
            # Falls Admin zum Dashboard weiterleiten
            if session['user_role'] == 'admin':
                return redirect(url_for('admins.dashboard'))          
            return redirect(url_for('views.index'))

    # Template rendern
    if isHmi:
        return render_template('loginhmi.html', form=form)
    else:
        return render_template('login.html', form=form)


@auth.route('/logout')
@login_required
def logout():
    ''' Logout view.
    :return: Redirect to index.html
    :rtype: Redirect
    '''
    session.clear()
    return redirect(url_for('views.index'))


@auth.route('/set_password', methods=('GET', 'POST'))
@login_required
def set_password():
    ''' View for setting the password on the first login.
    :return: set_password.html or redirect to views.index
    :rtype: HTML / Redirect
    '''
    form = SetPasswordForm()
    if form.validate_on_submit():
        password = form.password.data
        error = None
        if not password:
            error = "A password is required"
        if error is None:
            db = get_db()
            user_id = int(session['user_id'])
            db.execute(
                'UPDATE user SET password = ? , first_login = ? WHERE id = ?',
                (generate_password_hash(password), 0, user_id)
            )
            db.commit()
            return redirect(url_for('views.index'))
        flash(error)

    return render_template('set_password.html', form=form)

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


@auth.route('/login', methods=['GET', 'POST'])
@logout_required
def login():
    ''' Login view. Returns a view for the local HMI
    and another for any remote connection.

    :return:
    Before validation: login.html
    After validation: index.html or admin dashboard
    :rtype: HTML / redirect
    '''
    form = LoginForm()
    hmiForm = HmiLoginForm()
    if hmiForm.validate_on_submit() and request.remote_addr == "127.0.0.1":
        db = get_db()
        username = "hmilocal"
        password = hmiForm.password.data
        user = db.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()
        if check_password_hash(user['password'], password):
            session.clear()
            session['user_id'] = user['id']
            ipaddr = db.execute(
                'SELECT * FROM ipaddr WHERE userid = ?', (session['user_id'],)
            ).fetchall()
            if ipaddr:
                # The user already has an Ip Adress in the database
                pass
            else:
                # The current Ip adress is not in the database
                db.execute(
                    'INSERT INTO ipaddr (userid, ipaddress) VALUES (?, ?)',
                    (session['user_id'], request.remote_addr)
                )
                db.commit()
            session['user_role'] = user['user_role']
            return redirect(url_for('views.index'))
    if form.validate_on_submit():
        username = form.username.data
        password = form.password.data
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
            ipaddr = db.execute(
                'SELECT * FROM ipaddr WHERE userid = ?', (session['user_id'],)
            ).fetchall()
            ipnotset = True
            if ipaddr:
                # The user already has an Ip Adress in the database
                for ip in ipaddr:
                    if ip['ipaddress'] == request.remote_addr:
                        ipnotset = False
            if ipnotset:
                # The current Ip adress is not in the database
                db.execute(
                    'INSERT INTO ipaddr (userid, ipaddress) VALUES (?, ?)',
                    (session['user_id'], request.remote_addr)
                )
                db.commit()

            session['user_role'] = user['user_role']
            if user['first_login'] == 1:
                return redirect(url_for('auths.set_password'))
            if session['user_role'] == 'admin':
                return redirect(url_for('admins.dashboard'))
            return redirect(url_for('views.index'))

        flash(error)

    if request.remote_addr == "127.0.0.1":
        return render_template('loginhmi.html', form=hmiForm)
    else:
        return render_template('login.html', form=form)

# DELETE ME!!!
# Seriously, don't use me!!


@auth.route('/loginnonlocal', methods=['GET', 'POST'])
@logout_required
def login_local():
    ''' Remote login view. Returns the remote view on the local machine.
    :return:
    Before validation the login.html view,
    after validation the index.html/admin dashboard
    :rtype: HTML / redirect
    '''
    form = LoginForm()
    if form.validate_on_submit():
        username = form.username.data
        password = form.password.data
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
            ipaddr = db.execute(
                'SELECT * FROM ipaddr WHERE userid = ?', (session['user_id'],)
            ).fetchall()
            ipnotset = True
            if ipaddr:
                # The user already has an Ip Adress in the database
                for ip in ipaddr:
                    if ip['ipaddress'] == request.remote_addr:
                        ipnotset = False
            if ipnotset:
                # The current Ip adress is not in the database
                db.execute(
                    'INSERT INTO ipaddr (userid, ipaddress) VALUES (?, ?)',
                    (session['user_id'], request.remote_addr)
                )
                db.commit()

            if user['first_login'] == 1:
                return redirect(url_for('auths.set_password'))
            session['user_role'] = user['user_role']
            if session['user_role'] == 'admin':
                return redirect(url_for('admins.dashboard'))
            return redirect(url_for('views.index'))

        flash(error)

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

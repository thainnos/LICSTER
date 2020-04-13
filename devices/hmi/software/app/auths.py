import functools

from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for, jsonify, json
)
from werkzeug.security import check_password_hash, generate_password_hash

from app.db import get_db

auth = Blueprint('auths', __name__, template_folder='templates', static_folder='static')

def login_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None:
            return redirect(url_for('auths.login'))

        return view(**kwargs)

    return wrapped_view

@auth.route('/login', methods=['GET', 'POST'])
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


@auth.route('/logout')
@login_required
def logout():
    session.clear()
    return redirect(url_for('views.index'))


@auth.route('/reset_password')
@login_required
def reset_password():
    """
    Password reset view.
    :return: The password_reset.html view
    Note: Has no functionality yet
    """
    return render_template('reset_password.html')


@auth.route('/admin')
@login_required
def admin():
    """
    Admin view.
    :return: The admin.html view
    Note: Has no functionality yet
    """
    return render_template('admin.html')


@auth.route('/admin/add_user', methods=('GET', 'POST'))
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
            return redirect(url_for('auths.login'))

        flash(error)

    return render_template('add_user.html')
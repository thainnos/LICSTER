import functools

from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for, jsonify, json
)
from werkzeug.security import check_password_hash, generate_password_hash

from app.db import get_db

admin = Blueprint('admins', __name__, template_folder='templates/admins', static_folder='static')

"""
 This file handles user authentification.
 Methods
    admin_required: Decorator to check if user is logged in and admin
 Routes
    dashboard: Shows all users and allows to delete users
    dashboard/add_user: Inserts a new user/admin in the database
    dashboard/delete_user: Deletes a user/admin
    dashboard/reset_password: Will get moved to auth blueprint.
"""

def admin_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None or g.user['user_role'] != 'admin':
            return redirect(url_for('views.view'))

        return view(**kwargs)

    return wrapped_view

@admin.route('/dashboard', methods=('GET', 'POST'))
#@admin_required
def dashboard():
    """
    Dashboard. Shows all users and allows to delete users.
    Will get more functionality soon.
    :return: The admin.html view
    """
    db = get_db()
    if request.method == 'POST':
        selected_users = request.form.getlist("users")
        for user in selected_users:
            db.execute(
                'DELETE FROM user WHERE id = ?', (user,)
            )
        db.commit()
        return redirect(url_for('admins.dashboard'))
    rows = db.execute('SELECT * FROM user').fetchall()
    return render_template('admin.html', rows=rows)


@admin.route('/dashboard/add', methods=('GET', 'POST'))
#@admin_required
def add_user():
    """
    Add user or admin. Allows the admin to create a new user or admin
    :return: The add_user.html view
    """
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        role = request.form.get('role')
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
                'INSERT INTO user (username, password, user_role, first_login) VALUES (?, ?, ?, ?)',
                (username, generate_password_hash(password), role, 1)
            )
            db.commit()
            return redirect(url_for('admins.dashboard'))

        flash(error)

    return redirect(url_for('admins.dashboard'))

@admin.route('/dashboard/delete_user', methods=('GET', 'POST'))
@admin_required
def delete_user():
    """
    Delete user. Allows the admin to delete a user
    :return: The delete_user.html view
    """
    if request.method == 'POST':
        username = request.form['username']
        db = get_db()
        error = None

        if not username:
            error = "Username is required."
        elif db.execute(
            'SELECT id FROM user WHERE username = ?', (username,)
        ).fetchone() is None:
            error = "The user {} doesn't exist.".format(username)
        
        if error is None:
            db.execute(
                'DELETE FROM user WHERE username = ?', (username,)
            )
            db.commit()
            return redirect(url_for('admins.dashboard'))
    return render_template('delete_user.html')


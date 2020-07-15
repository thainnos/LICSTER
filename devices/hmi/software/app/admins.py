import functools
from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for, jsonify, json
)
from werkzeug.security import check_password_hash, generate_password_hash
from app.db import get_db
from app.forms import AddUserForm, DeleteUserForm, ResetLogForm

admin = Blueprint(
    'admins',
    __name__,
    template_folder='templates/admins',
    static_folder='static')


def admin_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None or g.user['user_role'] != 'admin':
            return redirect(url_for('views.view'))

        return view(**kwargs)

    return wrapped_view


@admin.route('/dashboard', methods=('GET', 'POST'))
@admin_required
def dashboard():
    ''' Admin Dashboard. Shows all users and snort logs. Allows adding/deleting users and resetting the logs.

    :return: The admin.html view
    :rtype: HTML
    '''
    add_form = AddUserForm()
    delete_form = DeleteUserForm()
    reset_form = ResetLogForm()
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
    clients = []
    for row in rows:
        ipadresses = db.execute(
            'SELECT ipaddress FROM ipaddr WHERE userid = ?', (row['id'],)
        ).fetchall()
        client = {}
        client['username'] = row['username']
        client['user_role'] = row['user_role']
        client['ipadresses'] = ipadresses
        if row['email'] is not None:
            client['email'] = row['email']
        clients.append(client)

    snort_rows = db.execute('SELECT * FROM snort').fetchall()
    snort_outer_row = []
    for snort_row in snort_rows:
        snort_inner_dict = {}
        snort_inner_dict['type'] = snort_row[1]
        snort_inner_dict['classification'] = snort_row[2]
        snort_inner_dict['priority'] = snort_row[3]
        snort_inner_dict['datetime'] = snort_row[4]
        snort_outer_row.append(snort_inner_dict)

    return render_template('admin.html', clients=clients, logs=snort_outer_row,
                           add_form=add_form, delete_form=delete_form, reset_form=reset_form)


@admin.route('/dashboard/add', methods=('GET', 'POST'))
@admin_required
def add_user():
    ''' Add a user or an admin.

    :return: Redirect to the admin dashboard method
    :rtype: redirect
    '''
    form = AddUserForm()
    if form.validate_on_submit():
        username = form.username.data
        password = form.password.data
        role = form.role.data
        email = form.email.data
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
        elif email and (role != 'admin'):
            error = 'Only admins can have an email adress.'
        if error is None:
            if not email:
                db.execute(
                    'INSERT INTO user (username, password, user_role, first_login) VALUES (?, ?, ?, ?)',
                    (username, generate_password_hash(password), role, 1)
                )
                db.commit()
            else:
                db.execute(
                    'INSERT INTO user (username, password, user_role, first_login, email) VALUES (?, ?, ?, ?, ?)',
                    (username, generate_password_hash(password), role, 1, email)
                )
                db.commit()
            return redirect(url_for('admins.dashboard'))

        flash(error)

    return redirect(url_for('admins.dashboard'))


@admin.route('/dashboard/delete', methods=('GET', 'POST'))
@admin_required
def delete_user():
    ''' Delete a user or an admin.

    :return: Redirect to the admin dashboard method
    :rtype: redirect
    '''
    form = DeleteUserForm()
    if form.validate_on_submit():
        username = form.username.data
        db = get_db()
        error = None

        if not username:
            error = "Username is required."
        elif db.execute(
            'SELECT id FROM user WHERE username = ?', (username,)
        ).fetchone() is None:
            error = "The user {} doesn't exist.".format(username)

        if error is None:
            # All ip addresses connected to the user need to get deleted first
            userid = db.execute(
                'SELECT id FROM user WHERE username = ?', (username,)
            ).fetchone()
            db.execute(
                'DELETE FROM ipaddr WHERE userid = ?', (userid['id'],)
            )
            db.execute(
                'DELETE FROM user WHERE username = ?', (username,)
            )
            db.commit()
            return redirect(url_for('admins.dashboard'))
        flash(error)

    return redirect(url_for('admins.dashboard'))


@admin.route('/dashboard/reset_logs', methods=('GET', 'POST'))
@admin_required
def reset_logs():
    ''' Reset the Snort log table.

    :return: Redirect to the admin dashboard method
    :rtype: redirect
    '''
    form = ResetLogForm()
    if form.validate_on_submit():
        db = get_db()
        db.execute('DROP TABLE IF EXISTS snort')
        db.commit()
        db.execute("CREATE TABLE IF NOT EXISTS snort(id INTEGER PRIMARY KEY AUTOINCREMENT, snort_type TEXT NOT NULL, snort_classification TEXT NOT NULL, snort_priority INTEGER NOT NULL, snort_datetime TEXT NOT NULL)")
        db.commit()
        return redirect(url_for('admins.dashboard'))
    return redirect(url_for('admins.dashboard'))

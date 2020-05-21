import sqlite3

import click
from flask import current_app, g
from flask.cli import with_appcontext
from werkzeug.security import generate_password_hash

def get_db():
    if 'db' not in g:
        g.db = sqlite3.connect(
            current_app.config['DATABASE'],
            detect_types=sqlite3.PARSE_DECLTYPES
        )
        g.db.row_factory = sqlite3.Row

    return g.db


def close_db(e=None):
    db = g.pop('db', None)

    if db is not None:
        db.close()


def init_db():
    db = get_db()
    with current_app.open_resource('schema.sql') as f:
        db.executescript(f.read().decode('utf8'))

def check_pw(pw1, valueType):
    print("Please repeat the " + valueType + ".")
    pw2 = input(">>> ")
    if pw1 == pw2:
        return True
    else:
        return False

@click.command('init-db')
@with_appcontext
def init_db_command():
    """Clear the existing data, create new tables, add the first admin and a user for the hmi."""
    init_db()
    click.echo('Initialized the database.')
    db = get_db()
    click.echo("Please enter the name for the admin user:")
    admin_name = input(">>> ")
    click.echo(admin_name)

    click.echo("Please enter the password for the admin user:")
    while True:
        admin_password = input(">>> ")
        breakNow = check_pw(admin_password, "password of the admin user")
        if breakNow:
            click.echo("The password was set.")
            break
        else:
            click.echo("The passwords don't match. Please try again.")

    click.echo("Please enter the password for the hmi. This password must only contain numbers.")
    while True:
        hmi_password = input(">>> ")
        try:
            isint = int(hmi_password)
            breakNow = check_pw(hmi_password, "password of the hmi")
            if breakNow:
                click.echo("The password was set.")
                break
            else:
                click.echo("The passwords don't match. Please try again.")
        except:
            click.echo("This password must only contain numbers. Please type in a password containing only numbers.")
    
    db.execute('INSERT INTO user (username, password, user_role, first_login) VALUES (?, ?, ?, ?)', 
    (admin_name, generate_password_hash(admin_password), 'admin', 1))
    db.commit()
    db.execute('INSERT INTO user (username, password, user_role, first_login) VALUES (?, ?, ?, ?)', 
    ('hmilocal', generate_password_hash(hmi_password), 'user', 1))
    db.commit()
    click.echo("A new admin - " + admin_name + " - was created.")
    click.echo("The password for the hmi is set.")

@click.command('change-hmi-password')
@with_appcontext
def change_hmi_password_command():
    """Change the password for the hmi."""
    click.echo("Please enter the new password for the hmi. This password must only contain numbers.")
    while True:
        hmi_password = input(">>> ")
        try:
            isint = int(hmi_password)
            breakNow = check_pw(hmi_password, "password of the hmi")
            if breakNow:
                click.echo("The password was set.")
                break
            else:
                click.echo("The passwords don't match. Please try again.")
        except:
            click.echo("This password must only contain numbers. Please type in a password containing only numbers.")
    db = get_db()
    db.execute('UPDATE user SET password = ? WHERE username = ?', (generate_password_hash(hmi_password), "hmilocal"))
    db.commit()

def init_app(app):
    app.teardown_appcontext(close_db)
    app.cli.add_command(init_db_command)
    app.cli.add_command(change_hmi_password_command)
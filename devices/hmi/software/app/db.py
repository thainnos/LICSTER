import sqlite3
import click
from flask import current_app, g
from flask.cli import with_appcontext
from werkzeug.security import generate_password_hash


def get_db():
    ''' Return a database connection.
    '''
    if 'db' not in g:
        g.db = sqlite3.connect(
            current_app.config['DATABASE'],
            detect_types=sqlite3.PARSE_DECLTYPES
        )
        g.db.row_factory = sqlite3.Row

    return g.db


def close_db(e=None):
    ''' Close the database connection.
    '''
    db = g.pop('db', None)

    if db is not None:
        db.close()


def init_db():
    ''' (Re-)Make the database table.
    Caution: Using this method will delete everything in the database.
    '''
    db = get_db()
    with current_app.open_resource('schema.sql') as f:
        db.executescript(f.read().decode('utf8'))


def check_pw(pw1, valueType):
    ''' Are pw1 and an input the same?

    :param pw1: Password, defaults to empty String
    :type pw1: String
    :param valueType: String containing the type that is getting checked,
    defaults to empty String
    :type valueType: String
    :return: Are pw1 and the input the same?
    :rtype: boolean
    '''
    print("Please repeat the " + valueType + ".")
    pw2 = input(">>> ")  # nosec
    if pw1 == pw2:
        return True
    else:
        return False


def add_email():
    ''' Prompts for an Email Address. Can be stopped by typing no.

    :return: Email Address
    :rtype: String
    '''
    click.echo('Do you want to add an email address for this user?\n'
               'If the IDS detects anything, it will be sent to all '
               'saved email addresses.')
    not_skipping = True
    admin_email = ""
    while not_skipping:
        click.echo(
            'If you dont want to add an email address you can simply '
            'type "no".')
        email = input(">>> ")  # nosec
        if email == 'no':
            break
        else:
            email2 = input(  # nosec
                'Please enter the email address again or skip this'  # nosec
                ' step by typing "no".\n>>> ')  # nosec
            if email2 == 'no' or email2 == 'No':
                not_skipping = False
                break
            elif email != email2:
                click.echo("The email addresses don't match.")
            else:
                admin_email = email
                not_skipping = False
                break
    return admin_email


def add_admin_password():
    ''' Prompts for admin password.

    :return: The admin Password.
    :rtype: String
    '''
    click.echo("Please enter the password for the admin user:")
    while True:
        admin_password = input(">>> ")  # nosec
        breakNow = check_pw(admin_password, "password of the admin user")
        if breakNow:
            click.echo("The password was set.")
            break
        else:
            click.echo("The passwords don't match. Please try again.")
    return admin_password


def add_hmi_password():
    ''' Prompts for HMI password.

    :return: The HMI Password.
    :rtype: Integer
    '''
    click.echo(
        "Please enter the password for the hmi. "
        "This password must only contain numbers.")
    while True:
        hmi_password = input(">>> ")  # nosec
        try:
            hmi_password = str(int(hmi_password))
            breakNow = check_pw(hmi_password, "password of the hmi")
            if breakNow:
                click.echo("The password was set.")
                break
            else:
                click.echo("The passwords don't match. Please try again.")
        except BaseException:
            click.echo(
                "This password must only contain numbers. "
                "Please type in a password containing only numbers.")
    return hmi_password


@click.command('init-db')
@with_appcontext
def init_db_command():
    ''' Initalizes the database with admin and hmi user.'''
    init_db()
    click.echo('Initialized the database.')
    db = get_db()
    click.echo("Please enter the name for the admin user:")
    admin_name = input(">>> ")  # nosec
    click.echo(admin_name)

    admin_email = add_email()
    admin_password = add_admin_password()
    hmi_password = add_hmi_password()

    if not admin_email:
        db.execute('INSERT INTO user (username, password, user_role, '
                   'first_login) VALUES (?, ?, ?, ?)',
                   (admin_name, generate_password_hash(admin_password),
                    'admin', 1))
    else:
        db.execute('INSERT INTO user (username, password, user_role, '
                   'first_login, email) VALUES (?, ?, ?, ?, ?)',
                   (admin_name, generate_password_hash(admin_password),
                    'admin', 1, admin_email))
    db.commit()
    db.execute('INSERT INTO user (username, password, user_role, first_login)'
               ' VALUES (?, ?, ?, ?)',
               ('hmilocal', generate_password_hash(hmi_password), 'user', 1))
    db.commit()
    click.echo("A new admin - " + admin_name + " - was created.")
    click.echo("The password for the hmi is set.")


@click.command('change-hmi-password')
@with_appcontext
def change_hmi_password_command():
    ''' Change the password for the hmi '''
    hmi_password = add_hmi_password()
    db = get_db()
    db.execute('UPDATE user SET password = ? WHERE username = ?',
               (generate_password_hash(hmi_password), "hmilocal"))
    db.commit()


def init_app(app):
    app.teardown_appcontext(close_db)
    app.cli.add_command(init_db_command)
    app.cli.add_command(change_hmi_password_command)

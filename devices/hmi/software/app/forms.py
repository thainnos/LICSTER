from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, BooleanField, SubmitField
from wtforms import SelectField
from wtforms.validators import DataRequired


class LoginForm(FlaskForm):
    ''' Form for logging in. Remember me has no functionality yet.
    '''
    username = StringField("username", validators=[DataRequired()])
    password = PasswordField("password", validators=[DataRequired()])
    remember_me = BooleanField("Remember me")
    submit = SubmitField("Log in")


class HmiLoginForm(FlaskForm):
    ''' Form for logging in locally.
    '''
    password = PasswordField("password", validators=[DataRequired()])


class AddUserForm(FlaskForm):
    ''' Form to add users (username, password, role, email).
    '''
    username = StringField("username", validators=[DataRequired()])
    password = PasswordField("password", validators=[DataRequired()])
    role = SelectField(
        u'user role', choices=[
            ('user', 'user'), ('admin', 'admin')])
    email = StringField("email")
    submit = SubmitField("Add user")


class DeleteUserForm(FlaskForm):
    ''' Form to delete a user.
    '''
    username = StringField("username", validators=[DataRequired()])
    submit = SubmitField("Delete user")


class SetPasswordForm(FlaskForm):
    ''' Form to set a password.
    '''
    password = PasswordField("password", validators=[DataRequired()])
    submit = SubmitField("Set password")


class ResetLogForm(FlaskForm):
    ''' Form to reset the Logs.
    '''
    submit = SubmitField("Reset logs")

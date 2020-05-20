from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, BooleanField, SubmitField
from wtforms import SelectField
from wtforms.validators import Length, Required

class LoginForm(FlaskForm):
    username = StringField("username", validators=[Required()])
    password = PasswordField("password", validators=[Required()])
    remember_me = BooleanField("Remember me")
    submit = SubmitField("Log in")

class HmiLoginForm(FlaskForm):
    password = PasswordField("password", validators=[Required()])

class AddUserForm(FlaskForm):
    username = StringField("username", validators=[Required()])
    password = PasswordField("password", validators=[Required()])
    role = SelectField(u'user role', choices=[('user', 'user'), ('admin', 'admin')])
    submit = SubmitField("Add user")

class DeleteUserForm(FlaskForm):
    username = StringField("username", validators=[Required()])
    submit = SubmitField("Delete user")

class SetPasswordForm(FlaskForm):
    password = PasswordField("password", validators=[Required()])
    submit = SubmitField("Set password")

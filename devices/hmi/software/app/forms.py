from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, BooleanField, SubmitField
from wtforms.validators import Length, Required

class LoginForm(FlaskForm):
    username = StringField("username", validators=[Required()])
    password = PasswordField("password", validators=[Required()])
    remember_me = BooleanField("Remember me")
    submit = SubmitField("Log in")
"""
This is the starting point of the flask web application.
It defines the routes that available to the HMI.
"""
import os
from flask import Flask, jsonify


def page_not_found(e):
    """
    The default error handler route for when a non-existant site is called.
    :param e: The error message
    :return: The text that will be displayed in the browser on an error.
    """
    return jsonify(error=404, text=str(e)), 404


def create_app(test_config=None):
    app = Flask(__name__, instance_relative_config=True)
    app.register_error_handler(404, page_not_found)
    app.config.from_mapping(
        SECRET_KEY=os.getenv('SECRET_KEY'),
        DATABASE=os.path.join(app.instance_path, "app.sqlite"),
    )

    @app.after_request
    def prevent_embedding(response):
        response.headers["X-Frame-Options"] = "DENY"
        response.headers["X-Content-Type-Options"] = "nosniff"
        response.headers["Server"] = "gunicorn"
        return response

    if test_config is None:
        # load instance config if exists
        app.config.from_pyfile('config.py', silent=True)
    else:
        # load test config if passed in
        app.config.from_mapping(test_config)

    # ensure the instance folder exists
    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    from . import db
    db.init_app(app)

    from . import views
    app.register_blueprint(views.bp)

    from . import auths
    app.register_blueprint(auths.auth)

    from . import admins
    app.register_blueprint(admins.admin)

    return app

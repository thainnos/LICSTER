from app.db import get_db


def test_dashboard_access(client, auth):
    auth.login('test_admin')
    assert client.get('/dashboard').status_code == 200


def test_adding_user(client, auth, app):
    auth.login('test_admin')
    client.post(
        '/dashboard/add',
        data={'username': 'addtest', 'password': 'test', 'role': 'user'}
        )
    with app.app_context():
        assert get_db().execute(
            "select * from user where username = 'addtest'",
        ).fetchone() is not None


def test_deleting_user(client, auth, app):
    auth.login('test_admin')
    client.post(
        '/dashboard/delete',
        data={'username': 'test_user'}
        )
    with app.app_context():
        assert get_db().execute(
            "select * from user where username = 'addtest'",
        ).fetchone() is None

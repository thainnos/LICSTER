from flask import g, session


def test_login(client, auth):
    response = auth.login('test_user')
    assert response.headers['Location'] == 'http://localhost/'

    with client:
        client.get('/')
        assert session['user_id'] == 2
        assert g.user['username'] == 'test_user'


def test_hmi_login(client, auth):
    response = auth.hmi_login()
    assert response.headers['Location'] == 'http://localhost/'

    with client:
        client.get('/')
        assert session['user_id'] == 4
        assert g.user['username'] == 'hmilocal'


def test_logout(client, auth):
    auth.login('test_user')

    with client:
        auth.logout()
        assert 'user_id' not in session

    auth.hmi_login()

    with client:
        auth.logout()
        assert 'user_id' not in session


def test_redirect_first_login(client, auth):
    response = auth.login('new_test_user')
    assert response.headers['Location'] == 'http://localhost/set_password'


def test_unauthorized_dashboard_access(client, auth):
    response = auth.login('test_user')
    assert response.headers['Location'] == 'http://localhost/'

    with client:
        client.get('/')
        assert session['user_id'] == 2
        assert g.user['username'] == 'test_user'
        assert client.get('/dashboard').status_code != 200

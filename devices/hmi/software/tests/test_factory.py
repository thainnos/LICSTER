from app import create_app


def test_config():
    assert not create_app().testing
    assert create_app({'TESTING': True}).testing


def test_page_not_found(client):
    with client:
        response = client.get("/thispagedoesntexist")
        assert response.status_code == 404


def test_request_headers(client):
    with client:
        response = client.get("/")
        assert response.headers["X-Frame-Options"] == "DENY"
        assert response.headers["X-Content-Type-Options"] == "nosniff"
        assert response.headers["Server"] == "gunicorn"

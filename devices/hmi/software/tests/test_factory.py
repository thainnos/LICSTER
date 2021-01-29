from app import create_app


def test_config():
    assert not create_app().testing
    assert create_app({'TESTING': True}).testing


def test_page_not_found(client):
    with client:
        response = client.get("/thispagedoesntexist")
        assert response.status_code == 404

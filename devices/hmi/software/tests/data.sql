INSERT INTO user(id, username, password, user_role, first_login, email)
VALUES
    (1, 'test_admin', 'pbkdf2:sha256:150000$KHpcan69$6a76c5dc608cdf84677bf8794947f7cd5619f7d8c651c88852793ddfc7b4a0d8', 'admin', 0, 'test@test.com');

INSERT INTO user(id, username, password, user_role, first_login)
VALUES
    (2, 'test_user', 'pbkdf2:sha256:150000$KHpcan69$6a76c5dc608cdf84677bf8794947f7cd5619f7d8c651c88852793ddfc7b4a0d8', 'user', 0),
    (3, 'new_test_user', 'pbkdf2:sha256:150000$KHpcan69$6a76c5dc608cdf84677bf8794947f7cd5619f7d8c651c88852793ddfc7b4a0d8', 'user', 1),
    (4, 'hmilocal', 'pbkdf2:sha256:150000$DdZ9rW7i$4a8a55a6c8326f74c6f5122301b65b662786d30bafd7933296aabc3179bf3b99', 'user', 0);

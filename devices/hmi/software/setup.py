from setuptools import setup

setup(
    name="LICSTERhmi",
    version="0.2.0",
    author="Felix Sauer, Matthias Niedermaier",
    author_email="mattias.niedermaier@hs-augsburg.de",
    description=(
        "LICSTER Web HMI"),
    license="Linux",
    keywords="LICSTER, HMI",
    url="https://www.hs-augsburg.de",
    install_requires=[
        'flask',
        'pymodbus',
        'WTForms',
        'Flask-WTF',
        'gunicorn'

    ],
    entry_points='''
        [console_scripts]
        LICSTERhmi=wsgi:main
    ''',
    classifiers=[
        "Development Status :: 0.2.0 - Beta",
        "Topic :: Utilities",
        "License :: GPL GPLv3",
    ],
)

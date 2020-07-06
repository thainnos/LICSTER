"""
A helper class to reduce the active instances of all the states.
"""


class Singleton(type):
    """
    A static meta class that allows for singleton instances.
    """
    _instances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(
                Singleton, cls).__call__(
                *args, **kwargs)
        return cls._instances[cls]

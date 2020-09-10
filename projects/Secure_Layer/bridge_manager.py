#!/usr/bin/env python3
"""
This module contains the BridgeManager class.
"""
import os
import signal
from queue import Empty
from threading import Lock, Thread, current_thread
from time import sleep
from typing import List

from bridge import Bridge
from config import Config


class BridgeManager:
    """
    This class manages several Bridges.
    This includes starting, and restarting them on errors.
    """
    def __init__(self):
        self.bridges = []
        self.bridge_lock = Lock()
        self.check_bridge_status_thread = Thread(
            target=self._check_bridge_status)
        self.check_bridge_status_thread.start()

    def _check_bridge_status(self) -> None:
        def restart_bridge_on_error(bridge) -> None:
            try:
                message = bridge.cfg.q_manage_out.get_nowait()
                if message == 'Error':
                    cfg = bridge.cfg
                    name = cfg.name
                    print('Error in {0}, restarting...'.format(name))
                    bridge = Bridge(bridge.cfg)
                    print('Restarted {0}!'.format(name))
            except Empty:
                pass

        current_thread().name = 'Check-Bridge-Status-Thread'
        while True:
            self.bridge_lock.acquire()
            for bridge in self.bridges:
                restart_bridge_on_error(bridge)
            self.bridge_lock.release()
            sleep(0.5)

    def add_bridge(self, cfg: Config) -> None:
        """
        Adds a new bridge based on the settings specified in the Config *cfg*.
        """
        self.bridge_lock.acquire()
        self.bridges.append(Bridge(cfg))
        self.bridge_lock.release()

    def add_bridges(self, cfgs: List[Config]) -> None:
        """
        Adds new bridges based on the settings supplied in the list of Config
        objects in *cfgs*.
        """
        self.bridge_lock.acquire()
        for cfg in cfgs:
            self.bridges.append(Bridge(cfg))
        self.bridge_lock.release()


def _sigterm_handler(signum, frame):
    print('Signal handler called with signal', signum)
    pid = os.getpid()
    os.kill(-pid, signal.SIGKILL)


def _set_signals() -> None:
    signal.signal(signal.SIGTERM, _sigterm_handler)


def __main():
    """
    Main function. (This is to prevent linting complaints)
    """
    _set_signals()
    cfgs = Config.load_from_config('config.ini')
    manager = BridgeManager()
    manager.add_bridges(cfgs)


if __name__ == "__main__":
    __main()

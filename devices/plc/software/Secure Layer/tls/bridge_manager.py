
from queue import Empty
from threading import Lock, Thread, current_thread
from time import sleep

from bridge import Bridge
from context import Context

class BridgeManager:

    def __init__(self):
        self.bridges = []
        self.bridge_lock = Lock()
        self.check_bridge_status_thread = Thread(target=self._check_bridge_status)
        self.check_bridge_status_thread.start()

    def _check_bridge_status(self):
        def restart_bridge_on_error(bridge):
            try:
                message = bridge.ctx.q_manage_out.get_nowait()
                msg_type = message[0]
                if msg_type == 'Error':
                    ctx = bridge.ctx
                    name = ctx.name
                    print(f'Error in {name}, restarting...', )
                    bridge = Bridge(bridge.ctx)
                    print(f'Restarted {name}!')
            except Empty:
                pass
        current_thread().name = 'Check-Bridge-Status-Thread'
        while True:
            self.bridge_lock.acquire()
            for bridge in self.bridges:
                restart_bridge_on_error(bridge)
            self.bridge_lock.release()
            sleep(0.5)

    def add_bridge(self, context: Context):
        self.bridge_lock.acquire()
        self.bridges.append(Bridge(context))
        self.bridge_lock.release()

    def add_bridges(self, contexts: list):
        for context in contexts:
            self.add_bridge(context)


def _main():
    contexts = Context.load_from_config('config.ini')
    manager = BridgeManager()
    manager.add_bridges(contexts)

if __name__ == "__main__":
    _main()
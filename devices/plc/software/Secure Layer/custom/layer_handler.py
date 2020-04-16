from time import sleep
from context import Context
from layer import Layer
from threading import Lock, Thread, current_thread
from queue import Empty

# TODO: maybe make locking a decorator

class LayerHandler:

    def __init__(self):
        self.layers = []
        self.layer_lock = Lock()
        self.check_layer_status_thread = Thread(target=self._check_layer_status)
        self.check_layer_status_thread.start()

    def _check_layer_status(self):
        def restart_layer_on_error(layer):
            try:
                message = layer.ctx.q_manage_out.get_nowait()
                msg_type = message[0]
                if msg_type == 'Error':
                    ctx = layer.ctx
                    name = ctx.name
                    print(f'Error in {name}, restarting...', )
                    layer = Layer(layer.ctx)
                    print(f'Restarted {name}!')
            except Empty:
                pass

        current_thread().name = 'Check-Layer-Status-Thread'
        while True:
            self.layer_lock.acquire()
            for layer in self.layers:
                restart_layer_on_error(layer)
            self.layer_lock.release()
            sleep(0.5)

    def add_layer(self, context: Context):
        self.layer_lock.acquire()
        self.layers.append(Layer(context))
        self.layer_lock.release()

    def add_layers(self, contexts: list):
        for context in contexts:
            self.add_layer(context)

def main():
    layer_handler = LayerHandler()
    contexts = Context.load_from_config('config.ini')
    layer_handler.add_layers(contexts)

if __name__ == "__main__":
    main()

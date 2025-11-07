import time
import socket


class MidiBridge:
    def __init__(self):
        self.conn = None

        # -- la consola remota (Modem Atari)
        self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.conn.settimeout(1)
        self.conn.connect(("127.0.0.1", 9000))

    def run(self):
        while True:
            try:
                data = self.conn.recv(1)
                ch = data.decode("iso8859-1")

                if ch == "\r" or ch == "\n":
                    print(flush=True)
                else:
                    print(data, end="", flush=True)
            except socket.timeout:
                pass
            except KeyboardInterrupt:
                break
        self.conn.close()


# --- show time
# probado con Altirra
#   - SIO serial adapter - Networked serial port - port 9000/tcp at 19200
#   - xxxxx

app = MidiBridge()
app.run()

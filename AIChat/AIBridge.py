import time
from dotenv import load_dotenv
import socket
from google import genai


class AIBridge:
    def __init__(self):
        self.conn = None
        self.ai_client = None
        self.ai_chat = None

        # -- la consola remota (Modem Atari)
        self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.conn.settimeout(1)
        self.conn.connect(("127.0.0.1", 9000))
        self.conn.sendall(b"\x0C")

        # --- AI de Google, la API key debe estar en la variable de ambiente GEMINI_API_KEY
        system_instrucction = "Actúa como un asistente útil y amigable que responde en español si usar tildes ni simbolos. "
        system_instrucction += "Responde de forma concisa y clara. """
        system_instrucction += "No uses asteriscos ni algo parecido para negritas ni tipos de textos "
        system_instrucction += "Para listas usa guiones al inicio de cada ítem. "

        self.ai_client = genai.Client()
        self.ai_chat = self.ai_client.chats.create(
            model="gemini-2.5-flash",
            config=genai.types.GenerateContentConfig(
                system_instruction=system_instrucction,
            ),
        )

    def get_line(self):
        line = ""
        self.conn.sendall(b"> ")
        print("> ", end="", flush=True)
        while True:
            try:
                data = self.conn.recv(1)
                ch = data.decode("iso8859-1")

                if ch == "\r" or ch == "\n":
                    self.conn.sendall(b"\r\n")
                    print(flush=True)
                    return line.strip()
                else:
                    self.conn.sendall(data)
                    print(ch, end="", flush=True)
                    line = line + ch
            except socket.timeout:
                pass
            except KeyboardInterrupt:
                self.conn.close()
                return

    def send_message(self, line):
        if len(line) == 0:
            return

        print("Espere ...", flush=True)

        response = self.ai_chat.send_message_stream(line)
        for chunk in response:
            if chunk.text is None:
                continue
            self.conn.sendall(chunk.text.encode("utf-8"))
            print(chunk.text, end="", flush=True)
        self.conn.sendall(b"\r\n")
        print(flush=True)

    def run(self):
        while True:
            line = self.get_line()
            self.send_message(line)


# --- show time
# probado con Altirra
#   - SIO serial adapter - Networked serial port - port 9000/tcp at 19200
#   - FlickerTerm 80 - FlickerTerm 80 0.51.atr

load_dotenv()
app = AIBridge()
app.run()

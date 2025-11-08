import serial
import rtmidi


class MidiBridge:
    def __init__(self):
        self.conn = None

        # midi port
        self.midi_out = rtmidi.MidiOut()
        self.midi_out.open_virtual_port("Atari Bridge Port")

        # Atari SIO
        self.conn = serial.Serial("/dev/ttyUSB0", baudrate=31250, timeout=1)

    def run(self):
        while True:
            try:
                b0 = self.conn.read(1)
                if b0 == b"":
                    continue
                b0 = b0[0]

                # system common messages
                if b0 & 0xF0 == 0xF0:
                    if b0 == 0xF0:
                        pass
                    elif b0 in (0xF1, 0xF3):
                        b1 = self.conn.read(1)[0]
                        self.midi_out.send_message([b0, b1])
                    elif b0 == 0xF2:
                        b1 = self.conn.read(1)[0]
                        b2 = self.conn.read(1)[0]
                        self.midi_out.send_message([b0, b1, b2])
                    else:
                        self.midi_out.send_message([b0])

                # channel voice/mode messages
                elif b0 & 0xF0 in (0xC0, 0xD0):
                    b1 = self.conn.read(1)[0]
                    self.midi_out.send_message([b0, b1])
                else:
                    b1 = self.conn.read(1)[0]
                    b2 = self.conn.read(1)[0]
                    self.midi_out.send_message([b0, b1, b2])
            except serial.SerialTimeoutException:
                pass
            except KeyboardInterrupt:
                break
        self.conn.close()


# --- show time
app = MidiBridge()
app.run()

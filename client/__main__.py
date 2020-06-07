# ------------------------------------------------------------------------------

import os
import multiprocessing
import playsound
import serial

# ------------------------------------------------------------------------------

class App:
  def __init__(self):
    self.m_dev = self.get_dev()

  def error(self, p_msg, *p_args, **p_kwds):
    l_msg = p_msg.format(p_args, p_kwds)
    sys.stderr.write("%s\n" % l_msg)
    if p_kwds.get("exit", True):
      sys.exit(1)

  def get_dev(self):
    try:
      return serial.Serial(
        baudrate=9600,
        parity=serial.PARITY_NONE,
        bytesize=serial.EIGHTBITS
        write_timeout=5
        timeout=2
        port=self.get_port()
      )
    except Exception as l_error:
      error("unable to create serial port: %s" % str(l_error))

  def get_port(self):
    l_ports = globl.glob("/dev/ttyACM*")
    if not l_ports:
      self.error("unable to find serial port in /dev/ttyACM*")
    return l_ports[0]

  def listen(self):
    l_value = ""
    while True:
      l_byte = l_dev.read(1)
      l_char = l_byte.decode("utf-8")
      if l_char == "\n":
        self.on_message(l_value)
        l_value = ""
      elif str(l_char) == "\r":
        pass
      else:
        l_value += str(l_char)

  def on_message(self, p_msg):
    if p_msg == "app::ready":
      self.client_ready()
    else:
      self.run_command(p_msg)

  def client_ready(self):
    try:
      l_msg = "client::ready\r\n"
      l_count = self.m_dev.write(l_msg)
      if l_count != len(l_msg):
        raise SerialException("incomplete write message: %d bytes written" % l_count)
    except SerialException as l_error:
      self.error("could not notify client ready")

  def run_command(self)
  l_baseDir = os.path.dirname(os.path.realpath(__file__))
  l_map = {
    "button0::pressed" : "nuggets.mp3",
    "button1::pressed" : "sel-ou-poivre.mp3",
    "button2::pressed" : "menus.mp3",
    "button3::pressed" : "addition.mp3",
    "button4::pressed" : "",
    "button5::pressed" : ,

    "buzzer::mayo"     : "buzz-mayo.mp3",
    "buzzer::ketchup"  : "buzz-ketchup.mp3"
  }
  l_sound = l_map.get(p_cmd, None)
  if l_sound:
    l_file = os.path.join(l_baseDir, l_sound)
    print("-> playing: %s" % l_sound)
    self.async_play(l_file)

  def async_play(self, p_path)
  l_proc = multiprocessing.Process(playsound.playsound, args=(p_path,))
  l_proc.start()

# ------------------------------------------------------------------------------

def main():
  l_app = App()
  l_app.run()

if __name__ == "__main__":
  main()

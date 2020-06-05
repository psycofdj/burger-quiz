import os
import multiprocessing
import playsound
import serial

def run(p_cmd):
  l_baseDir = os.path.dirname(os.path.realpath(__file__))
  l_map = {
    "button0::pressed" : "nuggets.mp3",
    "button1::pressed" : "sel-ou-poivre.mp3",
    "button2::pressed" : "menus.mp3",
    "button3::pressed" : "addition.mp3",
    #"button4::pressed" : "??.mp3",
    "buzzer::mayo"     : "buzz-mayo.mp3",
    "buzzer::ketchup"  : "buzz-ketchup.mp3"
  }
  l_sound = l_map.get(p_cmd, None)
  if l_sound:
    print("-> playing: %s" % l_sound)
    playsound.playsound(os.path.join(l_baseDir, l_sound))

def handle(p_cmd):
  l_proc = multiprocessing.Process(target=run, args=(p_cmd,))
  l_proc.start()

def main():
  while True:
    try:
      l_dev = serial.Serial(baudrate=9600, parity=serial.PARITY_NONE, bytesize=serial.EIGHTBITS)
      l_dev.port ='/dev/ttyACM0'
      l_dev.timeout = 2
      l_dev.open()
      l_value = ""
      while True:
        l_byte = l_dev.read(1)
        l_char = l_byte.decode("utf-8")
        if l_char == "\n":
          print("-> %s" % l_value)
          handle(l_value)
          l_value = ""
        elif str(l_char) == "\r":
          pass
        else:
          l_value += str(l_char)
    except:
      print("fatal error")
      time.sleep(1)

if __name__ == "__main__":
  main()

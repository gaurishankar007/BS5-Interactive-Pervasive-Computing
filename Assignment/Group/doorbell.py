# <=========== SetUp Parameters ===========>

# Doorbell pin to start the call
DOORBELL_PIN = 26

# Doorlock pin to start the call
DOORLOCK_PIN = 16

# Define the servo motor pin
SERVO_PIN = 12

# Number of seconds to keep the call active
DOORBELL_SCREEN_ACTIVE_S = 60

# Define initial value for toggling the servo motor
doorOpen = False
angle = 0.0

# ID of the JITSI meeting room
JITSI_ID = "Smart-Doorbell-MeetUp"  # If None, the program generates a random UUID
# JITSI_ID = "hackershackdoorbellexample"

# Path to the SFX file
RING_SFX_PATH = "/home/pi/doorbell-ring.wav"  # If None, no sound effect plays
# RING_SFX_PATH = "/home/pi/ring.wav"

# Enables email notifications
ENABLE_EMAIL = True

# Email you want to send the notification from (only works with gmail)
FROM_EMAIL = 'sendermail670@gmail.com'

# You can generate an app password here to avoid storing your password in plain text
# this should also come from an environment variable
# https://support.google.com/accounts/answer/185833?hl=en
FROM_EMAIL_PASSWORD = '123@123Shikhar'

# Email you want to send the update to
TO_EMAIL = 'magarrupendra@gmail.com'


# <=========== RPI 4 Codings ===========>

import time
import os
import signal
import subprocess
import smtplib
import uuid

from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.image import MIMEImage

try:
    import RPi.GPIO as GPIO
except RuntimeError:
    print("Error importing RPi.GPIO. This is probably because you need superuser. Try running again with 'sudo'.")
    
    
def show_screen():
    os.system("tvservice -p")
    os.system("xset dpms force on")
    
    
def hide_screen():
    os.system("tvservice -o")
    
    
def send_email_notification(chat_url):
    if ENABLE_EMAIL:
        sender = EmailSender(FROM_EMAIL, FROM_EMAIL_PASSWORD)
        email = Email(
            sender,
            'Smart Doorbell',
            'Notification: Knock Knock! Who is in the home?',
            'A new person is awaiting in front of your home door. Meet the person at %s' % chat_url
        )
        email.send(TO_EMAIL)
        
        
def ring_doorbell(pin):    
    print("entered ring")
    SoundEffect(RING_SFX_PATH).play()
    chat_id = JITSI_ID if JITSI_ID else str(uuid.uuid4())
    video_chat = VideoChat(chat_id)
    send_email_notification(video_chat.get_chat_url())
    show_screen()
    video_chat.start()
    time.sleep(DOORBELL_SCREEN_ACTIVE_S)
    video_chat.end()
    hide_screen()
    
    
class SoundEffect:
    def __init__(self, filepath):
        self.filepath = filepath
    def play(self):
        if self.filepath:
            subprocess.Popen(["aplay", self.filepath])
            
            
class VideoChat:
    def __init__(self, chat_id):
        self.chat_id = chat_id
        self._process = None
        
    def get_chat_url(self):
        return "http://meet.jit.si/%s" % self.chat_id
    
    def start(self):
        if not self._process and self.chat_id:
            self._process = subprocess.Popen(["chromium-browser", "-kiosk", self.get_chat_url()])
        else:
            print("Can't start video chat -- already started or missing chat id")
            
    def end(self):
        if self._process:
            os.kill(self._process.pid, signal.SIGTERM)
            
class EmailSender:
    def __init__(self, email, password):
        self.email = email
        self.password = password
        
        
class Email:
    def __init__(self, sender, subject, preamble, body):
        self.sender = sender
        self.subject = subject
        self.preamble = preamble
        self.body = body
        
    def send(self, to_email):
        msgRoot = MIMEMultipart('related')
        msgRoot['Subject'] = self.subject
        msgRoot['From'] = self.sender.email
        msgRoot['To'] = to_email
        msgRoot.preamble = self.preamble
        msgAlternative = MIMEMultipart('alternative')
        msgRoot.attach(msgAlternative)
        msgText = MIMEText(self.body)
        msgAlternative.attach(msgText)
        smtp = smtplib.SMTP('smtp.gmail.com', 587)
        smtp.starttls()
        smtp.login(self.sender.email, self.sender.password)
        smtp.sendmail(self.sender.email, to_email, msgRoot.as_string())
        smtp.quit()
        
def doorlock_toggle(pin):
    global doorOpen
    global angle
    global SERVO_PIN
    
    GPIO.setup(SERVO_PIN, GPIO.OUT)
    servo1 = GPIO.PWM(SERVO_PIN, 50) # pin 32 for servo1, pulse 50Hz
    
    # Start PWM running, with value of 0 (pulse off)
    servo1.start(0)
    
    doorOpen = not doorOpen
    if doorOpen:
        angle = 0.0
    else:
        angle = 90.0
        
    servo1.ChangeDutyCycle(2+(angle/18))
    time.sleep(0.5)
    servo1.ChangeDutyCycle(0)                
        
class Doorbell:
    def __init__(self, doorbell_button_pin, doorlock_button_pin):
        self._doorbell_button_pin = doorbell_button_pin
        self._doorlock_button_pin = doorlock_button_pin
        
        
    def run(self):
        try:
            print("Starting Doorbell...")
            hide_screen()
            self._setup_gpio1()
            self._setup_gpio()
            print("Waiting for doorbell rings...")
            self._wait_forever()
            
        except KeyboardInterrupt:
            print("Safely shutting down...")
            
        finally:
            self._cleanup()
            
    def _wait_forever(self):
        while True:
            time.sleep(0.1)
            
    def _setup_gpio(self): 
        GPIO.setup(self._doorbell_button_pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
        GPIO.add_event_detect(self._doorbell_button_pin, GPIO.RISING, callback=ring_doorbell, bouncetime=60000)
        
    def _setup_gpio1(self): 
        GPIO.setmode(GPIO.BCM)   
        GPIO.setup(self._doorlock_button_pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)      
        GPIO.add_event_detect(self._doorlock_button_pin, GPIO.RISING, callback=doorlock_toggle, bouncetime=1000)
        
    def _cleanup(self):
        GPIO.cleanup(self._doorbell_button_pin)
        show_screen()
        
if __name__ == "__main__":
    doorbell = Doorbell(DOORBELL_PIN, DOORLOCK_PIN)
    doorbell.run()
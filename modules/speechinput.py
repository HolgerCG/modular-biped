import speech_recognition as sr
from pubsub import pub

class SpeechInput:
    """
    Use speech_recognition to detect and interpret audio
    """
    def __init__(self, **kwargs):
        self.recognizer = sr.Recognizer()
        self.mic = sr.Microphone()
        self.listening = False

        pub.subscribe(self.enable, 'hotword')
        pub.subscribe(self.detect, 'loop:1')

    def enable(self):
        pub.sendMessage('log', msg='[Speech] Listening')
        self.listening = True

    def disable(self):
        pub.sendMessage('log', msg='[Speech] Not Listening')
        self.listening = False

    def detect(self):
        if not self.listening:
            return None
        pub.sendMessage('log', msg='[Speech] Initialising Detection')
        with self.mic as source:
            self.recognizer.adjust_for_ambient_noise(source)
            pub.sendMessage('log', msg='[Speech] Detecting...')
            audio = self.recognizer.listen(source)
        pub.sendMessage('log', msg='[Speech] End Detection')
        try:
            val = self.recognizer.recognize_google(audio)
            pub.sendMessage('log', msg='[Speech] I heard: ' + str(val))
            self.disable()
            return val
        except sr.UnknownValueError as e:
            pub.sendMessage('log:error', msg='[Speech] Detection Error: ' + str(e))
            self.disable()
            return None

    def detect_from_file(self, file):
        f = sr.AudioFile(file)
        with f as source:
            audio = self.recognizer.record(source)
        return self.recognizer.recognize_sphinx(audio)

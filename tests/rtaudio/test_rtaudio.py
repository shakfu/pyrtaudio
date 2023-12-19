import rtaudio

a = rtaudio.RtAudio()

print(a.getDeviceCount())
print(a.getDefaultOutputDevice())
print(a.getDefaultInputDevice())
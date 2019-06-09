import wave
import subprocess
import time
import sys

if len(sys.argv) != 2:
	print("usage: python3 go.py songfile-basename")
	print("example:")
	print("$ python3 go.py 001")
	print("compiles 001.c and saves it as Song-001.wav")
	quit()

f = sys.argv[1]
wf = wave.open("Song-" + f + ".wav", "wb")
wf.setframerate(96000)
wf.setsampwidth(2)
wf.setnchannels(2)

subprocess.Popen(["gcc", f + ".c", "-o", "sing", "-O2"]).wait()
p = subprocess.Popen(["./sing"], stdout=subprocess.PIPE)

c = 8
while True:
	pcm = b""
	try:
		pcm = p.stdout.read(4096)
	except:
		pcm = b""

	if len(pcm) == 0:
		c -= 1
		if c <= 0:
			break
		time.sleep(0.1)
	else:
		wf.writeframes(pcm)
		c = 8

wf.close()

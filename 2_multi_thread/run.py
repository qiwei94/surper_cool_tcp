import subprocess
import time

for i in range(10):
	print i," begin run"
	output=subprocess.Popen("./client.o",shell=True)
	oc=output.communicate();
	print oc[0]
	time.sleep(2)
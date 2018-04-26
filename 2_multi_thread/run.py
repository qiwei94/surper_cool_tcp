import subprocess
import time
from multiprocessing import Process
import multiprocessing
import time

total_num = 2000
p_list=[]

def task(i):
	output=subprocess.Popen("./client.o "+str(i),shell=True)

for i in range(total_num):
	print i," begin run"
	name="p_"+str(i)
	p = Process(target=task, args=(i,),name=name)
	p_list.append(p)


for i in range(total_num):
	p=p_list[i]
	p.start()

print "The number of CPU is:" + str(multiprocessing.cpu_count())


for p in multiprocessing.active_children():
    print("child p.name: " + p.name + "\tp.id: " + str(p.pid))


while True:
	str = raw_input("enter your choice , quit is q:")
	print "receive command is ",str

	if str == "q":
		break
	else:
		print "i am wait to be killed"

	time.sleep(0.2)

for i in range(total_num):
			p=p_list[i]
			if p.is_alive():
				print  "Process:"+ p.pid+" is running"
        		p.terminate() 
print "all process closed"
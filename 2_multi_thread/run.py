import subprocess
import time
from multiprocessing import Process
import multiprocessing


total_num = 100
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


for i in range(total_num):
	p=p_list[i]
	p.join()
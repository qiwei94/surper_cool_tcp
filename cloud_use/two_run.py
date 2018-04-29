import subprocess
import time
def task1():
	output=subprocess.Popen("python run.py ",shell=True,close_fds=True)


def task2():
	output=subprocess.Popen("python run2.py ",shell=True,close_fds=True)


if __name__ == '__main__':
	print "begin the first 10000"
	task1()
	print "finish and wait for 10 seconds"
	time.sleep(100)
	task2()
	print "all finish"

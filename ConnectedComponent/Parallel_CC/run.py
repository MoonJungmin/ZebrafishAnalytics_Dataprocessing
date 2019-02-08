import os, sys
import platform

node_num = int(platform.node().split("emerald")[-1]) - 1
#print node_num
result_path = "/scratch/jmmoon/connectedcomponent/"

for v in range(0, 5):
	os.system("./build/app " + str(v) + " " + str(node_num) + " " +result_path)
	#print "./build/app " + str(v) + " " + str(node_num) + " " +result_path

	# rootpath = "/home/Pearl/ridwan/Data/Raw/"
	# dstpath = "/home/Pearl/ridwan/Data/Tif/"
	# for i in range(0, 128):
		# src = rootpath + "v" + str(v) + "_s" + str(i) + ".raw"
		# dst = dstpath + "v" + str(v) + "_s" + str(i) + ".tif"
		# os.system("raw2tiff -w 10240 -l 9216 -d long -p minisblack -M " +src + " " + dst)

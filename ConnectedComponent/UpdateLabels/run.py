import os, sys
import platform

node_num = int(platform.node().split("emerald")[-1]) - 1

start = 5*node_num
end = 5*(node_num + 1)

path = "/scratch/jmmoon/connectedcomponent/"


os.system("./build/app " + path + " " + str(start) + " " + str(end))
#print "./build/app " + path + " " + str(start) + " " + str(end)
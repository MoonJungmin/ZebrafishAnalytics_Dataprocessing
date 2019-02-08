import os, sys
import platform

node_num = int(platform.node().split("emerald")[-1]) - 1
os.system("./build/app " + str(node_num))
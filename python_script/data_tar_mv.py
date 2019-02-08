import os, sys

import platform

node_num = int(platform.node().split("emerald")[-1]) - 1

os.system("mv /scratch/jmmoon/connectedcomponent_tile/data.tar /scratch/jmmoon/connectedcomponent_tile/data_" + str(node_num) + ".tar")
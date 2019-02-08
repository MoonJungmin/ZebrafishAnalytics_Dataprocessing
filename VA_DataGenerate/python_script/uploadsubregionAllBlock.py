#!/home/jmmoon/anaconda2/bin/python

##################

import os, sys
from contextlib import closing
from paramiko import SSHConfig, SSHClient
import numpy
import io, json, time
import platform

node_number = int(platform.node().split("emerald")[-1])

options = {}
options['ip'] = "diamond.unist.ac.kr"
options['user'] = "jmmoon"
options['password'] = "132435a"
options['port'] = 22

options['path_xy'] = "/home/SharedHDD/cell_block/XY/"
options['path_yz'] = "/home/SharedHDD/cell_block/YZ/"
options['path_zx'] = "/home/SharedHDD/cell_block/ZX/"


options['input_xy'] = "/scratch/jmmoon/Cell/Block/XY/"
options['input_yz'] = "/scratch/jmmoon/Cell/Block/YZ/"
options['input_zx'] = "/scratch/jmmoon/Cell/Block/ZX/"

my_name_list = []

rsync -avrz /scratch/jmmoon/SubregionBlock/ jmmoon@10.20.12.52:/home/SharedHDD/SubregionBlock/


import os, sys
import platform

node_num = int(platform.node().split("emerald")[-1]) - 1

CellPath = "/scratch/jmmoon/Cell/Block/XY/";
SubregionName = ["EyeLeft", "EyeRight", "GangliaPosteriorLateralLineRight", "GangliaPosteriorLateralLineLeft", "GangliaAnteriorLateralLineRight", "GangliaAnteriorLateralLineLeft"]
SubregionPathRoot = "/scratch/jmmoon/SubregionBlock/";
OutputPathRoot = "/home/jmmoon/Projects/SubregionCellCheck/shared/"


for name in SubregionName:
	print name
	OutputPath1 = OutputPathRoot + name + "_" + str(node_num) + ".dat_include"
	OutputPath2 = OutputPathRoot + name + "_" + str(node_num) + ".dat_intersect"

	SubregionPath = SubregionPathRoot + name + "/XY/"
	os.system("./build/app " + CellPath + " " + SubregionPath + " " + OutputPath1 + " " + OutputPath2)
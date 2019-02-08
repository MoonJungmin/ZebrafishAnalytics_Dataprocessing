import os, sys
import numpy


SubregionName = ["EyeLeft", "EyeRight", "GangliaPosteriorLateralLineRight", "GangliaPosteriorLateralLineLeft", "GangliaAnteriorLateralLineRight", "GangliaAnteriorLateralLineLeft"]
Cell_IndexPath = "/home/jmmoon/Projects/SubregionCellCheck/shared/"


for name in SubregionName:

	mlist = []
	for i in range(0,16):
		fname = Cell_IndexPath + name + "_" + str(i) + ".dat_include"
		fp = open(fname, "r")
		
		content = fp.readlines()
		

		for c in content:
			cell_arr = c.split(" ")

			for index in cell_arr:
				if len(index) > 0:
					mlist.append(int(index))

	mArray = numpy.array(mlist, dtype=numpy.uint32)

	mArray = numpy.unique(mArray)

	print len(mArray)

	f = open(Cell_IndexPath + name + "_composited.dat_include", 'w')
	for d in mArray:
		f.write(str(d) + "\n")



for name in SubregionName:

	mlist = []
	for i in range(0,16):
		fname = Cell_IndexPath + name + "_" + str(i) + ".dat_intersect"
		fp = open(fname, "r")
		
		content = fp.readlines()
		

		for c in content:
			cell_arr = c.split(" ")

			for index in cell_arr:
				if len(index) > 0:
					mlist.append(int(index))

	mArray = numpy.array(mlist, dtype=numpy.uint32)

	mArray = numpy.unique(mArray)

	print len(mArray)

	f = open(Cell_IndexPath + name + "_composited.dat_intersect", 'w')
	for d in mArray:
		f.write(str(d) + "\n")




# mlist.sort(key=lambda x: x['index'], reverse=True)
# newlist = sorted(mlist, key=lambda k: k['index']) 

# resultlist = []
# prev_idx = -1
# for t in newlist:
# 	if prev_idx == -1:
# 		prev_idx = t['index']
# 		resultlist.append(t)
# 	else:
# 		if prev_idx == t['index']:
# 			if resultlist[-1]['minX'] > t['minX']:
# 				resultlist[-1]['minX'] = t['minX']

# 			if resultlist[-1]['minY'] > t['minY']:
# 				resultlist[-1]['minY'] = t['minY']

# 			if resultlist[-1]['minZ'] > t['minZ']:
# 				resultlist[-1]['minZ'] = t['minZ']

# 			if resultlist[-1]['maxX'] < t['maxX']:
# 				resultlist[-1]['maxX'] = t['maxX']

# 			if resultlist[-1]['maxY'] < t['maxY']:
# 				resultlist[-1]['maxY'] = t['maxY']

# 			if resultlist[-1]['maxZ'] < t['maxZ']:
# 				resultlist[-1]['maxZ'] = t['maxZ']
						
				
# 		else:
# 			resultlist.append(t)

# 		prev_idx = t['index']	



# f = open("/home/jmmoon/Projects/ConnectedComponent/shared/BoundingBoxComp.dat", 'w')
# for t in resultlist:
#     data = str(t['index']) + " " + str(t['minX']) + " " + str(t['minY']) + " " + str(t['minZ']) + " " + str(t['maxX']) + " " + str(t['maxY']) + " " + str(t['maxZ']) + " " + str(t['seedX']) + " " + str(t['seedY']) + " " + str(t['seedZ']) + "\n"
#     f.write(data)

# f.close()

# prev_idx = -1
# for t in resultlist:
# 	if prev_idx == -1:
# 		prev_idx = t['index']
# 	else:
# 		if prev_idx >= t['index']:
# 			print("Error")
# 		prev_idx = t['index']	

# print len(newlist)
# print len(resultlist)
# # print mlist[-1]['index']
# # print len(mlist)





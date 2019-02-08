import os, sys



mlist = []
for i in range(0,16):
	fname = "/home/jmmoon/Projects/ConnectedComponent/shared/BoundingBox_" + str(i) + ".dat";
	content = []
	with open(fname) as f:
		content = f.readlines()

		for line in content:
			obj = {}
			l = line.split("\n")[0].split(" ")
			obj['index'] = int(l[0])
			obj['minX'] = int(l[1])
			obj['minY'] = int(l[2])
			obj['minZ'] = int(l[3])
			obj['maxX'] = int(l[4])
			obj['maxY'] = int(l[5])
			obj['maxZ'] = int(l[6])
			obj['seedX'] = int(l[7])
			obj['seedY'] = int(l[8])
			obj['seedZ'] = int(l[9])
			


			mlist.append(obj)

# mlist.sort(key=lambda x: x['index'], reverse=True)
newlist = sorted(mlist, key=lambda k: k['index']) 

resultlist = []
prev_idx = -1
for t in newlist:
	if prev_idx == -1:
		prev_idx = t['index']
		resultlist.append(t)
	else:
		if prev_idx == t['index']:
			if resultlist[-1]['minX'] > t['minX']:
				resultlist[-1]['minX'] = t['minX']

			if resultlist[-1]['minY'] > t['minY']:
				resultlist[-1]['minY'] = t['minY']

			if resultlist[-1]['minZ'] > t['minZ']:
				resultlist[-1]['minZ'] = t['minZ']

			if resultlist[-1]['maxX'] < t['maxX']:
				resultlist[-1]['maxX'] = t['maxX']

			if resultlist[-1]['maxY'] < t['maxY']:
				resultlist[-1]['maxY'] = t['maxY']

			if resultlist[-1]['maxZ'] < t['maxZ']:
				resultlist[-1]['maxZ'] = t['maxZ']
						
				
		else:
			resultlist.append(t)

		prev_idx = t['index']	



f = open("/home/jmmoon/Projects/ConnectedComponent/shared/BoundingBoxComp.dat", 'w')
for t in resultlist:
    data = str(t['index']) + " " + str(t['minX']) + " " + str(t['minY']) + " " + str(t['minZ']) + " " + str(t['maxX']) + " " + str(t['maxY']) + " " + str(t['maxZ']) + " " + str(t['seedX']) + " " + str(t['seedY']) + " " + str(t['seedZ']) + "\n"
    f.write(data)

f.close()

prev_idx = -1
for t in resultlist:
	if prev_idx == -1:
		prev_idx = t['index']
	else:
		if prev_idx >= t['index']:
			print("Error")
		prev_idx = t['index']	

print len(newlist)
print len(resultlist)
# print mlist[-1]['index']
# print len(mlist)





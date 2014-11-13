f = open("database2.txt", "w")
cols = ['essn', 'name', 'salary', 'depno']
entries = [['a123', 'shash', '32000', '5'], ['b119', 'shar', '372000', '3'], ['c003', 'prak', '2000', '4'], ['d103', 'tim', '20000', '1']]
for col in cols:
    f.write(col + ("#" * (40 - len(col) )))
f.write("\n")

for row in entries:
    for entry in row:
        f.write(entry + ("#" * (40 - len(entry) )))
    f.write("\n")
f.close()
#f = open("test", "r")
#f.seek(3)
#line = f.readline()
#print line
#print len(line)

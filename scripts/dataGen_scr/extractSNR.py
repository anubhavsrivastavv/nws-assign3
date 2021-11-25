import csv
stringToMatch = 'Avg snr value saved:' 
matchedLine = '' 

#get line 
count = 0
f1 = open('SNR', 'a')
entries = []
fileList = ["logsnr.txt"]
for fileName in fileList:
    with open(fileName, 'r') as inFile:
        for line in inFile: 
            if stringToMatch in line: 
                matchedLine = line 
                matchedLine = matchedLine.split(":")[1]
                f1.write(matchedLine)

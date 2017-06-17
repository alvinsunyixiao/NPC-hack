#Written by Randy Pan
import datetime
import os
import shutil
import csv
import subprocess
import time
import sys

questionNo=input("Question Number: ")
log=open("log.txt","a")

programs=[]
directory=os.getcwd()
if len(sys.argv) == 2 and os.path.isdir(sys.argv[1]):
    directory = os.path.abspath(sys.argv[1])

for i in os.listdir(directory):
    if (i[-3:]==".py" and i!="judge.py") or i[-4:]==".exe" or i[-4:]==".jar":
        programs.append(os.path.join(directory, i))


maps=[]
guidebooks=[]
nights=[None for i in range(100)]

for i in os.listdir(directory):
    if i[-6:]=="-night":
        nights[int(i[:-8])]=i

for i in nights:
    if i!=None:
        maps.append(os.path.join(directory, i, 'map.csv'))
        guidebooks.append(os.path.join(directory, i, "guidebook.csv"))

def CalculateDistance(Location1,Location2):
    Distance=((int(Location1[0])-int(Location2[0]))**2+(int(Location1[1])-int(Location2[1]))**2)**0.5
    return Distance

def readInfo():
    mapList=[]
    guidebookFile=csv.reader(open('guidebook.csv',newline=''))
    for row in guidebookFile:
        guidebookList=row
    mapFile=csv.reader(open('map.csv',newline=''))
    for row in mapFile:
        mapList.append(row)
    return guidebookList,mapList

def checkValid(solution,solutionFile):
    guidebookList,mapList=readInfo()
    lastrow=[0,0]
    distance=0
    track=[]
    trees=[]
    allExist=True
    for row in solution:
        distance+=CalculateDistance(row,lastrow)
        lastrow=row
        try:
            track.append(row[2])
            trees.append(row)
            if row not in mapList:
                allExist=False
        except:log.write(str(datetime.datetime.now())+" #Caution: Irrelevant content found in solution file created by "+eachProgram+"\n")

    if int(distance)==int(solutionFile[9:-4]) or int(distance)+1==int(solutionFile[9:-4]):
        valid="Valid"
    else: valid="Invalid"
    if track!=guidebookList or not allExist:
        valid="Invalid"
    if questionNo=="2" and valid=="Valid":
        valid=checkDuplicate(valid,trees)
    if valid=="Invalid":
        log.write(str(datetime.datetime.now())+" #Caution: Result of"+eachProgram+" is invalid\n")
    return valid,distance

def checkDuplicate(valid,trees):
    usedTree=[]
    for row in trees:
        if row in usedTree:
            valid="Invalid"
        usedTree.append(row)
    return valid

def judgeSolution():
    for i in os.listdir(directory):
        if i[:8]=="solution" and i[-4:]==".csv":
            solutionFile=i
    try:
        file=open(solutionFile,newline='')
        solution=csv.reader(file)
        valid,distance=checkValid(solution,solutionFile)
        file.close()
        removeSolutions()
        result.append([distance,valid])
    except:
        result.append("N/A")
        log.write(str(datetime.datetime.now())+" !WARNING!: An error occured when judging the result of "+eachProgram+"\n")

def removeSolutions():
    for i in os.listdir(directory):
        if i[:8]=="solution" and i[-4:]==".csv":
            os.remove(i)

def startProcess(program):
    child=subprocess.Popen(program,shell=True)
    try: child.wait(3600)
    except:
        subprocess.Popen("TASKKILL /F /PID {pid} /T".format(pid=child.pid))
        log.write(str(datetime.datetime.now())+" !WARNING!: "+program+" terminated after time limit exceeded\n")
        time.sleep(1)


def testProgram(program):
    for i in range(len(maps)):
        shutil.copy(maps[i],directory)
        shutil.copy(guidebooks[i],directory)
        log.write(str(datetime.datetime.now())+" log: Process started for "+eachProgram+"\n")
        begin = datetime.datetime.now()
        startProcess(program)
        end = datetime.datetime.now()
        log.write(str(datetime.datetime.now())+" log: Process ended for "+eachProgram+"\n")
        judgeSolution()
        log.write(str(datetime.datetime.now())+" log: Judged night"+str(i+1)+" of "+eachProgram+"\n")
        try:result[i].append(end-begin)
        except:pass
    dataBase=open("database.txt","a")
    basicInfo=program+" Question Number "+questionNo+"\n"
    dataBase.write(basicInfo)
    for i in result:
        if i!="N/A":
            for j in range(3):
                if j!=2:dataBase.write(str(i[j])+" ")
                else:dataBase.write(str(i[j])+"\n")
        else:dataBase.write(i+"\n")
    dataBase.write("\n")
    dataBase.close()

for eachProgram in programs:
    log.write(str(datetime.datetime.now())+" log: Judging started for "+eachProgram+"\n")
    result=[]
    testProgram(eachProgram)
    print(result)
    log.write(str(datetime.datetime.now())+" log: Judging completed for "+eachProgram+"\n")

log.close()
print("Completed")
input("Press any to quit")

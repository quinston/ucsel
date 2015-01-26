import subprocess
import tempfile
import os
import re

uwaterlooDatasetsRepo = "https://github.com/uWaterloo/Datasets.git"
subjectAndNumberPatt = re.compile(r'^"([A-Z]+)","([A-Z0-9]+)".*$')

def getCourseList():
 # tmpDir is a str
 tmpDir = tempfile.mkdtemp()
 subprocess.call("git clone {} {}".format(uwaterlooDatasetsRepo, tmpDir).split(' '))

 csvDir = os.path.join(tmpDir, "Courses")
 csvNames = os.listdir(csvDir)
 for filename in csvNames:
  csv = open(os.path.join(csvDir, filename))
  # skip heading line
  csv.readline()
  for line in csv:
   try:
    match = subjectAndNumberPatt.match(line)
    subject = match.group(1)
    number = match.group(2)
   except:
    print(line)
    raise
   print(subject, number)
  csv.close()

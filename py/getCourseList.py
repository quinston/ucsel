import subprocess
import tempfile
import os

uwaterlooDatasetsRepo = "https://github.com/uWaterloo/Datasets.git"

def getCourseList():
 # tmpDir is a str
 tmpDir = tempfile.mkdtemp()
 subprocess.call("git clone {} {}".format(uwaterlooDatasetsRepo, tmpDir).split(' '))



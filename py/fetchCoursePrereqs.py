#!/usr/bin/env python3
import urllib.request
import getCourseList
import common

apiUrl = "https://api.uwaterloo.ca/v2/courses/{subject}/{catalogueNumber}/prerequisites.json?key={key}"
destDirectory = common.dataDirectory + "/prerequisites"

"""
key: Waterloo OpenData API key
"""
def fetchCoursePrereqs(key):
 courses = getCourseList.getCourseList()
 
 for subject, catalogueNumber in courses:
  f = open("{path}/{subject}-{catalogueNumber}.json".format(path=destDirectory, subject=subject, catalogueNumber=catalogueNumber), "w")
  r = urllib.request.urlopen(apiUrl.format(subject=subject, catalogueNumber=catalogueNumber, key=key))
  f.write(r.read().decode("utf8"))
  f.close()

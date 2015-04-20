import json
import common

def getPrereqsFromJson(jsonStr):
	try:
		parsed = json.loads(jsonStr)
		return parsed["data"]["prerequisites_parsed"]
	except ValueError:
		# could not read prereqs, possibly because the file was blank.
		return []

def getSeasonsFromJson(jsonStr):
	try:
		parsed = json.loads(jsonStr)
		return parsed["data"]["terms_offered"]
	except ValueError:
		return []

"""
Returns a record (make-course <courseName> <seasons offered>) for use
in the Guile interpreter.

courseName: string naming a course. Hyphenate like so: CS-101
"""
def lispifyReqs(courseName):
	with open("{}/info/{}.json".format(common.dataDirectory, courseName)) as infoDataFile, open("{}/prerequisites/{}.json".format(common.dataDirectory, courseName)) as prereqsDataFile:
		# TODO: Course prereqs. 
		return "(make-course {} {})".format('"' + courseName + '"', "'(" + " ".join(map(lambda x: str(x), getSeasonsFromJson(infoDataFile.read()))) + ")")

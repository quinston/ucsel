import re

"""
Capture groups:
* Course code, without spaces
* Description of components (e.g. LAB, TUT...) and weight (e.g. 0.5 credits)
* Course ID (6 digits?)
* Course name
* Course description
 * may include offering schedule (starts with "[Offered:")
* Additional info, including:
 * Notes (start with "[Note:")
  * may include offering schedule
 * Prereqs (start with "Prereq:")
 * Antireqs (start with "Antireq:")
 * "Department Consent Required" (as printed)
 * "Only offered Online"
 * "Also offered Online"
"""


wholePattern = re.compile(r"""<center><table border=0 width=80%><tr><td align=left><b><a name = "(?P<courseCode>[A-Z0-9]+)"></a>(?P<components>[^<]+)</b></td><td align=right>Course ID: (?P<courseID>\d+)</td></tr><tr><td colspan=2><b>(?P<courseName>[^<]+)</b></td></tr><tr><td colspan=2>(?P<courseDesc>[^<]+)</td></tr>((?:<tr><td colspan=2><i>[^<]*</i></td></tr>)*)""")
notePattern = re.compile(r"<i>([^<]*)</i>")

def normalizeHtml(html):
 # Lowecase tag names
 tagPattern = re.compile(r"<(/?\w+)((?: [^>]*)?)>")
 return tagPattern.sub(lambda m: "<" + m.group(1).lower() + m.group(2) + ">", html)

"""
Returns a list of tuples like this:
("ABC101", "ABC 101 LEC 0.50", "110103", "Course Name", "Course descrption goes here.", ["Note 1", "Note 2"])
"""
def extractCourseInfo(html):
 html = normalizeHtml(html)
 return [x[:-1] + tuple([notePattern.findall(x[-1])]) for x in wholePattern.findall(html)]

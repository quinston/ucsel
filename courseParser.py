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


wholePattern = re.compile(r"""<center><table border=0 width=80%><tr><td align=left><B><a name = "(?P<courseCode>[A-Z0-9]+)"></a>(?P<components>[^<]+)</b></td><td align=right>Course ID: (?P<courseID>\d+)</td></tr><tr><td colspan=2><b>(?P<courseName>[^<]+)</B></td></tr><tr><td colspan=2>(?P<courseDesc>[^<]+)</td></tr>(?:<tr><td colspan=2><i>([^<]*)</i></td></tr>)*<p></table></center>""")

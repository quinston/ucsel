import urllib.request
from html.parser import HTMLParser

def fetchHtmlInSubject(subject):
	""" Downloads the page of the undergraduate calendar for all courses of the given subject. """
	url = "http://ugradcalendar.uwaterloo.ca/courses/{}".format(subject)
	response = urllib.request.urlopen(url)
	data = response.read()
	return data.decode()

def getCourseTableTags(html):
	"""Returns a list of strings, each string which is a <table></table> tag containing all the information on one course."""
	

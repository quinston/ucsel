cmake_minimum_required (VERSION 3.0)

add_custom_target(course-info
  python3 -c "import fetchCourseInfo; fetchCourseInfo.fetchCourseInfo(input(\"Enter API key\"));" WORKING_DIRECTORY ${ucsel_SOURCE_DIR}/py VERBATIM)

add_custom_target(course-prereqs
python3 -c "import fetchCoursePrereqs; fetchCoursePrereqs.fetchCoursePrereqs(input(\"Enter API key\"));" WORKING_DIRECTORY ${ucsel_SOURCE_DIR}/py VERBATIM)

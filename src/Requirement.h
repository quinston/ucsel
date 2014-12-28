#pragma once
#include<boost/graph/graph_traits.hpp>
#include<boost/graph/adjacency_list.hpp>


class Course;

struct Requirement {
 virtual bool is_satisfied() = 0;
};

/*
 Might not be necessary due to the nature of
backtracking (this requirement might be
implicitly guaranteed fulfilment)
*/
struct PrereqCoursesRequirement {
 std::vector<Course> prereq_courses;
 PrereqCoursesRequirement(std::vector<Course>&&);
 bool is_satisfied();
};

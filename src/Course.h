#pragma once 
#include<memory>
#include<string>
#include "Requirement.h"
#include<boost/graph/graph_traits.hpp>
#include<boost/graph/adjacency_list.hpp>

enum { NEVER_TAKEN };



struct Course {
enum CourseType {
 Normal,
 Disjunction
};
std::vector<Requirement> reqs_;
 CourseType ct_;
 bool is_taken_ = false;
 std::string name_;
};


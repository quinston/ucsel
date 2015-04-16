#include<iostream>
#include<vector>
#include<string>
#include<libguile.h>
#include "base/logging.h"
#include "constraint_solver/constraint_solver.h"

using operations_research::Solver;
using operations_research::DecisionBuilder;
using operations_research::IntVar;
using operations_research::Constraint;
using std::cout;
using std::string;

/**
An internal method. Adds to the solver a variable named \p courseName and looks up relevant restrictions, such as:

* prerequisites
* season of offering
\param courseName a string containing the name of the course
\param termList a list of the symbols {'W, 'F, 'S} representing the seasons during each user's terms
*/
static SCM add_course_constraint(SCM courseName, SCM termList) {
 SCM_ASSERT_TYPE(scm_is_string(courseName), courseName, 0, "add-course-constraint", "string");
 SCM_ASSERT_TYPE(scm_is_true(scm_list_p(termList)), termList, 1, "add-course-constraint", "list");

 scm_dynwind_begin((scm_t_dynwind_flags) 0);
 char* name = scm_to_locale_string(courseName);
 scm_dynwind_free(name);
 scm_dynwind_end();
 return SCM_BOOL_T;
}


static void inner_main(void *data, int argc, char **argv) {
 scm_c_define_gsubr("add-course-constraint", 2, 0, 0, (SCM (*)()) add_course_constraint);
 scm_shell (argc, argv);
}

int main(int argc, char** argv) {
 scm_boot_guile(argc, argv, inner_main, 0);

 return 0;
}

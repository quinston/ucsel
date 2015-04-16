#include<iostream>
#include<vector>
#include<libguile.h>
#include "base/logging.h"
#include "constraint_solver/constraint_solver.h"

using operations_research::Solver;
using operations_research::DecisionBuilder;
using operations_research::IntVar;
using operations_research::Constraint;
using std::cout;

void add_season_constraint();

/**
solve: (listof Course) (listof Term) -> (union Solution #f)
*/
static SCM solve(SCM courseList, SCM termList) {
 return SCM_BOOL_F;
}


static void inner_main(void *data, int argc, char **argv) {
 scm_c_define_gsubr("solve", 2, 0, 0, (SCM (*)()) solve);
 scm_shell (argc, argv);
}

int main(int argc, char** argv) {
 scm_boot_guile(argc, argv, inner_main, 0);

 return 0;
}

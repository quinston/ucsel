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

static void inner_main(void *data, int argc, char **argv) {
 scm_shell (argc, argv);
}

int main(int argc, char** argv) {
 scm_boot_guile(argc, argv, inner_main, 0);

 return 0;
}

/*!

@file main.cpp

Implements a small Scheme wrapper to OR tools.
*/

#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<algorithm>
#include<functional>
#include<libguile.h>
#include "base/logging.h"
#include "constraint_solver/constraint_solver.h"

using operations_research::Solver;
using operations_research::DecisionBuilder;
using operations_research::IntVar;
using operations_research::Constraint;
using std::cout;
using std::string;

typedef std::map<string, IntVar*> CourseMap;
typedef std::map<unsigned int, IntVar*> MaximumMap;

Solver solver ("anonymous");
CourseMap cm {};
MaximumMap mm {};

/*!  An internal method. Adds to the solver a variable named \p courseName.

TODO: automatically up relevant restrictions, such as: 
- prerequisites
- season of offering

\param noTerms The number of terms being considered
\param type The type of constraint to add.
 
 Currently supported:
- "bad term": arg is a term in which this course shall not or cannot be taken
- "oneof": arg is a list of courses which alongside the course \p courseName at least one course should be taken
- "prereq": arg is a lsit of courses and/or term numbers that should occur after the given course \p courseName
 
\param courseName A string identifying a course 
\param arg Function depends on \p type
*/
 SCM add_course_constraint (SCM noTerms, SCM type, SCM courseName, SCM arg)
{
	const char thisMethodName[] = "add-course-constraint";
	char* szType;
	string strType;
	char* szName;
	string strName;
	unsigned char ucNoTerms;
	unsigned char badTerm;
	IntVar* iv;
	char* szOtherName;
	string strOtherName;
	IntVar* ivOther;
	std::function<IntVar*(string)> getNamedIntVar;

	SCM_ASSERT_TYPE (scm_is_integer (noTerms), noTerms, 0, thisMethodName, "int");
	SCM_ASSERT_TYPE (scm_is_string (type), type, 1, thisMethodName, "string");
	SCM_ASSERT_TYPE (scm_is_string (courseName), courseName, 2, thisMethodName, "string");

	scm_dynwind_begin ( (scm_t_dynwind_flags) 0);
	ucNoTerms = scm_to_uint8 (noTerms);
	szType = scm_to_locale_string (type);
	scm_dynwind_free (szType);
	szName = scm_to_locale_string (courseName);
	scm_dynwind_free (szName);

	strName = string (szName);
	strType = string (szType);

	getNamedIntVar = [ucNoTerms](const string&& name) {
		if (cm.find(name) == cm.end()) {
			/*
			Convention: If a course has value \p noTerms, then it is not being taken.

			*/
			return cm[name] = solver.MakeIntVar (0, ucNoTerms, name);
		}
		else {
			return cm.find (name)->second;
		}
	};
	iv = getNamedIntVar(strName);


	if (strType == "bad term") {
		SCM_ASSERT (scm_is_integer (arg) || scm_is_string(arg), arg, 3, thisMethodName);
		if (scm_is_integer (arg)) {
			badTerm = scm_to_uint8 (arg);
			solver.AddConstraint (solver.MakeNonEquality (iv, badTerm));
		}
		else if (scm_is_string(arg)) {
			szOtherName = scm_to_locale_string(arg);
			scm_dynwind_free(szOtherName);
			strOtherName = string (szOtherName);
			ivOther = getNamedIntVar(strOtherName);
			solver.AddConstraint(solver.MakeNonEquality (iv, ivOther));
		}
		else {
			scm_wrong_type_arg_msg(thisMethodName, 3, arg, "one of: integer, string");
		}
	}
	else if (strType == "prereq") {
		SCM_ASSERT (scm_is_integer (arg) || scm_is_string(arg), arg, 3, thisMethodName);
		if (scm_is_string(arg)) {
			szOtherName = scm_to_locale_string(arg);
			scm_dynwind_free(szOtherName);
			strOtherName = string (szOtherName);
			ivOther = getNamedIntVar(strOtherName);
			solver.AddConstraint (solver.MakeLess(iv, ivOther));
		}
		else if (scm_is_integer (arg)) {
			solver.AddConstraint(solver.MakeLess(iv, scm_to_uint8(arg)));
		}
		else {
			scm_wrong_type_arg_msg(thisMethodName, 3, arg, "one of: integer, string");
		}
	}
	else if (strType == "oneof") {
		SCM_ASSERT_TYPE (scm_is_true(scm_list_p(arg)), arg, 3, thisMethodName, "list");
		std::vector<IntVar*> disjugates = {iv};
		while (!scm_is_null(arg)) {
			SCM head = scm_list_ref(arg, scm_from_int8(0));
			SCM_ASSERT_TYPE (scm_is_string(head), head, 3, thisMethodName, "list of string");
			szOtherName = scm_to_locale_string(head);
			scm_dynwind_free(szOtherName);
			strOtherName = string (szOtherName);
			disjugates.push_back(getNamedIntVar(strOtherName));
			// Replace arg with (cdr arg)
			arg = scm_list_tail(arg, scm_from_int8(1));
		}
		// Check that they are not all equal to noTerms
		solver.AddConstraint(solver.MakeLess(solver.MakeSum(disjugates),
											(int) (disjugates.size() * ucNoTerms)));
	}
	else {
		scm_misc_error (thisMethodName, "Unsupported constraint type: ~s", type);
		return SCM_BOOL_F;
	}

	scm_dynwind_end();
	return scm_from_unsigned_integer (0);
}

/*!
An internal method. Adds a cap on the number of courses that can be taken in one term.

In order to create this constraint, all the variables have to be created first.
Hence the constraints are only made at the start of a solve.

\param numTerm Term number (zero-indexed)
\param maximumFrequency The maximum number of times this term number should appear i.e. the maximum number of courses that can be taken in the given term
*/

SCM add_load_cap(SCM numTerm, SCM maximumFrequency) {
	const char thisMethodName[] = "add-load-cap";
	SCM_ASSERT_TYPE(scm_is_integer(numTerm), numTerm, 0, thisMethodName, "integer");
	SCM_ASSERT_TYPE(scm_is_integer(maximumFrequency), maximumFrequency, 1, thisMethodName, "integer");
	mm[scm_to_uint64(numTerm)]
	= solver.MakeIntVar(0,
					scm_to_uint64(maximumFrequency),
					"Term " + std::to_string(scm_to_uint64(numTerm)) + " Workload");
	return scm_from_unsigned_integer(0);
}

/*!
Invokes the solver to find a solution. Once the solutions are exhausted, it throws with key \p 'no-more a single argument containing an error message string.
*/
SCM find_solution()
{
	std::vector<IntVar*> courseVars {};
	std::vector<IntVar*> loadVars {};
	std::vector<IntVar*> vars {};

	std::transform (cm.begin(), cm.end(), std::back_inserter (courseVars), [] (const CourseMap::value_type & p) {
		return p.second;
	});
	std::transform (mm.begin(), mm.end(), std::back_inserter (loadVars), [] (const MaximumMap::value_type & p) {
		return p.second;
	});
	std::copy(courseVars.begin(), courseVars.end(), std::back_inserter(vars));
	std::copy(loadVars.begin(), loadVars.end(), std::back_inserter(vars));


	if (solver.state() == Solver::OUTSIDE_SEARCH) {
		scm_write_line(scm_from_utf8_string("Starting new solve\n"), scm_current_error_port());

		/* Course load constraints */
		for (auto p : mm) {
			solver.AddConstraint(solver.MakeCount(courseVars, p.first, p.second));
		}

		DecisionBuilder* db = solver.MakePhase (vars, Solver::CHOOSE_RANDOM, Solver::ASSIGN_RANDOM_VALUE);
		solver.NewSearch (db);
	}

	if (solver.NextSolution()) {
		SCM result = scm_list_n (SCM_UNDEFINED);

		for (const IntVar* var : courseVars) {
			/* Assignment is necessary since scm_assoc_set_x may or may
			not mutate the given list. */
			result = scm_assoc_set_x (result, scm_from_locale_string (var->name().c_str()), scm_from_int64 (var->Value()));
		}

		return result;
	} else {
		solver.EndSearch();
		cm.erase(cm.begin(), cm.end());
		mm.erase(mm.begin(), mm.end());
		return scm_throw (scm_from_utf8_symbol ("no-more"), scm_list_1 (scm_from_utf8_string ("No more solutions.")));
	}
}


static void inner_main (void* data, int argc, char** argv)
{
	scm_c_define_gsubr ("add-course-constraint", 4, 0, 0, (scm_t_subr) add_course_constraint);
	scm_c_define_gsubr ("find-solution", 0, 0, 0, (scm_t_subr) find_solution);
	scm_c_define_gsubr ("add-load-cap", 2, 0, 0, (scm_t_subr) add_load_cap);
	scm_shell (argc, argv);
}

int main (int argc, char** argv)
{
	scm_boot_guile (argc, argv, inner_main, 0);

	return 0;
}

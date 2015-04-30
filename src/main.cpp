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

Solver solver ("anonymous");
CourseMap cm {};

/**
An internal method. Adds to the solver a variable named \p courseName.

TODO: automatically up relevant restrictions, such as:
* prerequisites
* season of offering

\param noTerms The number of terms being considered
\param type The type of constraint to add.

Currently supported:
- "bad term": arg is a term in which this course shall not or cannot be taken

\param courseName A course identifier in this format: "CODE-000" (e.g. "CS-100")
\param arg Function depends on \p type
*/
static SCM add_course_constraint (SCM noTerms, SCM type, SCM courseName, SCM arg)
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
	}
	else if (strType == "prereq") {
		SCM_ASSERT_TYPE(scm_is_string(arg), arg, 3, thisMethodName, "string");
		szOtherName = scm_to_locale_string(arg);
		scm_dynwind_free(szOtherName);
		strOtherName = string (szOtherName);
		ivOther = getNamedIntVar(strOtherName);
		solver.AddConstraint (solver.MakeLess(iv, ivOther));
	}
	else {
		scm_misc_error (thisMethodName, "Unsupported constraint type: ~s", type);
		return SCM_BOOL_F;
	}

	scm_dynwind_end();
	return scm_from_unsigned_integer (0);
}

/**
Invokes the solver to find a solution.
*/
SCM find_solution()
{
	std::vector<IntVar*> vars {};
	std::transform (cm.begin(), cm.end(), std::back_inserter (vars), [] (const CourseMap::value_type & p) {
		return p.second;
	});

	if (solver.state() == Solver::OUTSIDE_SEARCH) {
		scm_write_line(scm_from_utf8_string("Starting new solve\n"), scm_current_error_port());
		DecisionBuilder* db = solver.MakePhase (vars, Solver::CHOOSE_RANDOM, Solver::ASSIGN_RANDOM_VALUE);
		solver.NewSearch (db);
	}

	scm_simple_format(scm_current_error_port(),
					scm_from_utf8_string("Solving with ~A variables\n"),
					scm_list_n(scm_from_uint64(vars.size()), SCM_UNDEFINED));

	if (solver.NextSolution()) {
		SCM result = scm_list_n (SCM_UNDEFINED);

		for (const IntVar* var : vars) {
			/* Assignment is necessary since scm_assoc_set_x may or may
			not mutate the given list. */
			result = scm_assoc_set_x (result, scm_from_locale_string (var->name().c_str()), scm_from_int64 (var->Value()));
		}

		return result;
	} else {
		solver.EndSearch();
		cm.erase(cm.begin(), cm.end());
		return scm_throw (scm_from_utf8_symbol ("no-more"), scm_list_1 (scm_from_utf8_string ("No more solutions.")));
	}
}


static void inner_main (void* data, int argc, char** argv)
{
	scm_c_define_gsubr ("add-course-constraint", 4, 0, 0, (scm_t_subr) add_course_constraint);
	scm_c_define_gsubr ("find-solution", 0, 0, 0, (scm_t_subr) find_solution);
	scm_shell (argc, argv);
}

int main (int argc, char** argv)
{
	scm_boot_guile (argc, argv, inner_main, 0);

	return 0;
}

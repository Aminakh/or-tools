// Copyright 2010-2014 Google
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Utility functions to interact with an lp solver from the SAT context.

#ifndef OR_TOOLS_SAT_LP_UTILS_H_
#define OR_TOOLS_SAT_LP_UTILS_H_

#include "sat/boolean_problem.pb.h"
#include "linear_solver/linear_solver2.pb.h"
#include "lp_data/lp_data.h"
#include "sat/sat_solver.h"

namespace operations_research {
namespace sat {

// Converts an integer program with only binary variables to a Boolean
// optimization problem. Returns false if the problem didn't contains only
// binary integer variable, or if the coefficients couldn't be converted to
// integer with a good enough precision.
bool ConvertBinaryMPModelProtoToBooleanProblem(
    const new_proto::MPModelProto& mp_model, LinearBooleanProblem* problem);

// Converts a Boolean optimization problem to its lp formulation.
void ConvertBooleanProblemToLinearProgram(const LinearBooleanProblem& problem,
                                          glop::LinearProgram* lp);

// Changes the variable bounds of the lp to reflect the variables that have been
// fixed by the SAT solver (i.e. assigned at decision level 0). Returns the
// number of variables fixed this way.
int FixVariablesFromSat(const SatSolver& solver, glop::LinearProgram* lp);

// Solves the given lp problem and uses the lp solution to drive the SAT solver
// polarity choices. The variable must have the same index in the solved lp
// problem and in SAT for this to make sense.
//
// Returns false if a problem occured while trying to solve the lp.
bool SolveLpAndUseSolutionForSatAssignmentPreference(
    const glop::LinearProgram& lp, SatSolver* sat_solver,
    double max_time_in_seconds);

// Solves the lp and add constraints to fix the integer variable of the lp in
// the LinearBoolean problem.
bool SolveLpAndUseIntegerVariableToStartLNS(const glop::LinearProgram& lp,
                                            LinearBooleanProblem* problem);

}  // namespace sat
}  // namespace operations_research

#endif  // OR_TOOLS_SAT_LP_UTILS_H_

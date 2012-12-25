//
// An improved second implementation.
// We reintroduce the n(n-1)/2 differences variables.
#include <vector>
#include <sstream>

#include "base/commandlineflags.h"
#include "base/logging.h"
#include "constraint_solver/constraint_solver.h"

DEFINE_int32(n, 0, "Number of marks. If 0 will test different values of n.");
DEFINE_bool(print, false, "Print solution or not?");

// kG[n] = G(n).
static const int kG[] = {
  -1, 0, 1, 3, 6, 11, 17, 25, 34, 44, 55, 72, 85,
  106, 127, 151, 177, 199, 216, 246
};

static const int kKnownSolutions = 19;

namespace operations_research {

bool next_interval(const int n,
                   const int i,
                   const int j,
                   int* next_i,
                   int* next_j)  {
  CHECK_LT(i, n);
  CHECK_LE(j, n);
  CHECK_GE(i, 1);
  CHECK_GT(j, 1);

  if (j == n) {
    if (i == n - 1) {
      return false;
    } else {
      *next_i = i + 1;
      *next_j = i + 2;
    }
  } else {
    *next_i = i;
    *next_j = j + 1;
  }

  return true;
}

void GolombRuler(const int n) {
  CHECK_GE(n, 1);

  Solver s("golomb");

  // Upper bound on G(n), only valid for n < 65 000
  CHECK_LT(n, 65000);
  const int64 max = n * n - 1;

  // Variables
  std::vector<IntVar*> X(n + 1);
  X[0] = s.MakeIntConst(-1);  // The solver doesn't allow NULL pointers
  X[1] = s.MakeIntConst(0);   // X(1) = 0

  for (int i = 2; i <= n; ++i) {
    X[i] = s.MakeIntVar(1, max, StringPrintf("X%03d", i));
  }

  std::vector<std::vector<IntVar *> >Y(n + 1, std::vector<IntVar *>(n + 1));
  for (int i = 1; i < n; ++i) {
    for (int j = i + 1; j <= n; ++j) {
    Y[i][j] = s.MakeDifference(X[j], X[i])->Var();
    Y[i][j]->SetMin(1);
    }
  }

  int k, l, next_k, next_l;
  for (int i = 1; i < n - 1; ++i) {
    for (int j = i + 1; j <= n; ++j) {
      k = i;
      l = j;
      while (next_interval(n, k, l, &next_k, &next_l)) {
        s.AddConstraint(s.MakeNonEquality(Y[i][j], Y[next_k][next_l]));
        k = next_k;
        l = next_l;
      }
    }
  }

  OptimizeVar* const length = s.MakeMinimize(X[n], 1);

  SolutionCollector* const collector = s.MakeLastSolutionCollector();
  collector->Add(X);
  DecisionBuilder* const db = s.MakePhase(X,
                                          Solver::CHOOSE_FIRST_UNBOUND,
                                          Solver::ASSIGN_MIN_VALUE);

  s.Solve(db, collector, length);  // go!
  CHECK_EQ(collector->solution_count(), 1);

  const int64 result = collector->Value(0, X[n]);
  LOG(INFO) << "G(" << n << ") = " << result;
  LOG(INFO) << "Time: " << s.wall_time()/1000.0 << " s";

  if (FLAGS_print) {
    std::ostringstream solution_str;
    solution_str << "Solution: ";
    for (int i = 1; i <= n; ++i) {
      solution_str << collector->Value(0, X[i]) << " ";
    }
    LOG(INFO) << solution_str.str();
  }

  if (n < kKnownSolutions) {
    CHECK_EQ(result, kG[n]);
  }
}

}   // namespace operations_research

int main(int argc, char **argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  if (FLAGS_n != 0) {
    operations_research::GolombRuler(FLAGS_n);
  } else {
    for (int n = 4; n < 11; ++n) {
      operations_research::GolombRuler(n);
    }
  }
  return 0;
}

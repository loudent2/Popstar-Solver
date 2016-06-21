#pragma once

#ifdef SOLVER_EXPORTS
#define SOLVER_API __declspec(dllexport)
#else
#define SOLVER_API __declspec(dllimport)
#endif


//void SolvePopStarPuzzle(PopStarSolver::BoardPiece puzzle[PopStarSolver::BoardWidth][PopStarSolver::BoardHeight]);
SOLVER_API
void SolveRandomPuzzle();

SOLVER_API
void SolveColumPuzzle();

SOLVER_API
void SolveTwifirPuzzle();

SOLVER_API
void SolveFourFerPuzzle();
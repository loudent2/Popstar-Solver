// Solver.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Solver.h"
#include "PuzzleSolution.h"
#include <memory>

void
CreateRandomPopStarBoard(PopStarSolver::BoardPiece board[PopStarSolver::BoardWidth][PopStarSolver::BoardHeight])
{
	for (int x = 0; x < PopStarSolver::BoardWidth; x++)
	{
		for (int y = 0; y < PopStarSolver::BoardHeight; y++)
		{
			board[x][y] = (PopStarSolver::BoardPiece)(rand() % (int)PopStarSolver::BoardPiece::Undefined);
		}
	}
}

void
CreateColumnPopStarBoard(PopStarSolver::BoardPiece board[PopStarSolver::BoardWidth][PopStarSolver::BoardHeight])
{
	for (int x = 0; x < PopStarSolver::BoardWidth; x++)
	{
		PopStarSolver::BoardPiece color = (PopStarSolver::BoardPiece) (x % (int)PopStarSolver::BoardPiece::Undefined);
		for (int y = 0; y < PopStarSolver::BoardHeight; y++)
		{
			board[x][y] = color;
		}

	}
}

void
CreateTwoforPopStarBoard(PopStarSolver::BoardPiece board[PopStarSolver::BoardWidth][PopStarSolver::BoardHeight])
{
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < PopStarSolver::BoardHeight; y++)
		{
			board[x][y] = PopStarSolver::BoardPiece::Red;
		}

	}
	for (int x = 5; x < PopStarSolver::BoardHeight; x++)
	{
		for (int y = 0; y < PopStarSolver::BoardHeight; y++)
		{
			board[x][y] = PopStarSolver::BoardPiece::Blue;
		}
	}
}

void
CreateFourFerPopStarBoard(PopStarSolver::BoardPiece board[PopStarSolver::BoardWidth][PopStarSolver::BoardHeight])
{
	PopStarSolver::BoardPiece color = PopStarSolver::BoardPiece::Red;
	for (int x = 0; x < PopStarSolver::BoardWidth; x++)
	{
		for (int y = 0; y < PopStarSolver::BoardHeight; y++)
		{
			board[x][y] = color;
		}

		if ((x % 4) == 0)
		{
			int n = (int) color;
			n++;
			color = (PopStarSolver::BoardPiece) n;
		}
	}
}

void SolveRandomPuzzle()
{
	PopStarSolver::BoardPiece board[PopStarSolver::BoardWidth][PopStarSolver::BoardHeight];
	CreateRandomPopStarBoard(board);
///	std::unique_ptr<PopStarSolver::PuzzleSolution> puzzleBoard(new PopStarSolver::PuzzleSolution(board));
	std::unique_ptr<PopStarSolver::PuzzleSolver> puzzleBoard(new PopStarSolver::PuzzleSolver());
	puzzleBoard->Solve(board);

}

void SolveColumPuzzle()
{
	PopStarSolver::BoardPiece board[PopStarSolver::BoardWidth][PopStarSolver::BoardHeight];
	CreateColumnPopStarBoard(board);
	std::unique_ptr<PopStarSolver::PuzzleSolver> puzzleBoard(new PopStarSolver::PuzzleSolver());
	puzzleBoard->Solve(board);
}
void SolveTwifirPuzzle()
{
	PopStarSolver::BoardPiece board[PopStarSolver::BoardWidth][PopStarSolver::BoardHeight];
	CreateTwoforPopStarBoard(board);
	std::unique_ptr<PopStarSolver::PuzzleSolver> puzzleBoard(new PopStarSolver::PuzzleSolver());
	puzzleBoard->Solve(board);
}
void SolveFourFerPuzzle()
{
	PopStarSolver::BoardPiece board[PopStarSolver::BoardWidth][PopStarSolver::BoardHeight];
	CreateFourFerPopStarBoard(board);
	std::unique_ptr<PopStarSolver::PuzzleSolver> puzzleBoard(new PopStarSolver::PuzzleSolver());
	puzzleBoard->Solve(board);
}

void SolvePopStarPuzzle(PopStarSolver::BoardPiece puzzle[PopStarSolver::BoardWidth][PopStarSolver::BoardHeight])
{
	PopStarSolver::BoardPiece board[PopStarSolver::BoardWidth][PopStarSolver::BoardHeight];
	CreateColumnPopStarBoard(board);
	std::unique_ptr<PopStarSolver::PuzzleSolution> puzzleBoard(new PopStarSolver::PuzzleSolution(board));
}


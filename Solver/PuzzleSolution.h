#pragma once

#include "PopStarBoad.h" //for PopStarBoard et al.
#include <vector>
#include "ConcurrentQueue.h"
#include <unordered_set>

namespace PopStarSolver
{
	//this class containsa solution path
	//that may or may not be incomplete. It is used
	//as part of a recursive call
	class PuzzleSolution
	{
	public:
		PuzzleSolution() =  delete;
		PuzzleSolution(PopStarBoard board)
			: m_board(board)
		{ }
		PuzzleSolution(const PuzzleSolution& other);
		PuzzleSolution(PuzzleSolution&& other);
		PuzzleSolution& operator=(const PuzzleSolution& other);

		~PuzzleSolution();
		
		//get the board
		PopStarBoard& Board()
		{
			return m_board;
		}
		unsigned int& CurrentPoints()
		{
			return m_currentPoints;
		}
		unsigned int& Level()
		{
			return m_level;
		}

		Position& NextPop()
		{
			return m_nextPop;
		}
		std::vector<PopStarSolver::Position> m_solutionPath; //**FLAg move to proctected and give access

	protected:
		unsigned int m_currentPoints = 0;
		PopStarSolver::PopStarBoard m_board;
		unsigned int m_level = 0;
		Position m_nextPop = { BoardWidth + 1, BoardHeight + 1 };
	};

	//this class uses  instances of PuzzleSolution to solve the puzzle
	//It will create a work order and put it in the Queue. This
	//work order should include the position of the item to pop
	//then it pops, adds the score and creates a new set of work orders with every
	//popable item after that
	class PuzzleSolver
	{
	public:
//		PuzzleSolver(PopStarBoard board)
//			: m_board(board)
		PuzzleSolver()
		{ };
		~PuzzleSolver()
		{ /**FLAG free solutions*/};


		void Solve(PopStarBoard board);
		
		//Initial solve implementation. This uses no 
		//threads or any performance enhancing patterns
		//this version can solve around 40k-50k solutions
		//per second in release mode
		void LinearSolver(PuzzleSolution* solution);

		//async solver. this will create a couple of
		//worker threads to solve the problems
		//as well as a sort of finalizer threadsd
		//that goes through the solutions to 
		//figure out the best ones.
		void ASyncSolver(PopStarBoard board);

	protected:
		//some multi-threaded stuff to handle when to post
		//process
		std::mutex m_mutex;
		std::condition_variable m_process;
		bool m_finished = false;

		ConcurrentUnorderd_Set<BoardBitSet> m_finishedBoards;

		//post process solutions to determine if they're better than what I have
		void ProcessSolutions();

		//current best solution
		PuzzleSolution* m_highestPoints = nullptr;
	
		//this is an output queue of solutions it goes through a posst process to evaluate
		//if its better 
		ConcurrentQueue<PuzzleSolution*> m_outputQueue;
	};
}
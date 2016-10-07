#ifdef _WINDOWS
#include "stdafx.h"
#endif

#include "PuzzleSolution.h"
#include <thread>
#include "ThreadPool.h"
#include <iostream> //**FLAG temporary  to give output to command line


namespace PopStarSolver
{
	PuzzleSolution::PuzzleSolution(const PuzzleSolution& other) 
	{
		m_solutionPath = other.m_solutionPath;
		m_currentPoints = other.m_currentPoints;
		//**flag might need a memcpy
		m_board = other.m_board;
	}
	PuzzleSolution::PuzzleSolution(PuzzleSolution&& other)
	{
		m_solutionPath = std::move(other.m_solutionPath);
		m_currentPoints = std::move(other.m_currentPoints);
		m_board = std::move(other.m_board);
	}
	PuzzleSolution&
	PuzzleSolution::operator=(const PuzzleSolution& other)
	{
		if (this != &other)
		{
			m_solutionPath = other.m_solutionPath;
			m_currentPoints = other.m_currentPoints;
			m_board = other.m_board;
		}
		return *this;
	}

	PuzzleSolution::~PuzzleSolution()
	{}

	void 
	PuzzleSolver::Solve(PopStarBoard board)
	{
		//create a queue of solutions with recomended pop locations
		//create some worker threads that will dequeue and solve.
		//finally we want to insert into the outputqueue

//		PuzzleSolution* Solution = new PuzzleSolution(board);
//		LinearSolver(Solution);
	
		ASyncSolver(board);
		
	}

	
	void
	PuzzleSolver::ASyncSolver(PopStarBoard board)
	{
		PuzzleSolution* Solution = new PuzzleSolution(board);
		
		auto task = 
			[&](PuzzleSolution* solution, Concurrency::ThreadPool<PuzzleSolution>& pool)
		{
			Position nextPop = solution->NextPop();
			if (nextPop.m_x < BoardWidth && nextPop.m_y < BoardHeight)
			{
				solution->CurrentPoints() += solution->Board().Pop(nextPop.m_x, nextPop.m_y);
				solution->m_solutionPath.push_back(Position(solution->NextPop().m_x, solution->NextPop().m_y));
			}
			PieceMap matches;
			solution->Board().GetPoppableItems(matches);
			//if this is the end of this line of pops then add to solutions and return
			if (0 >= matches.size())
			{
				PuzzleSolution* processSolution = new PuzzleSolution(*solution);
				processSolution->CurrentPoints() += processSolution->Board().AddBonusPoints();
				m_outputQueue.Push(processSolution);
				m_process.notify_one();
			}

			//if we've already processed this board (and it's not finished)
			//and the score is lower than when we got here then skip
			unsigned int tempScore;
			if (true == m_finishedBoards.Find(solution->Board().BitSet(), tempScore)
				&&  tempScore >= solution->CurrentPoints())
			{
				delete solution; solution = nullptr;
				return;
			}		
			m_finishedBoards.Insert(solution->Board().BitSet(), solution->CurrentPoints());
			
			for (auto pos : matches)
			{
				PuzzleSolution* newSolution = new PuzzleSolution(*solution);
				newSolution->NextPop() = pos;
				newSolution->Level() += 1;
				pool.Enqueue(newSolution);
			}

			delete solution; solution = nullptr;
		};
		
		Concurrency::ThreadPool<PuzzleSolution> pool(std::thread::hardware_concurrency(), task);
		pool.Enqueue(Solution);
		std::thread processing(&PuzzleSolver::ProcessSolutions, this);

		{   // acquire lock
			std::mutex mutex;
			std::unique_lock<std::mutex> lock(mutex);
			pool.workIsDone.wait(lock);
		}
		m_finished = true;
		m_process.notify_one();
		if (processing.joinable())
			processing.join();
		if (nullptr == m_highestPoints)
			return; 
	}

	void 
	PuzzleSolver::LinearSolver(PuzzleSolution* solution)
	{
		Position nextPop = solution->NextPop();
		if (nextPop.m_x < BoardWidth && nextPop.m_y < BoardHeight)
		{
			solution->CurrentPoints() += solution->Board().Pop(nextPop.m_x, nextPop.m_y);
			solution->m_solutionPath.push_back(Position(solution->NextPop().m_x, solution->NextPop().m_y));
		}
		PieceMap matches;
		solution->Board().GetPoppableItems(matches);
		//if this is the end of this line of pops then add to solutions and return
		if (0 >= matches.size())
		{
			PuzzleSolution* processSolution = new PuzzleSolution(*solution);
			m_outputQueue.Push(processSolution);
			m_process.notify_one();

		}
		for (auto pos : matches)
		{
			PuzzleSolution* newSolution = new PuzzleSolution(*solution);
			newSolution->NextPop() = pos;
			newSolution->Level() += 1;
			LinearSolver(newSolution);
		}
		delete solution; solution = nullptr;

	}

	void PuzzleSolver::ProcessSolutions()
	{
		size_t numProcessed = 0;
		std::unique_lock<std::mutex> lock(m_mutex);

		while(true !=  m_finished
		 || m_outputQueue.Size() > 0)
		{
			if (m_outputQueue.Size() <= 0)
			{
//	**FLAG			m_process.wait(lock, [&]() {return !(m_finished && 0 >= m_outputQueue.Size()); })
				m_process.wait(lock);
			}
			else
			{
				//**FLAG Remove after evaluation
				numProcessed++;
				if (0 == numProcessed % 1000000) std::cout << numProcessed << " Processed. (" << m_finishedBoards.Size() << " in skip queue)" << std::endl;

				PuzzleSolution* solution = nullptr;
				m_outputQueue.Pop(solution);
				if (nullptr == solution)
				{					
					continue;
				}
				if (nullptr == m_highestPoints)
				{	
					m_highestPoints = new PuzzleSolution(*solution);
					std::cout << "New Best: Points: [" << m_highestPoints->CurrentPoints() << "], Pops [" << m_highestPoints->m_solutionPath.size() << "] Remainging: [" << m_highestPoints->Board().GetRemainingPieces() << "]" << std::endl;
				}
				else
				if ((solution->CurrentPoints() > m_highestPoints->CurrentPoints())
					|| ((solution->CurrentPoints() == m_highestPoints->CurrentPoints()) && (solution->m_solutionPath.size() < m_highestPoints->m_solutionPath.size())))
				{
					*m_highestPoints = *solution;
					std::cout << "New Best: Points: [" << m_highestPoints->CurrentPoints() << "], Pops [" << m_highestPoints->m_solutionPath.size() << "] Remainging: [" << m_highestPoints->Board().GetRemainingPieces() << "]" << std::endl;
				}
				delete solution; solution = nullptr;
			}
		}
	}


}
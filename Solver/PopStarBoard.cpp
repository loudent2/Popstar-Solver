#ifdef _WINDOWS
#include "stdafx.h"
#endif

#include "PopStarBoad.h"
#include <algorithm>
#include <queue>
#include <memory>
#include <cstring>
#include <assert.h>

//implementation of the popstar board class
namespace PopStarSolver
{

	PopStarBoard::PopStarBoard()
	{
		ClearBoard();
		m_bitSet.reset();
	}
	
	PopStarBoard::PopStarBoard(const PopStarBoard& other)
	{
		std::memcpy(m_board, other.m_board, sizeof(BoardPiece) * BoardWidth * BoardHeight);
		m_bitSet = other.m_bitSet;

	}
	PopStarBoard::PopStarBoard(PopStarBoard&& other)
	{
		std::memcpy(m_board, other.m_board, sizeof(BoardPiece) * BoardWidth * BoardHeight);
		m_bitSet = other.m_bitSet;
	}

	PopStarBoard::PopStarBoard(BoardPiece board[BoardHeight][BoardWidth])
	{
		std::memcpy(m_board, board, sizeof(BoardPiece) * BoardWidth * BoardHeight);
		CreateBitSet();
	}

	PopStarBoard& 
	PopStarBoard::operator=(const PopStarBoard& other)
	{
		if (this != &other)
		{
			std::memcpy(m_board, other.m_board, sizeof(BoardPiece) * BoardWidth * BoardHeight);
			m_bitSet = other.m_bitSet;
		}
		return *this;
	}


	PopStarBoard::~PopStarBoard()
	{
	}

	// Couple of methods to return the board at a particular position
	BoardPiece 
	PopStarBoard::get(unsigned int x, unsigned int y)
	{
		if (x >= BoardWidth || y >= BoardHeight) //don't need to check less than 0 since unsigned
		{
			return 	BoardPiece::Undefined;
		}
		return m_board[x][y];
	}
	BoardPiece 
	PopStarBoard::get(unsigned int position)
	{
		unsigned int x = position / BoardWidth;
		unsigned int y = position % BoardWidth;
		return get(x, y);
	}

	void PopStarBoard::ClearBoard()
	{
		//**flag perhaps it would be faster to just 0 out everything
		for (unsigned int i = 0; i < BoardWidth; i++)
		{
			for (unsigned int j = 0; j < BoardHeight; j++)
			{
				m_board[i][j] = BoardPiece::Undefined;
			}
		}
		m_bitSet.reset();
	}
	//pops the piece at position and returns
	//the number of points
	unsigned int
	PopStarBoard::Pop(unsigned int x, unsigned int y)
	{
		PieceMap matches;
		unsigned int nCount = MatchingConnectors(x, y, matches);
		for (auto pos : matches)
		{
			m_board[pos.m_x][pos.m_y] = BoardPiece::Undefined;
		}
		CollapseBoard();
		CreateBitSet();
		//generate points
		auto retPoint = [&](unsigned int nCount)->unsigned int
		{	unsigned int AddAmount = 25;
			const unsigned int IncreaseAmmount = 10;
			unsigned int trackCount = (nCount > 1)?20:0;
			for (; nCount > 2; nCount--)
			{
				trackCount += AddAmount;
				AddAmount += IncreaseAmmount;
			}
			return trackCount;
		};

		return retPoint(nCount);
	}

	//get a collection of poppable chunks. Note that this isn't
	//all possible poppable locations just locations that create "unique"
	//poppalbe chunk (e.g. if 1, 1 is red and 1, 2 is red then both 1,1 and 1,2 are
	//poppbable but it will only return 1,1 since it's the same chunk as 2,2
	unsigned int 
	PopStarBoard::GetPoppableItems(PieceMap& poppables)
	{
		PieceMap matches;
		for (unsigned int x = 0; x < BoardWidth; x++)
		{
			for (unsigned int y = 0; y < BoardHeight; y++)
			{
				//if we haven't already matched this one
				if (FindPosition(matches, Position(x, y)))
				{
					continue;
				}
				//ok, not in the list of matches
				//keeping a running list of matches makes the check confused
				int nCount = MatchingConnectors(x, y, matches); //**FLAG during profiling see what hte cost is to ensure only one match of each x,y value is added
				if (nCount > 1)
				{
					AddPosition(poppables, Position(x, y));
				}
			}
		}
		return poppables.size();
	}

	size_t 
	PopStarBoard::GetRemainingPieces()
	{
		size_t count = 0;
		for (unsigned int x = 0; x < BoardWidth; x++)
		{
			for (unsigned int y = 0; y < BoardHeight; y++)
			{
				if (m_board[x][y] != BoardPiece::Undefined)
				{
					count++;
				}
			}
		}
		
		return count;
	}

	//returns true if the column is empty 
	bool 
	PopStarBoard::EmptyColumn(unsigned int x)
	{
		for (unsigned int y = 0; y < BoardHeight; y++)
		{
			if (BoardPiece::Undefined != m_board[x][y])
			{
				return false;
			}
		}
		return true;
	}

	void 
	PopStarBoard::SwapColumn(unsigned int left, unsigned int right)
	{
		for (unsigned int y = 0; y < BoardHeight; y++)
		{
			std::swap(m_board[left][y], m_board[right][y]);
		}
	}
	//board collapses from left to right and top to bottom
	void 
	PopStarBoard::CollapseBoard()
	{
		//first collapse top to bottom
		for (unsigned int i = 0; i < BoardWidth; i++)
		{
			for (unsigned int j = 1; j < BoardHeight; j++)
			{
				if (BoardPiece::Undefined != m_board[i][j])
				{
					unsigned tempj = j;
					while (tempj > 0 && BoardPiece::Undefined == m_board[i][tempj - 1])
					{
						std::swap(m_board[i][j], m_board[i][tempj - 1]);
						tempj--;
					}
				}

			}
		}

		//now collapse left to right
		for (unsigned int x = 1; x < BoardWidth; x++)
		{
			if (EmptyColumn(x - 1) && !EmptyColumn(x))
			{
				unsigned tempx = x;
				while (tempx > 0 && EmptyColumn(tempx - 1))
				{
					SwapColumn(tempx, tempx - 1);
					tempx--;
				}
			}
		}
	}
	//returns the number of mmatching connected pieces as well as a collection of those matches
	unsigned int 
	PopStarBoard::MatchingConnectors(unsigned int x, unsigned int y, /*out*/PieceMap& matches)
	{
		if (x >= BoardWidth || y >= BoardHeight			
			|| BoardPiece::Undefined == m_board[x][y])
		{
			return 0;
		}
		std::queue<Position> line;
		line.push(Position(x, y));

		return MatchColor(line, matches);
	}

	unsigned int 
	PopStarBoard::MatchColor(std::queue<Position>& line, /*out*/PieceMap& matches)
	{
		if (line.empty())
		{
			return 0;
		}
		int nRetValue = 0;
		auto MatchValue = [&](unsigned int x, unsigned int y, unsigned int x2, unsigned int y2)
		{
			if (m_board[x][y] == m_board[x2][y2])
			{
				//if we haven't already matched this one
				if(!FindPosition(matches, Position(x2, y2)))
				{
					//add it to the queue to process;
					line.push(Position(x2, y2));
				}
			}
		};

		while (!line.empty())
		{
			Position frontItem = line.front();
			line.pop();
			//check if it's the matches list already
			if (FindPosition(matches, frontItem))
			{
				continue;
			}

			if (frontItem.m_x < BoardWidth - 1)
			{
				MatchValue(frontItem.m_x, frontItem.m_y, frontItem.m_x + 1, frontItem.m_y);
			}
			if (frontItem.m_x > 0)
			{
				MatchValue(frontItem.m_x, frontItem.m_y, frontItem.m_x - 1, frontItem.m_y);
			}
			if (frontItem.m_y < BoardHeight - 1)
			{
				MatchValue(frontItem.m_x, frontItem.m_y, frontItem.m_x, frontItem.m_y + 1);
			}
			if (frontItem.m_y > 0)
			{
				MatchValue(frontItem.m_x, frontItem.m_y, frontItem.m_x, frontItem.m_y - 1);
			}

			nRetValue++;
			AddPosition(matches, frontItem);
		}
		

		return nRetValue;;
	}

	void SolvePuzzle()
	{
		//in: current score, current puzzle
		//iterate through each column and row
		//and build a list of poppable stars
		//for each popable
		   //pop - add scroe
			//call solve puzzle for that.
	}
}
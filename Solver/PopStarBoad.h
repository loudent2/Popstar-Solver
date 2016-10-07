#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include <map>
#include <bitset>
#include <algorithm>

//**FLAG #include <hash>

//the pop star board is a, generallyl 10x10  board comprising on of 4 colors
//the pieces can connect horizontally and vertically but not diagnoally. You
//must have at least 2 next to each other to pop them and the larger the group the more 
//points you get. In addition there are bonus points if you have less than (iirc 8)

namespace PopStarSolver
{
	using BoardBitSet = std::bitset<300>;

	constexpr unsigned int BoardHeight = 10;
	constexpr unsigned int BoardWidth = 10;
	enum class BoardPiece { Red = 0b0, Yellow = 0b1, Purple = 0b10, Blue = 0b11, Undefined = 0b100 };
	//**FLAG harder
	using BoardRepresentation = BoardPiece[BoardHeight][BoardWidth];

	//simple struct to hold position
	struct Position 
	{
	public:
		Position(unsigned int x, unsigned int y)
			: m_x(x), m_y(y)
		{}
		Position& operator=(const Position& other)
		{
			m_x = std::move(other.m_x);
			m_y = std::move(other.m_y);
			return *this;
		}
		bool operator==(const Position& other)
		{
			return(m_x == other.m_x
				&&	m_y == other.m_y);
		}
		//in order to improve efficiency I'm
		//switching to an unordered map. I need
		//a key for positions. Since  (at least
		//initially) I'm using a 10x10 board 
		//using the piece number should suffice
		unsigned int Key()
		{
			return (m_x * BoardHeight) + m_y;
		}

		unsigned int m_x;
		unsigned int m_y;
	};
	//I'm not sure what is the most efficient way to store matches. using a 
	//vector and std::find is costing me quite a bit.  But considering the number
	//of insertions and the fact that I have to iterate over them
	//I can't decide between map and unordered_map. so I'm 
	//defining the type here and the lookup.
	using PieceMap = std::vector<Position>;

	class PopStarBoard
	{
	public:
		//There's going to be a lot of copying of boards and each search branch will need the current board
		//plus the history of pops;
		PopStarBoard();
		PopStarBoard(const PopStarBoard& other);
		PopStarBoard(PopStarBoard&& other);
		PopStarBoard(BoardPiece board[BoardHeight][BoardWidth]);
		PopStarBoard& operator=(const PopStarBoard& other);
		~PopStarBoard();

		//this hash is created from the bitset representation
		size_t Key()
		{
			std::hash<BoardBitSet> boardHash;
			return boardHash(m_bitSet);
		}

		BoardBitSet BitSet()
		{
			return m_bitSet;
		}


		// Couple of methods to return the board at a particular position
		BoardPiece get(unsigned int x, unsigned int y);
		BoardPiece get(unsigned int position);

		//pops the piece at position and returns
		//the number of points
		unsigned int Pop(unsigned int x, unsigned int y);
		unsigned int GetPoppableItems(PieceMap& poppables);

		size_t GetRemainingPieces();

		inline unsigned int AddBonusPoints()
		{
			size_t pieces = GetRemainingPieces();
			if (pieces < 10)
			{
				switch (pieces)
				{
				case 0:
					return 2000;
					break;
				case 1:
					return 1980;
					break;
				case 2:
					return 1920;
					break;
				case 3:
					return 1820;
					break;
				case 4:
					return 1680;
					break;
				case 5:
					return 1500;
					break;
				case 6:
					return 1280;
					break;
				case 7:
					return 1020;
					break;
				case 8:
					return 720;
					break;
				case 9:
					return 370;
					break;
				}
			}
			return 0;
		}
			

	protected:
		BoardBitSet CreateBitSet()
		{
			m_bitSet.reset();
			for (unsigned int x = 0; x < BoardWidth; x++)
			{
				for (unsigned int y = 0; y < BoardHeight; y++)
				{
					m_bitSet <<= 3;
					m_bitSet |= static_cast<char>(m_board[x][y]);
				}
			}
			return m_bitSet;
		}

		inline bool FindPosition(PieceMap& map, Position Pos)
		{
			return (std::find(map.begin(), map.end(), Pos) != map.end());

		}
		inline  void AddPosition (PieceMap& map, Position Pos)
		{
			map.push_back(Pos);
		}

		//returns true if the column is empty 
		bool EmptyColumn(unsigned int x);
		
		void SwapColumn(unsigned int left, unsigned int right);

		//board collapses from left to right and top to bottom
		void CollapseBoard();

		//returns the number of matching the number of connectors
		unsigned int MatchingConnectors(unsigned int x, unsigned int y, /*out*/PieceMap& matches);
		//starts with the item in the first postion of the line and continues until
		//all matching connected colors are in the list
		unsigned int MatchColor(std::queue<Position>& line, /*out*/PieceMap& matches);


		//**FLAG perhaps the internal storage should be the bitset
		//**FLAG I've had some  great performance from vectors. Perhaps
		//I should use them instead of arrays
		BoardPiece m_board[BoardHeight][BoardWidth];

		//I'm thinking it might be faster to be able to store board setups
		//in a compressed format in order to not evaluate boards that have already
		//been evaluated. Because there could be literally billions of combinations
		//I'm storing it in the smallest chunk possible.
		BoardBitSet m_bitSet;

		// call to create clear board
		void ClearBoard();

	};
}
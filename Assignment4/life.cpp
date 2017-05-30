/*
*	life.cpp
*	Created by:
*	Group 11:	Andreas Sjöberg, Oscar Falkman.
**/

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <iostream>
#include <string>
#include <sstream>

using namespace Gecode;
using namespace std;

class GameOfLife : public Script {
public:
	int m;
	// Array representing the matrix.
	BoolVarArray q;
	// Matrix representing the board
	GameOfLife(const SizeOptions& opt) : Script(opt), m(opt.size() + 4), q(*this, m * m, 0, 1) {
		const int n = q.size();
		
		Matrix<BoolVarArgs> mat(q, m, m);

		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < m; ++j) {
				if(i < 2 || j < 2 || i > m - 3 || j > m - 3)
				rel(*this, mat(i, j), IRT_EQ, 0);
			}
		}

		for (int i = 2; i < m - 2; ++i) {
			for (int j = 2; j < m - 2; ++j) {
				//if(!mat(i, j).assigned())
					//rel(*this, mat(i, j), IRT_EQ, 1);
				if (mat(i, j).assigned() && mat(i, j).val() == 1)  {
					rel(*this, mat(i - 1, j + 1) + mat(i, j + 1) + mat(i + 1, j + 1) + mat(i - 1, j - 1) + mat(i, j - 1) + mat(i + 1, j - 1) + mat(i - 1, j) + mat(i + 1, j) == 3);
					//rel(*this, mat(i - 1, j + 1) + mat(i, j + 1) + mat(i + 1, j + 1) + mat(i - 1, j - 1) + mat(i, j - 1) + mat(i + 1, j - 1) + mat(i - 1, j) + mat(i + 1, j) <= 3);
				} else if(mat(i, j).assigned() && mat(i, j).val() == 0) {
					rel(*this, mat(i - 1, j + 1) + mat(i, j + 1) + mat(i + 1, j + 1) + mat(i - 1, j - 1) + mat(i, j - 1) + mat(i + 1, j - 1) + mat(i - 1, j) + mat(i + 1, j) != 3);
				}
			}
		}

		branch(*this, q, INT_VAR_SIZE_MIN(), INT_VAL_MED());
		//print(cout);
	}

	// Constructor for cloning \a s
	GameOfLife(bool share, GameOfLife& s) : Script(share, s) {
		q.update(*this, share, s.q);
	}

	// Perform copying during cloning
	virtual Space*
		copy(bool share) {
		return new GameOfLife(share, *this);
	}

	// Print solution
	virtual void
		print(std::ostream& os) const {
		os << "GameOfLife\n\t";
		for (int i = 0; i < q.size(); i++) {
			os << q[i] << ",";
			if ((i + 1) % m == 0)
				os << std::endl << "\t";
		}
		os << std::endl;
	}
};

int main(int argc, char* argv[]) {
	bool hello = true;
	while (hello) {
		SizeOptions opt("GameOfLife");
		cout << "Input the size you want for the matrix:\t 0 to quit." << endl;
		opt.iterations(500);
		int number = 0;
		string input = "";
		getline(cin, input);
		stringstream sInput(input);
		sInput >> number;
		if (number == 0)
			break;
		opt.size(number);
		opt.parse(argc, argv);
		Script::run<GameOfLife, DFS, SizeOptions>(opt);
	}
	return 0;
}
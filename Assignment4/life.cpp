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
	IntVar matSize;
	// Array representing the matrix.
	BoolVarArray q;
	// Matrix representing the board
	GameOfLife(const SizeOptions& opt) : Script(opt), matSize(*this, opt.size() + 4, opt.size() + 4), q(*this, matSize.val() * matSize.val(), 0, 1) {
		const int n = q.size();
		
		Matrix<BoolVarArgs> mat(q, matSize.val(), matSize.val());

		for (int i = 0; i < matSize.val(); ++i) {
			for (int j = 0; j < matSize.val(); ++j) {
				if(i < 2 || j < 2 || i > matSize.val() - 3 || j > matSize.val() - 3)
				rel(*this, mat(i, j), IRT_EQ, 0);
			}
		}

		for (int j = 2; j < matSize.val() - 2; ++j) {
			for (int i = 2; i < matSize.val() - 2; ++i) {
				rel(*this, mat(i - 1, j + 1) + mat(i, j + 1) + mat(i + 1, j + 1) + mat(i - 1, j - 1) + mat(i, j - 1) + mat(i + 1, j - 1) + mat(i - 1, j) + mat(i + 1, j) >= 2);
				rel(*this, mat(i - 1, j + 1) + mat(i, j + 1) + mat(i + 1, j + 1) + mat(i - 1, j - 1) + mat(i, j - 1) + mat(i + 1, j - 1) + mat(i - 1, j) + mat(i + 1, j) <= 3);
				if (mat(i, j).assigned() && mat(i, j).val() == 0)
					rel(*this, mat(i - 1, j + 1) + mat(i, j + 1) + mat(i + 1, j + 1) + mat(i - 1, j - 1) + mat(i, j - 1) + mat(i + 1, j - 1) + mat(i - 1, j) + mat(i + 1, j) != 3);
			}
		}
		print(cout);
		branch(*this, q, INT_VAR_SIZE_MAX(), INT_VAL_MAX());
	}

	// Constructor for cloning \a s
	GameOfLife(bool share, GameOfLife& s) : Script(share, s) {
		q.update(*this, share, s.q);
		matSize.update(*this, share, s.matSize);
	}

	// Perform copying during cloning
	virtual Space* copy(bool share) {
		return new GameOfLife(share, *this);
	}

	virtual void constrain(const Space& _b) {
		const GameOfLife& b = static_cast<const GameOfLife&>(_b);
		int currentLargest = 0;
		for (int i = 0; i < b.q.size(); i++) {
			currentLargest += b.q[i].val();
		}
		int smaller = 0;
		for (int i = 0; i < b.q.size(); i++) {
			smaller += q[i].val();
		}
		linear(*this, q, IRT_GR, currentLargest);
	}

	virtual void print(std::ostream& os) const {
		os << "GameOfLife\n\t";
		Matrix<BoolVarArgs> mat(q, matSize.val(), matSize.val());
		for (int i = 2; i < matSize.val(); i++) {
			for (int j = 2; j < matSize.val(); ++j) {
				if (!(i < 2 || j < 2 || i > matSize.val() - 3 || j > matSize.val() - 3))
					os << mat(i, j) << ",";
				if (j == matSize.val() - 2)
					os << std::endl << "\t";
			}
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
		Script::run<GameOfLife, BAB, SizeOptions>(opt);
	}
	return 0;
}
/*
*	life.cpp
*	Created by:
*	Group 11:	Andreas Sjöberg, Oscar Falkman.
**/

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

using namespace Gecode;

class GameOfLife : public Script {
public:
	// Position of queens on boards
	IntVarArray q;
	// The actual problem
	GameOfLife(const SizeOptions& opt) : Script(opt), q(*this, opt.size(), 0, opt.size() - 1) {
		const int n = q.size();
		

		branch(*this, q, INT_VAR_SIZE_MIN(), INT_VAL_MED());
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
		os << "GameOfLife\t";
		for (int i = 0; i < q.size(); i++) {
			os << q[i] << ", ";
			if ((i + 1) % 10 == 0)
				os << std::endl << "\t";
		}
		os << std::endl;
	}
};

int main(int argc, char* argv[]) {
	SizeOptions opt("Queens");
	opt.iterations(500);
	opt.size(20);
	opt.parse(argc, argv);
	Script::run<GameOfLife, DFS, SizeOptions>(opt);
	return 0;
}
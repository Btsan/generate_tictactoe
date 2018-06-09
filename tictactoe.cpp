#include <iostream>
#include <vector>
#include <set>

using namespace std;

void generate(vector<short>&, const short);

vector< vector<short> > boards;
const short num_dim = 9; //9 + 5;

int main(int argc, char const *argv[])
{
	vector<short> board(num_dim, 0);
	generate(board, 1);
	static set< vector<short> > s(boards.begin(), boards.end());
	boards.assign(s.begin(), s.end());
	s.clear();
	cout << boards.size() << ', ' << boards[0].size() << endl;
	for (unsigned long i = 0; i < boards.size(); ++i)
	{
		cout << boards[i][0];
		for (unsigned short j = 1; j < boards[i].size(); ++j)
			cout << ' ' << boards[i][j];
		cout << endl;
	}
	boards.clear();
	return 0;
}

void check_line(const vector< vector<short> >& b, vector<short>& m, const short x1, const short y1, const short x2, const short y2, const short x3, const short y3)
{
	if (b[x1][y1] || b[x2][y2] || b[x3][y3])
	{
		if (b[x1][y1] && b[x2][y2] && b[x3][y3] && (b[x2][y2] == b[x1][y1]) && (b[x2][y2] == b[x3][y3]))
			if (b[x2][y2] > 0)
			{
				m[0] += 1;
				m[2] += 1;
				if (!m[4]) m[4] = 1;
			}
			else
			{
				m[1] += 1;
				m[3] += 1;
				if (!m[4]) m[4] = -1;
			}
		else if (b[x2][y2] && b[x1][y1] && (b[x2][y2] == b[x1][y1]) && (b[x3][y3] == 0))
			if (b[x2][y2] > 0)
			{
				m[0] += 1;
				m[2] += 1;
			}
			else
			{
				m[1] += 1;
				m[3] += 1;
			}
		else if (b[x2][y2] && b[x3][y3] && (b[x2][y2] == b[x3][y3]) && (b[x1][y1] == 0))
			if (b[x2][y2] > 0)
			{
				m[0] += 1;
				m[2] += 1;
			}
			else
			{
				m[1] += 1;
				m[3] += 1;
			}
		else if (b[x1][y1] && b[x3][y3] && (b[x1][y1] == b[x3][y3]) && (b[x2][y2] == 0))
			if (b[x1][y1] > 0)
			{
				m[0] += 1;
				m[2] += 1;
			}
			else
			{
				m[1] += 1;
				m[3] += 1;
			}
		else if ((b[x1][y1] == b[x2][y2] && !b[x2][y2]) || (b[x2][y2] == b[x3][y3] && !b[x3][y3]) || (b[x1][y1] == b[x3][y3] && !b[x1][y1]))
			if ((b[x1][y1] + b[x2][y2] + b[x3][y3]) == 1)
				m[0] += 1;
			else
				m[1] += 1;
	}
}

void generate(vector<short>& b, const short turn)
{
	vector<short> metrics(5, 0); // heuristic metric: # possible lines that can form from existing marks, # of those lines with 2 or more marks, game winner
	vector< vector<short> > board(3, vector<short>(3, 0));
	for (unsigned short i = 0; i < 3; ++i)
	{
		const unsigned short ii = i*3;
		for (unsigned short j = 0; j < 3; ++j)
			if (b[ii + j])
				board[i][j] = b[ii + j];
	}

	for (unsigned short i = 0; i < 3; ++i)
	{
		check_line(board, metrics, i, 0, i, 1, i, 2);
		check_line(board, metrics, 0, i, 1, i, 2, i);
	}
	check_line(board, metrics, 0, 0, 1, 1, 2, 2);
	check_line(board, metrics, 0, 2, 1, 1, 2, 0);

	// b[9] = metrics[0];
	// b[10] = metrics[1];
	// b[11] = metrics[2];
	// b[12] = metrics[3];
	// b[13] = metrics[4];
	boards.push_back(b);
	if (metrics[4] || (b[0] && b[1] && b[2] && b[3] && b[4] && b[5] && b[6] && b[7] && b[8])) return;
	else
		for (unsigned short i = 0; i < 9; ++i)
			if (b[i] == 0)
			{
				b[i] = turn;
				generate(b, -turn);
				b[i] = 0;
			}
}


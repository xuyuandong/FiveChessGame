#pragma once

const int Rows = 15;
const int Cols = 15;
const int Poss = 664;//2N(N-4)+2[ (N-2)(N-3)+(N-4) ]
const int DEAD = -1;

class ChessInfo
{
private:
	//人的棋局
	bool pTable[Rows][Cols][Poss];
	//电脑棋局
	bool cTable[Rows][Cols][Poss];
	//人的棋局权重
	int pWeight[Poss];
	//电脑棋局权重
	int cWeight[Poss];
	//人的 棋局分数
	int pScore[Rows][Cols];
	//电脑棋局分数
	int cScore[Rows][Cols];
	//棋盘矩阵
	int chessMatrix[Rows][Cols];
	//

public:
	ChessInfo();
	~ChessInfo();
	void Initialize();
	int UpdateAfterPerson(int x, int y);
	int UpdateAfterComputer(int x, int y);
	void CaculateWeight();
	void ComputerSelect(int &x, int &y);
	int CheckPWin();
	int CheckCWin();
	void GetWinPos(int tag, int* a, int* b, const char* player);

};
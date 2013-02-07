#pragma once

const int Rows = 15;
const int Cols = 15;
const int Poss = 664;//2N(N-4)+2[ (N-2)(N-3)+(N-4) ]
const int DEAD = -1;

class ChessInfo
{
private:
	//�˵����
	bool pTable[Rows][Cols][Poss];
	//�������
	bool cTable[Rows][Cols][Poss];
	//�˵����Ȩ��
	int pWeight[Poss];
	//�������Ȩ��
	int cWeight[Poss];
	//�˵� ��ַ���
	int pScore[Rows][Cols];
	//������ַ���
	int cScore[Rows][Cols];
	//���̾���
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
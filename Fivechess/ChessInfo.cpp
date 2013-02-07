#include "stdafx.h"
#include "ChessInfo.h"

enum{
	W1 = 5,
	W2 = 50,
	W3 = 100,
	W4 = 400
}RankWeight;

enum{
	EMPTY,
	BLACK,
	WHITE
}ChessMan;

ChessInfo::ChessInfo()
{}

ChessInfo::~ChessInfo()
{}

void ChessInfo::Initialize()
{
	//初始化棋盘矩阵
	for(int i=0;i<Rows;i++)
	{
		for(int j=0;j<Cols;j++)
		{
			chessMatrix[i][j] = EMPTY;
		}
	}

	//初始化棋局表
	for(int i=0;i<Rows;i++)
	{
		for(int j=0;j<Cols;j++)
		{
			for(int k=0;k<Poss;k++)
			{
				pTable[i][j][k] = false;
				cTable[i][j][k] = false;
			}
		}
	}
	int count = 0;
	// 初始化水平组合
	for(int i=0;i<Rows;i++)
	{
		for(int j=0;j<Cols-4;j++)
		{
			for(int k=0;k<5;k++)
			{
				pTable[i][j+k][count] = true;
				cTable[i][j+k][count] = true;
			}
			count++;
		}
	}
	// 初始化垂直组合
	for(int i=0;i<Rows-4;i++)
	{
		for(int j=0;j<Cols;j++)
		{
			for(int k=0;k<5;k++)
			{
				pTable[i+k][j][count] = true;
				cTable[i+k][j][count] = true;
			}
			count++;
		}
	}
	//初始化左上右下组合
	for(int i=0; i<Rows-4;i++)
	{
		for(int j=0;j<Cols-4;j++)
		{
			for(int k=0;k<5;k++)
			{
				pTable[i+k][j+k][count] = true;
				cTable[i+k][j+k][count] = true;
			}
			count++;
		}
	}
	// 初始化左下右上组合
	for(int i=0;i<Rows-4;i++)
	{
		for(int j=Cols-1;j>4;j--)
		{
			for(int k=0;k<5;k++)
			{
				pTable[i+k][j-k][count]++;
				cTable[i+k][j-k][count]++;
			}
			count++;
		}
	}

	//初始化权重表
	for(int i=0;i<Poss;i++)
	{
		pWeight[i] = 0;
		cWeight[i] = 0;
	}

}

//after human player place a chessman at (x,y), update the chess table and weight
int ChessInfo::UpdateAfterPerson(int x, int y)
{
	if( (x<0)||(x>Rows-1)||(y<0)||(y>Cols-1)||(chessMatrix[x][y]!=EMPTY) )
		return 0;

	chessMatrix[x][y] = BLACK;

	for(int i=0;i<Poss;i++)
	{
		if( (pTable[x][y][i]==true)&&(pWeight[i]!=DEAD) )
			pWeight[i]++;
		if( cTable[x][y][i]==true )
		{
			cTable[x][y][i] = false;
			cWeight[i] = DEAD;
		}
	}
	return 1;
}

//after computer place a chessman at (x,y), update the chess table and weight
int ChessInfo::UpdateAfterComputer(int x, int y)
{
	if( (x<0)||(x>Rows-1)||(y<0)||(y>Cols-1)||(chessMatrix[x][y]!=EMPTY) )
		return 0;

	chessMatrix[x][y] = WHITE;

	for(int i=0;i<Poss;i++)
	{
		if( (cTable[x][y][i]==true)&&(cWeight[i]!=DEAD) )
			cWeight[i]++;
		if( pTable[x][y][i]==true )
		{
			pTable[x][y][i] = false;
			pWeight[i] = DEAD;
		}
	}
	return 1;
}

void ChessInfo::CaculateWeight()
{
	//human player
	for(int i=0; i<Rows; i++)
	{
		for(int j=0;j<Cols;j++)
		{
			pScore[i][j] = 0;
			if( chessMatrix[i][j]==EMPTY )
			{
				for(int k=0;k<Poss;k++)
				{
					if(pTable[i][j][k]==true)
					{
						switch( pWeight[k] )
						{
						case 1:
							pScore[i][j] += W1;	break;
						case 2:
							pScore[i][j] += W2;	break;
						case 3:
							pScore[i][j] += W3;	break;
						case 4:
							pScore[i][j] += W4;	break;
						default:
							break;
						}
					}
				}
			}
		}
	}
				
	//computer
	for(int i=0; i<Rows; i++)
	{
		for(int j=0;j<Cols;j++)
		{
			cScore[i][j] = 0;
			if( chessMatrix[i][j]==EMPTY )
			{
				for(int k=0;k<Poss;k++)
				{
					if(cTable[i][j][k]==true)
					{
						switch( cWeight[k] )
						{
						case 1:
							cScore[i][j] += W1;	break;
						case 2:
							cScore[i][j] += W2;	break;
						case 3:
							cScore[i][j] += W3;	break;
						case 4:
							cScore[i][j] += W4;	break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}

void ChessInfo::ComputerSelect(int &x, int &y)
{
	int maxdefense = -1;
	int maxattack = -1;
	int xdefense, ydefense;
	int xattack, yattack;

	for(int i=0;i<Rows;i++)
	{
		for(int j=0;j<Cols;j++)
		{
			if( chessMatrix[i][j]==EMPTY )
			{
				if( maxdefense<pScore[i][j] )
				{
					maxdefense = pScore[i][j];
					xdefense = i;
					ydefense = j;
				}
				if( maxattack<cScore[i][j] )
				{
					maxattack = cScore[i][j];
					xattack = i;
					yattack = j;
				}
			}
		}
	}
	if(maxattack>=maxdefense)
	{
		x = xattack;
		y = yattack;
	}else{
		x = xdefense;
		y = ydefense;
	}

	return ;
}

int ChessInfo::CheckPWin()
{
	int pWin = -1;
	for(int i=0;i<Poss;i++)
	{
		if(pWeight[i]==5)
		{
			pWin = i;
			break;
		}
	}
	return pWin;
}

int ChessInfo::CheckCWin()
{
	int cWin = -1;
	for(int i=0;i<Poss;i++)
	{
		if(cWeight[i]==5)
		{
			cWin = i;
			break;
		}
	}
	return cWin;
}

void ChessInfo::GetWinPos(int tag, int *a, int *b, const char* player)
{
	int count = 0;
	if( !strcmp(player,"computer") )
	{
		for(int i=0;i<Rows;i++)
		{
			for(int j=0;j<Cols;j++)
			{
				if( cTable[i][j][tag]==true )
				{
					a[count] = i;
					b[count] = j;
					count++;
				}
				if(count==5) return;
			}
		}
	}
	else
	{
		for(int i=0;i<Rows;i++)
		{
			for(int j=0;j<Cols;j++)
			{
				if( pTable[i][j][tag]==true )
				{
					a[count] = i;
					b[count] = j;
					count++;
				}
				if(count==5) return;
			}
		}
	}
	return;	
}


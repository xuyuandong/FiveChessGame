// FivechessDlg.h : header file
//
#pragma once

#include "ChessInfo.h"

#define INTERVAL 20
#define LENGTH 340

// CFivechessDlg dialog
class CFivechessDlg : public CDialog
{
// Construction
public:
	CFivechessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FIVECHESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStart();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedAbout();

private:
	ChessInfo myChess;
	BOOL gameOver;
	CBitmap* chessBoard;
	CBitmap* blackMan;
	CBitmap* whiteMan;
	CBitmap* winnerMan;
	
};

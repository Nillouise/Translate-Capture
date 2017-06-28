#pragma once
#include"Server.h"
#include<string>
// CClient dialog

class CClient : public CDialogEx
{
	DECLARE_DYNAMIC(CClient)

public:
	CClient(CWnd* pParent = NULL);   // standard constructor
	virtual ~CClient();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_client };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	DWORD m_ip;
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedImg();
	void drawpic(CString filename);
	CString findpic(int num);
	int getpicNum(CString filename);

	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	int frameNum;
};
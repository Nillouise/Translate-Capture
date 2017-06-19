#pragma once
#define  WM_SOCKET WM_USER+33

// CServer dialog

class CServer : public CDialogEx
{
	DECLARE_DYNAMIC(CServer)

public:
	CServer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServer();
	SOCKET socket_server;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_server };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
protected:
//	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);
public:
	CString m_link;
};


const int Server_Port = 8944;
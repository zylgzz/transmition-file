#pragma once
#define WM_ADDSERVER (WM_USER + 2)

// CChooser 对话框

class CChooser : public CDialogEx
{
	DECLARE_DYNAMIC(CChooser)

public:
	CChooser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChooser();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	int m_port, m_num;
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedCannel();
	afx_msg void OnBnClickedSure();
};

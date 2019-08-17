#pragma once
#define WM_CLOSESERVER (WM_USER + 3)

// CCloserServer 对话框

class CCloserServer : public CDialogEx
{
	DECLARE_DYNAMIC(CCloserServer)

public:
	CCloserServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCloserServer();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	long m_port;
	virtual void OnOK();
	virtual void OnCancel();
};

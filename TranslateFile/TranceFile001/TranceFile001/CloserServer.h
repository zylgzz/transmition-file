#pragma once
#define WM_CLOSESERVER (WM_USER + 3)

// CCloserServer �Ի���

class CCloserServer : public CDialogEx
{
	DECLARE_DYNAMIC(CCloserServer)

public:
	CCloserServer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCloserServer();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	long m_port;
	virtual void OnOK();
	virtual void OnCancel();
};

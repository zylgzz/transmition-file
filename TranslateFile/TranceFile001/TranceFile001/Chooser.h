#pragma once
#define WM_ADDSERVER (WM_USER + 2)

// CChooser �Ի���

class CChooser : public CDialogEx
{
	DECLARE_DYNAMIC(CChooser)

public:
	CChooser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChooser();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	int m_port, m_num;
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedCannel();
	afx_msg void OnBnClickedSure();
};

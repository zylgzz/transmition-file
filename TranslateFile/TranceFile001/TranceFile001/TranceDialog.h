#pragma once
#define WM_CHILD_DIALOG_CLOSE (WM_USER + 100) 
#define WM_SOCKET (WM_USER + 1)
#define RECVBUFSIZE  1*1024*1024
#define READBUFSIZE  1*1024*1024
#include"../../command/MyStr.h"
#include<fstream>
#include "afxwin.h"

struct Fileinfo{
	CString FileName;
	ULONGLONG FileSize;
};
// CTranceDialog 对话框
class CTranceDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTranceDialog)

public:
	CTranceDialog(CWnd* pParent = NULL);   // 标准构造函数
	~CTranceDialog();
	
	void SetSocket(SOCKET socket);
	static BOOL StringToChar(CString str, char ch[])
	{
		char* tmpch;
		int wLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		tmpch = new char[wLen + 1];
		WideCharToMultiByte(CP_ACP, 0, str, -1, tmpch, wLen, NULL, NULL);
		for (int i = 0; i < wLen; ++i)
			ch[i] = tmpch[i];
		ch[wLen] = '\0';
		return TRUE;
	}
// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void SendFile();									//sned file

	DECLARE_MESSAGE_MAP()
public:
	Fileinfo m_FileInfo;
	CWinThread* m_FileSendThread;
	CWinThread* m_FileReadThread;
	SOCKET socket;
	afx_msg void OnClose();
	afx_msg void OnBnClickedBrowser();
	// //下载文件的路径
	CString m_DownloadPath;
	afx_msg void OnEnKillfocusEsrcpath();
	CString m_SrcPath;
	afx_msg void OnBnClickedBrowser2();
	CString m_FilePath;
	CString m_FileSize;
	afx_msg void OnBnClickedStartdownload2();//start up file 
	CString m_UpPath;
	afx_msg void OnBnClickedStartdownload();
	DWORD m_DownloadSpeech;
	afx_msg void OnEnKillfocusEsrcpath2();
	afx_msg void OnEnKillfocusEdownloadpath();
	afx_msg void OnEnKillfocusEsetspeech();
	afx_msg void OnEnKillfocusEdownloadpath2();
	afx_msg void OnEnKillfocusEsetspeech2();
	CString m_receive_total_size;

	CString m_SendCompleteSize;
	CString m_SendSpeech;
	CString m_SendState;
	CString m_SendNeedTime;
	CString m_receive_in_fact_speech;
	CString m_ReceiveCompleteSize;
	CString m_ReceiveState;
	CString m_ReceiveNeedTime;
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	long long  m_SendAgainSize;
	long long m_recvAginSize;

};

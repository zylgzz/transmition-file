
// TranceFile001Dlg.h : 头文件
//


#pragma once
#include "afxcmn.h"
#include"TranceDialog.h"
#include"Chooser.h"
#include"CloserServer.h"
#include<vector>
//#include"../../command/hideprocess.h"
//#pragma comment(lib,"libcmt.lib")
#define VERSION L"1.2"

//server struct ,use it to open server
struct mySocket
{
	CString ip;
	int port;
	SOCKET server_socket;
};


//list view data struct
struct  structListData
{
	CString ip;					//server ip
	CString port;				//server port
	CString server_version;		//server version
	CString use_state;			//sub open or not
	SOCKET socket;				//socket of client host

	BOOL receive_file;			//receive file state
	DWORDLONG receive_size;		//receive file size now
	DWORDLONG receive_total_size;	//receive file speech
	CTranceDialog *dlg;			//use update sub windown

	std::fstream *file;
	long long receive_start_time;//the time of start receive 


};

// CTranceFile001Dlg 对话框
class CTranceFile001Dlg : public CDialogEx
{
// 构造
public:
	CTranceFile001Dlg(CWnd* pParent = NULL);	// 标准构造函数

	virtual BOOL PreTranslateMessage(MSG* msg);							//add
	virtual LRESULT WindowProc(UINT msg, WPARAM wParam, LPARAM lParam); //code them
	void SockMsg(WPARAM wParam, LPARAM lParam);							//code them

// 对话框数据
	enum { IDD = IDD_TRANCEFILE001_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	friend class CTranceDialog;
	void SetListTitle();								//set list title
	void ShowList(CListCtrl &list, std::vector<structListData>& m_ListDatas);//show list 
	void ShowListenPort(std::vector<mySocket>& m_Servers);//show listen port
	void DealwithData();								//server deal with data  
	BOOL WriteDateList( std::vector<structListData>& m_ListDatas, structListData list_data);
														//write  list data
	BOOL DeleteDateList(std::vector<structListData>& m_ListDatas, SOCKET socket);
	BOOL DeleteListenPort(std::vector<mySocket>& m_Servers, int port);
	int GetListRow(std::vector<structListData>& m_ListDatas, SOCKET socket);//get list row

	BOOL StartServer(CString ip=L"0.0.0.0", int port=5657,long num=1000);				//start server


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	std::vector<mySocket> m_Servers;
	std::vector<structListData> m_ListDatas;
	mySocket TempSocket;
	structListData m_TempListData;
	SOCKET m_ServerSocket;
	SOCKET m_ClientSocket;
	SOCKADDR_IN m_svrAddr;
	SOCKADDR_IN m_ClientAddr;

	CString receive_file_dest_path;
	CString receive_file_src_path;
	DWORDLONG total=0;
	DWORD ret;
	CWinThread* m_SeverThread;

	afx_msg void OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnChildDialogClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddServer(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloseServer(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);

	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnMenuTerminal();
	afx_msg void OnAddServer2();
	afx_msg void OnCloseServer();
	CString m_ListenPort;
	long m_OnlineCount;
	BOOL bHideWindown;
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
};



// TranceFile001Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TranceFile001.h"
#include "TranceFile001Dlg.h"
#include "afxdialogex.h"

#include <io.h>
#include <fcntl.h>
#include<iostream>
#include<mutex>
std::mutex mut;
void OpenConsole()
{
	AllocConsole();
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle, _O_TEXT);
	FILE * hf = _fdopen(hCrt, "w");
	*stdout = *hf;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTranceFile001Dlg 对话框



CTranceFile001Dlg::CTranceFile001Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTranceFile001Dlg::IDD, pParent)
	, m_ListenPort(_T(""))
	, m_OnlineCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTranceFile001Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_LISTENPORT, m_ListenPort);
	DDX_Text(pDX, IDC_EDIT2, m_OnlineCount);
}

BEGIN_MESSAGE_MAP(CTranceFile001Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CTranceFile001Dlg::OnDoubleClickList)
	ON_MESSAGE(WM_CHILD_DIALOG_CLOSE, &CTranceFile001Dlg::OnChildDialogClose)
	ON_MESSAGE(WM_ADDSERVER, &CTranceFile001Dlg::OnAddServer)
	ON_MESSAGE(WM_CLOSESERVER, &CTranceFile001Dlg::OnCloseServer)
	ON_MESSAGE(WM_HOTKEY, &CTranceFile001Dlg::OnHotKey)
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_TERMINAL, &CTranceFile001Dlg::OnMenuTerminal)
	ON_COMMAND(ID_Menu, &CTranceFile001Dlg::OnAddServer2)
	ON_COMMAND(ID_CLOSESERVER_CLOSESERVER, &CTranceFile001Dlg::OnCloseServer)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTranceFile001Dlg 消息处理程序

BOOL CTranceFile001Dlg::OnInitDialog()
{
	//OpenConsole();
	CDialogEx::OnInitDialog();
	bHideWindown = FALSE;
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	SetTimer(1, 1000, NULL);
	this->DealwithData();
	RegisterHotKey(m_hWnd, (WM_USER+4), MOD_CONTROL, 'Q');
	RegisterHotKey(m_hWnd, (WM_USER+5), MOD_CONTROL,'q');
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTranceFile001Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTranceFile001Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTranceFile001Dlg::SetListTitle()
//set list column title
{

	LONG styles;
	styles = GetWindowLong(m_list.m_hWnd, GWL_STYLE);
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, styles | LVS_REPORT);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CStringList list_tile;
	list_tile.AddTail(L"IP adress");
	list_tile.AddTail(L"Port");
	list_tile.AddTail(L"Server version");
	list_tile.AddTail(L"use state");
	int i = 0;
	for (POSITION pos = list_tile.GetHeadPosition(); pos != NULL;)
	{
		m_list.InsertColumn(i++, list_tile.GetNext(pos), LVCFMT_LEFT, 190);
	}
}

void CTranceFile001Dlg ::ShowList(CListCtrl &list, std::vector<structListData>& m_ListDatas)
//show list
{
	list.DeleteAllItems();
	int irow = 0, num=0;
	for (long it = m_ListDatas.size()-1; it >=0 ; it--)
	{
		irow = list.InsertItem(0, 0);
		list.SetItemText(irow, 0, m_ListDatas[it].ip);
		list.SetItemText(irow, 1, m_ListDatas[it].port);
		list.SetItemText(irow, 2, m_ListDatas[it].server_version);
		list.SetItemText(irow, 3, m_ListDatas[it].use_state);
		num++;
	}
	m_OnlineCount = num;
	UpdateData(FALSE);
}

void CTranceFile001Dlg::ShowListenPort(std::vector<mySocket>& m_Servers)
{
	CString Tempstr;
	m_ListenPort = L"";
	for (auto it = m_Servers.begin(); it < m_Servers.end(); it++)
	{
		Tempstr.Format(L"%d,", it->port);
		m_ListenPort += Tempstr;
	}
	UpdateData(FALSE);
}

BOOL CTranceFile001Dlg::WriteDateList(std::vector<structListData>& m_ListDatas, structListData list_data)
{
	
	m_ListDatas.push_back(m_TempListData);
	ShowList(m_list, m_ListDatas);
	return TRUE;
}

BOOL CTranceFile001Dlg::DeleteDateList(std::vector<structListData>& m_ListDatas, SOCKET socket)
//delete host 
{
	for (auto it = m_ListDatas.begin(); it < m_ListDatas.end(); it++)
	{
		if (it->socket == socket)
		{
			delete it->file;		//free memory
			m_ListDatas.erase(it);
			ShowList(m_list, m_ListDatas);
			return TRUE;
		}
	}
	ShowList(m_list, m_ListDatas);
	return FALSE;
}

int CTranceFile001Dlg::GetListRow(std::vector<structListData>& m_ListDatas, SOCKET socket)
//get list row
{
	int i = 0;
	for (auto it = m_ListDatas.begin(); it < m_ListDatas.end(); it++,i++)
	{
		if (it->socket == socket)return i;
	}
	return -1;
}

void CTranceFile001Dlg::DealwithData()
//to deal with data
{
	//1.set list column title
	this->SetListTitle();
	this->ShowList(m_list,m_ListDatas);
	StartServer();
}

void CTranceFile001Dlg::OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int row=m_list.GetNextItem(-1, LVIS_SELECTED);
	std::cout << "double click num:" << row << std::endl;;
	
	if (m_list.GetItemText(row, 3) == L"no use")
	{
		m_ListDatas[row].use_state = L"use";
	//	m_list.SetItemText(row, 3, L"use");
		ShowList(m_list, m_ListDatas);
		CTranceDialog *dlg = new CTranceDialog();
		dlg->Create(IDD_DIALOG1);
		dlg->SetWindowText(L"//" + m_ListDatas[row].ip + L":" + m_ListDatas[row].port);
		dlg->SetSocket(m_ListDatas[row].socket);
		dlg->ShowWindow(SW_SHOWNORMAL);
		m_ListDatas[row].dlg = dlg;
	}
	else if (m_list.GetItemText(row, 3) == L"use")
	{
		if (m_ListDatas[row].dlg->IsWindowVisible())m_ListDatas[row].dlg->ShowWindow(SW_HIDE);
		else m_ListDatas[row].dlg->ShowWindow(SW_SHOWNORMAL);
	}
	
	*pResult = 0;
}

LRESULT CTranceFile001Dlg::OnChildDialogClose(WPARAM wParam, LPARAM lParam)
{
	wParam = GetListRow(m_ListDatas, lParam);
	m_ListDatas[wParam].use_state = L"no use";
	ShowList(m_list, m_ListDatas);
	std::cout << "close deai with:" << wParam << std::endl;
	//UpdateData(FALSE);
	return 0;
}

LRESULT CTranceFile001Dlg::OnAddServer(WPARAM wParam, LPARAM lParam)
{
	StartServer(L"0.0.0.0", wParam, lParam);
	return 0;
}

LRESULT CTranceFile001Dlg::OnCloseServer(WPARAM wParam, LPARAM lParam)
{
	CString port;
	port.Format(L"%ld", wParam);
	for (auto it = m_ListDatas.begin(); it < m_ListDatas.end(); it++)
	{
		if (it->port == port && it->use_state == "use")
		{
			MessageBox(L"Server is runing,please stop it");
			return 1;
		}
	}
	for (auto it = m_ListDatas.begin(); it < m_ListDatas.end(); it++)
	{
		if (it->port == port)
		{
			send(it->socket, "weiyibiaoshifu@terminal it", 40, 0);
			Sleep(10);
		}
	}
	DeleteListenPort(m_Servers, wParam);
	return 0;
}

LRESULT CTranceFile001Dlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	if (wParam == (WM_USER+4) || wParam == (WM_USER+5))
	{
		if (!bHideWindown)
		{
			ShowWindow(SW_HIDE);
			bHideWindown = TRUE;
			
		}
		else
		{
			ShowWindow(SW_NORMAL);
			bHideWindown = FALSE;
		}
	}
	return 0;
}

void CTranceFile001Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//if(IsWindowVisible())HideChildDlg(m_ListDatas);
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CTranceFile001Dlg::DeleteListenPort(std::vector<mySocket>& m_Servers, int port)
{
	for (auto it = m_Servers.begin(); it < m_Servers.end(); it++)
	{
		if (it->port == port)
		{
			closesocket(it->server_socket);
			Sleep(2);
			m_Servers.erase(it);
			ShowListenPort(m_Servers);
			return true;
		}
	}
	ShowListenPort(m_Servers);
	return false;
}

BOOL CTranceFile001Dlg::StartServer(CString ip, int port,long num)
//start server
{
	//1.init socket
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		return FALSE;
	}

	if ((TempSocket.server_socket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		WSACleanup();
		return FALSE;
	}
	//2.start listen
	USES_CONVERSION;
	char * pFileName = T2A(ip);
	this->m_svrAddr.sin_family = AF_INET;
	this->m_svrAddr.sin_addr.s_addr = inet_addr(pFileName);
	this->m_svrAddr.sin_port = htons(port);
	if (bind(TempSocket.server_socket, (sockaddr *)&this->m_svrAddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		AfxMessageBox(L"bind error"+ip);
		return FALSE;
	}
	if (listen(TempSocket.server_socket, num)< 0)
	{
		AfxMessageBox(L"listen error");
		return FALSE;

	}
	int iResult = WSAAsyncSelect(TempSocket.server_socket, m_hWnd, WM_SOCKET, FD_READ | FD_ACCEPT | FD_CLOSE);
	if (iResult)
		MessageBox(L"Can't call WSAAsyncSelect");
	else
	{
		TempSocket.ip = ip;
		TempSocket.port = port;
		m_Servers.push_back(TempSocket);
	}
	ShowListenPort(m_Servers);
	return TRUE;
	
}

BOOL CTranceFile001Dlg::PreTranslateMessage(MSG* msg)
{
	return CDialog::PreTranslateMessage(msg);
}

LRESULT CTranceFile001Dlg::WindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_SYSCOMMAND:
		{
		   if (wParam == SC_CLOSE)
		   {
			   for (auto it : m_ListDatas)if (it.use_state==L"use")return 0;
			   CDialogEx::OnCancel();
			  return 0;
		   }
		   else if (wParam == SC_MINIMIZE)
		   {
			   bHideWindown = TRUE;
			   ShowWindow(SW_HIDE);
			   return 0;
		   }
		   break;
		}
	}
	if (msg == WM_SOCKET)
		SockMsg(wParam, lParam);
	return CDialog::WindowProc(msg, wParam, lParam);
}

void CTranceFile001Dlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	char *recv_buf = new char[RECVBUFSIZE];
	memset(recv_buf, 0, RECVBUFSIZE);
	if (WSAGETSELECTERROR(lParam))
	{
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_ACCEPT:
		{
			m_TempListData = { 0 };
			m_TempListData.socket = accept(wParam,NULL,NULL);
			m_TempListData.ip = "123";
			m_TempListData.server_version = VERSION;
			m_TempListData.use_state = L"no use";
			m_TempListData.receive_file = FALSE;
			m_TempListData.receive_size = 0;
			m_TempListData.file = new std::fstream;
			if (!WriteDateList(m_ListDatas, m_TempListData))
			{
				MessageBox(L"add host to list error");
				return;
			}
			break;
		}
		case FD_READ:
		{
			int i = GetListRow(m_ListDatas, wParam);
			if (i < 0)
			{
				if (m_ListDatas[i].file->is_open())m_ListDatas[i].file->close();
				m_ListDatas[i].receive_file = FALSE;
				break;
			}

			if (m_ListDatas[i].ip == L"123")//first to recv data
        	{
				//memset(buf, 0, BUFSIZE);
        		ret = recv(wParam, recv_buf,RECVBUFSIZE, 0);
            	char ip[50] = { 0 }, port[10] = { 0 }, sp = '&';
            	MyString::myStrSplit(recv_buf, ip, sp, port);
            	CStringA strAIp(ip), strAPort(port);
            	CString strIp(strAIp), strPort(strAPort);
		    	m_ListDatas[i].ip = strIp;
		    	m_ListDatas[i].port = strPort;
				ShowList(m_list, m_ListDatas);
        	}
			else//not the first recv message
			{
				//memset(buf, 0, BUFSIZE);
				ret= recv(wParam, recv_buf, RECVBUFSIZE, 0);
				if ( ret<0)
				{
					MessageBox(L"recv data fail");
					break;
				}
				else					
				{
					//std::cout << "error buf" << recv_buf << std::endl;
					//send agin deal with
					if (m_ListDatas[i].dlg->m_SendAgainSize < 0 &&
						MyString::myStrLen(recv_buf)>27&&
						MyString::myStrContainS(recv_buf, "weiyibiaoshifu@send file agin"))
					{
						char temp[30] = { 0 };
						MyString::myStrSubSplitR(recv_buf, temp, '#');
						m_ListDatas[i].dlg->m_SendAgainSize = MyString::charToDwordlong(temp);
						std::cout << "send start file location:" << m_ListDatas[i].dlg->m_SendAgainSize << std::endl;
						break;
					}

					

					// deal with msg to write to file
					if (!m_ListDatas[i].receive_file)
					{
						//extract info= file name+ file total size
						if (!MyString::myStrContainS(recv_buf, "weiyibiaoshifu@send file head"))break;
						m_ListDatas[i].dlg->m_recvAginSize = -1;

						char file_path[MAX_PATH] = {0};
						char receive_sp[30] = {0};
						char agin_size[30] = { 0 };
						char temp[MAX_PATH] = {0};

						MyString::myStrSplit(recv_buf, file_path, '#', temp);
						MyString::myStrSplit(temp, file_path, '$', receive_sp);
						
						MyString::myStrcp(temp,receive_sp);					//agin size
						MyString::myStrSplit(temp, receive_sp, '&', agin_size);
						m_ListDatas[i].dlg->m_recvAginSize = MyString::charToDwordlong(agin_size);

						//record total size and translate state and receive size and now_size
						printf("\n%s\n\n%s\n%s\n%s\n\n", recv_buf, file_path, receive_sp, agin_size);
						m_ListDatas[i].receive_total_size=MyString::charToDwordlong(receive_sp);
						m_ListDatas[i].receive_size = m_ListDatas[i].dlg->m_recvAginSize;
						if (m_ListDatas[i].dlg->m_recvAginSize > m_ListDatas[i].receive_total_size+1)
						{
							m_ListDatas[i].receive_size = 0;
							m_ListDatas[i].dlg->m_recvAginSize = 0;
						}
						m_ListDatas[i].receive_file = TRUE;

						//update view
						CStringA tempA;
						tempA.Format("%0.2f", m_ListDatas[i].receive_total_size / 1024.0);
						tempA += " k";
						m_ListDatas[i].dlg->m_receive_total_size=tempA;
						m_ListDatas[i].dlg->UpdateData(FALSE);

						//open file name is file_path
						/*int temp_size = MultiByteToWideChar(CP_ACP, 0, file_path, -1,NULL, 0);
						wchar_t *wfile_path = new wchar_t[temp_size];
						memset(wfile_path, 0, temp_size);
						MultiByteToWideChar(CP_ACP, 0, file_path, -1, wfile_path, temp_size);*/
						if (m_ListDatas[i].file->is_open())m_ListDatas[i].file->close();
						if (m_ListDatas[i].receive_size == 0)
						{
							m_ListDatas[i].file->open(file_path, std::ios_base::out | std::ios_base::binary);
						}
						else
						{
							m_ListDatas[i].file->open(file_path, std::ios_base::app | std::ios_base::binary);

						}
						if (m_ListDatas[i].file->is_open())std::wcout << L"\nfile open\n";
						//delete []wfile_path;
						//get in fact speech of variable
						m_ListDatas[i].receive_start_time = MyString::myGetSysTimeMicros();
						break;
					}
					else
					{

						//

						double speech;
						CStringA strTemp;
						if (!m_ListDatas[i].receive_file)break;
						m_ListDatas[i].dlg->GetDlgItem(IDC_STARTDOWNLOAD)->SetWindowText(L"end download");
						if (m_ListDatas[i].receive_total_size <= m_ListDatas[i].receive_size ||
							MyString::myStrSame(recv_buf, "weiyibiaoshifu@send file over"))
						{
							std::wcout << recv_buf << std::endl;
							//if (!MyString::myStrSame(buf, "weiyibiaoshifu@send file over"))break; 
							if (m_ListDatas[i].file->is_open())m_ListDatas[i].file->close();
							m_ListDatas[i].receive_file = FALSE;
							std::wcout << L"file receive success\n";
							
							//update view
							m_ListDatas[i].dlg->m_ReceiveState = L"Free ...";
							m_ListDatas[i].dlg->m_receive_in_fact_speech = L"0 k/s";
							m_ListDatas[i].dlg->m_ReceiveNeedTime = L"0 s";
							m_ListDatas[i].dlg->GetDlgItem(IDC_STARTDOWNLOAD)->SetWindowText(L"start download");
							strTemp.Format("%0.2lfk", m_ListDatas[i].receive_size / 1024.0);
							m_ListDatas[i].dlg->m_ReceiveCompleteSize = strTemp;
							m_ListDatas[i].dlg->UpdateData(FALSE);
							m_ListDatas[i].receive_size = 0;

							break;
						}
						m_ListDatas[i].receive_size += ret;
				     	std::wcout << L"now receive size:"
							<< m_ListDatas[i].receive_size << std::endl;
				     	m_ListDatas[i].file->write(recv_buf, ret);
				     	m_ListDatas[i].file->flush();
						
						//update view
						speech = (m_ListDatas[i].receive_size - m_ListDatas[i].dlg->m_recvAginSize) * 10 * 10 * 10 * 10 * 10
							* 10 * 10 / (MyString::myGetSysTimeMicros() -m_ListDatas[i].receive_start_time) / 1024.0;
						strTemp.Format("%0.2lfkb/s", speech);
						m_ListDatas[i].dlg->m_receive_in_fact_speech = strTemp;
						strTemp.Format("%0.2lfk", m_ListDatas[i].receive_size / 1024.0);
						m_ListDatas[i].dlg->m_ReceiveCompleteSize = strTemp;
						m_ListDatas[i].dlg->m_ReceiveState = L"Sending ...";
						strTemp.Format("%0.2lfs", (m_ListDatas[i].receive_total_size
							- m_ListDatas[i].receive_size)/ 1024/ speech);
						m_ListDatas[i].dlg->m_ReceiveNeedTime = strTemp;
						m_ListDatas[i].dlg->UpdateData(FALSE);
					}
				}//end else of deal with file
			}//end  else of msg not first
			//delete[] buf;
        	break;
		}
		case FD_CLOSE:
		{
			//MessageBox(L"have client exit");
			DeleteDateList(m_ListDatas, wParam);
			closesocket(wParam);
			break;
		}

	}//end switch
	delete[]recv_buf;
}


void CTranceFile001Dlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_LIST1);
	RECT rect;
	pWnd->GetWindowRect(&rect);
	CMenu popMenu;
	popMenu.LoadMenu(IDR_MENU2);
	POINT posMouse;
	GetCursorPos(&posMouse);
	if (posMouse.x<rect.left || posMouse.x>rect.right ||//菜单区域屏蔽
		posMouse.y<rect.top || posMouse.y>rect.bottom)return;
	CMenu* pSubMenu = NULL;
	pSubMenu = popMenu.GetSubMenu(0);
	pSubMenu->TrackPopupMenu(0, posMouse.x, posMouse.y, this);
}


void CTranceFile001Dlg::OnMenuTerminal()
{
	
	int row = m_list.GetNextItem(-1, LVIS_SELECTED);
	if (row == -1)return;
	if (m_ListDatas[row].use_state==L"use")
	{
		MessageBox(L"please wait translate complete");
		return;
	}
	send(m_ListDatas[row].socket, "weiyibiaoshifu@terminal it", 40, 0);
	Sleep(10);
	
}


void CTranceFile001Dlg::OnAddServer2()
{
	CChooser *dlg = new CChooser;
	dlg->Create(IDD_DIALOG2);
	dlg->ShowWindow(SW_SHOWNORMAL);
}


void CTranceFile001Dlg::OnCloseServer()
{
	CCloserServer *dlg = new CCloserServer;
	dlg->Create(IDD_DIALOG3);
	dlg->ShowWindow(SW_SHOWNORMAL);
}


void CTranceFile001Dlg::OnDestroy()
{

	CDialogEx::OnDestroy();
	UnregisterHotKey(m_hWnd, WM_USER+4);
	UnregisterHotKey(m_hWnd, WM_USER+5);
}

void CTranceFile001Dlg::OnOK()
{
}


void CTranceFile001Dlg::OnCancel()
{
}

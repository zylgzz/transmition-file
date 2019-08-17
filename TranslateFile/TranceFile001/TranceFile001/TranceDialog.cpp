// TranceDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TranceFile001.h"
#include "TranceDialog.h"
#include "afxdialogex.h"
#include<iostream>


//test use start
CString strDownloadSrcPath = L"D:/iso/cn_windows_7_professional_x64_dvd_x15-65791.iso";
CString strDownloadDstPath = L"C:/Users/zz/Desktop/test1/";
CString strUpfileDstPath = L"C:/Users/zz/Desktop/test1/";
//tset use end


// CTranceDialog 对话框

IMPLEMENT_DYNAMIC(CTranceDialog, CDialogEx)

CTranceDialog::CTranceDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTranceDialog::IDD, pParent)
	, m_DownloadPath(_T(""))
	, m_SrcPath(_T(""))
	, m_FilePath(_T(""))
	, m_UpPath(_T(""))
	, m_DownloadSpeech(0)
	, m_receive_total_size(_T(""))
	, m_SendCompleteSize(_T(""))
	, m_SendSpeech(_T(""))
	, m_SendState(_T(""))
	, m_SendNeedTime(_T(""))
	, m_receive_in_fact_speech(_T(""))
	, m_ReceiveCompleteSize(_T(""))
	, m_ReceiveState(_T(""))
	, m_ReceiveNeedTime(_T(""))
{
	m_DownloadPath = strDownloadDstPath;
	m_SrcPath = strDownloadSrcPath;
	m_UpPath = strUpfileDstPath;
	m_SendSpeech = L"1024";
	m_SendState = L"Free ...";
	m_DownloadSpeech = 10;
	m_ReceiveState = L"Free ...";
	m_ReceiveNeedTime = L"0 s";

	m_SendAgainSize = -1;
	m_recvAginSize = -1;
	
	

}

CTranceDialog::~CTranceDialog()
{
	
}

void CTranceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDOWNLOADPATH, m_DownloadPath);
	DDX_Text(pDX, IDC_ESRCPATH, m_SrcPath);
	DDX_Text(pDX, IDC_EDOWNLOADPATH2, m_FilePath);
	DDX_Text(pDX, IDC_EFILESIZE2, m_FileSize);
	DDX_Text(pDX, IDC_ESRCPATH2, m_UpPath);
	DDX_Text(pDX, IDC_ESETSPEECH, m_DownloadSpeech);
	DDX_Text(pDX, IDC_EFILESIZE, m_receive_total_size);
	DDX_Text(pDX, IDC_ECOMPELETESIZE2, m_SendCompleteSize);
	DDX_Text(pDX, IDC_ESETSPEECH2, m_SendSpeech);
	DDX_Text(pDX, IDC_EDOWNLOADSTATE2, m_SendState);
	DDX_Text(pDX, IDC_ENEEDTIME2, m_SendNeedTime);
	DDX_Text(pDX, IDC_EINFACTSPEECH, m_receive_in_fact_speech);
	DDX_Text(pDX, IDC_ECOMPELETESIZE, m_ReceiveCompleteSize);
	DDX_Text(pDX, IDC_EDOWNLOADSTATE, m_ReceiveState);
	DDX_Text(pDX, IDC_ENEEDTIME, m_ReceiveNeedTime);
}


BEGIN_MESSAGE_MAP(CTranceDialog, CDialogEx)
	
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BROWSER, &CTranceDialog::OnBnClickedBrowser)
	ON_EN_KILLFOCUS(IDC_ESRCPATH, &CTranceDialog::OnEnKillfocusEsrcpath)
	ON_BN_CLICKED(IDC_BROWSER2, &CTranceDialog::OnBnClickedBrowser2)
	ON_BN_CLICKED(IDC_STARTDOWNLOAD2, &CTranceDialog::OnBnClickedStartdownload2)
	ON_BN_CLICKED(IDC_STARTDOWNLOAD, &CTranceDialog::OnBnClickedStartdownload)
	ON_EN_KILLFOCUS(IDC_ESRCPATH2, &CTranceDialog::OnEnKillfocusEsrcpath2)
	ON_EN_KILLFOCUS(IDC_EDOWNLOADPATH, &CTranceDialog::OnEnKillfocusEdownloadpath)
	ON_EN_KILLFOCUS(IDC_ESETSPEECH, &CTranceDialog::OnEnKillfocusEsetspeech)
	ON_EN_KILLFOCUS(IDC_EDOWNLOADPATH2, &CTranceDialog::OnEnKillfocusEdownloadpath2)
	ON_EN_KILLFOCUS(IDC_ESETSPEECH2, &CTranceDialog::OnEnKillfocusEsetspeech2)
	ON_WM_DROPFILES()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTranceDialog 消息处理程序

void CTranceDialog::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	LPARAM sock = this->socket;
	GetParent()->SendMessage(WM_CHILD_DIALOG_CLOSE, 0,sock);
	Sleep(10);
	CDialogEx::OnCancel();
}

void CTranceDialog::SetSocket(SOCKET socket)//set socket
{
	this->socket = socket;
	SetTimer(2, 500, NULL);

	this->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//使窗口可以被其他窗口覆盖
}

void CTranceDialog::OnBnClickedBrowser()//get save path
{
	GetDlgItem(IDC_BROWSER)->EnableWindow(FALSE);
	TCHAR         szFolderPath[MAX_PATH];
	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.hwndOwner = AfxGetMainWnd()->m_hWnd;
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T(" Save file Path");
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX ;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			this->m_DownloadPath = szFolderPath;
			this->m_DownloadPath.Replace(L"\\", L"/");
			this->m_DownloadPath.Append(L"/");
			UpdateData(FALSE);
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
	GetDlgItem(IDC_BROWSER)->EnableWindow(TRUE);
}


void CTranceDialog::OnEnKillfocusEsrcpath()//get src path
{
	UpdateData(TRUE);

}


void CTranceDialog::OnBnClickedBrowser2()//select file to send
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_BROWSER2)->EnableWindow(FALSE);
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"all file (*.*)|*.*||", this);
	if (dlg.DoModal() == IDOK)
	{

		m_FilePath = dlg.GetPathName();				//get file path
		m_FileInfo.FileName = dlg.GetFileName();	//get file name
		m_FilePath.Replace(L"\\", L"/");
		CFile file(m_FilePath, CFile::modeRead);	//open file	
		m_FileInfo.FileSize = file.GetLength();		//get file zie	
		file.Close();
		//update view 
		CStringA strTempSize;
		strTempSize.Format("%0.2lf", m_FileInfo.FileSize / 1024.0);
		m_FileSize = strTempSize;
		GetDlgItem(IDC_EFILESIZE2)->SetWindowText(m_FileSize+L" k");
		GetDlgItem(IDC_EDOWNLOADPATH2)->SetWindowText(m_FilePath);
		if (m_FileInfo.FileSize/1024.0 == 0)GetDlgItem(IDC_EFILESIZE2)->SetWindowText(L"0 k");
		GetDlgItem(IDC_ECOMPELETESIZE2)->SetWindowText(L"0 k");
		GetDlgItem(IDC_EINFACTSPEECH2)->SetWindowText(L"0 kb/s");
	}
	GetDlgItem(IDC_BROWSER2)->EnableWindow(TRUE);

}


UINT FileSendThread(LPVOID para)
{
	CTranceDialog* sDlg = (CTranceDialog*)para;
	sDlg->m_SendAgainSize = -1;
	char cBufName[MAX_PATH], cBufPath[MAX_PATH], cBufSize[MAX_PATH], cFileName[MAX_PATH];
	CStringA TempStrA;
	CString TempStr;
	//1.send file info name ,size and save path to client host
		//1.1 send file name to client host+@N
	if (sDlg->m_FilePath.GetLength() == 0)	return 1;
	if (sDlg->m_UpPath.GetLength() == 0)	return 1;
	sDlg->m_UpPath.Replace(L"\\", L"/");
	CTranceDialog::StringToChar(sDlg->m_FileInfo.FileName,cBufName);
	CTranceDialog::StringToChar(sDlg->m_UpPath, cBufPath);
	MyString::myStrcat(cBufName, "@N");
	MyString::myStrcat(cBufPath, cBufName);
	send(sDlg->socket, (const char *)cBufPath, MyString::myStrLen(cBufPath)+1, 0);
	Sleep(3);
		//1.2 send file name to client host+@S
	MyString::DwordlongToChar(sDlg->m_FileInfo.FileSize, cBufSize);
	MyString::myStrcat(cBufSize, "@S");
	send(sDlg->socket, (const char *)cBufSize, MyString::myStrLen(cBufSize)+1, 0);
	Sleep(3);
	

	//2.open file and send 
	CTranceDialog::StringToChar(sDlg->m_FilePath, cFileName);
	std::fstream file(cFileName,std::ios_base::in|std::ios_base::binary);
	if (!file.is_open())
	{
		AfxMessageBox(_T("open file error"));
		return 1;
	}
	char *read_buf=new char[READBUFSIZE];
	memset(read_buf, 0, READBUFSIZE);
	//char read_buf[BUFSIZE] = { 0 };

	MyString::myStrcp(read_buf, "@O");
	Sleep(3);
	send(sDlg->socket, (const char *)read_buf, MyString::myStrLen(read_buf) + 1, 0);//send file start
	Sleep(100);
	while (TRUE)//wait sure of client
	{
		if (sDlg->m_SendAgainSize >=0)break;
		Sleep(1000);
	}
	DWORDLONG nSize = 0;//send size
	DWORDLONG dwCount = sDlg->m_SendAgainSize;
	file.seekg(dwCount, std::ios_base::beg);

	long long nStartTime,nNowTime;							//control speech
	double StanderSpecch,speech,NeedTime;
	nStartTime = MyString::myGetSysTimeMicros();
	sDlg->StringToChar(sDlg->m_SendSpeech, cBufSize);
	StanderSpecch = MyString::charToDwordlong(cBufSize);
	sDlg->m_SendState = L"Sending ...";
	sDlg->GetDlgItem(IDC_EDOWNLOADSTATE2)->SetWindowText(sDlg->m_SendState);
	sDlg->GetDlgItem(IDC_STARTDOWNLOAD2)->SetWindowText(L"end send!");
	while (file)									//start to send file
	{
		if (sDlg->m_SendState == L"Free ...")break;
		//read the size content
		if (StanderSpecch < READBUFSIZE/1024)file.read(read_buf, (int)StanderSpecch*1024 - 1);	//control sizeof
		else file.read(read_buf, READBUFSIZE-1);
		Sleep(10);
		nSize = send(sDlg->socket, (const char *)read_buf, file.gcount(), 0);
		dwCount += (nSize);
		//control speech and update view
		MyString::getTranslateSpeech(nStartTime, StanderSpecch, dwCount - sDlg->m_SendAgainSize);
		nNowTime = MyString::myGetSysTimeMicros();
		speech = (dwCount - sDlg->m_SendAgainSize) * 10 * 10 * 10 * 10 * 10 * 10 * 10 / (nNowTime - nStartTime) / 1024.0;
		TempStrA.Format("%0.2lf Kb/s", speech);
		CString str(TempStrA);
		sDlg->GetDlgItem(IDC_EINFACTSPEECH2)->SetWindowText(str);
		TempStrA.Format("%0.2fk", dwCount/1024.0);
		sDlg->m_SendCompleteSize = TempStrA;
		sDlg->GetDlgItem(IDC_ECOMPELETESIZE2)->SetWindowText(sDlg->m_SendCompleteSize);
		NeedTime = (sDlg->m_FileInfo.FileSize - dwCount) / 1024.0 / speech;
		TempStrA.Format("%0.2f s", NeedTime);
		sDlg->m_SendNeedTime = TempStrA;
		sDlg->GetDlgItem(IDC_ENEEDTIME2)->SetWindowText(sDlg->m_SendNeedTime);
	}
	Sleep(3);
	MyString::myStrcp(read_buf, "@E");
	send(sDlg->socket, (const char *)read_buf, MyString::myStrLen(read_buf), 0);//send file end
	Sleep(3);
	MyString::myStrcp(read_buf, "@E");
	send(sDlg->socket, (const char *)read_buf, MyString::myStrLen(read_buf), 0);//send file end
	sDlg->m_SendState = L"Free ...";
	sDlg->GetDlgItem(IDC_EDOWNLOADSTATE2)->SetWindowText(sDlg->m_SendState);
	sDlg->GetDlgItem(IDC_ENEEDTIME2)->SetWindowText(L"0 s");
	sDlg->GetDlgItem(IDC_EINFACTSPEECH2)->SetWindowText(L"0 k/s");
	//TempStrA.Format("%0.0f", StanderSpecch);
	//sDlg->m_SendSpeech = TempStrA;
	//sDlg->GetDlgItem(IDC_ESETSPEECH2)->SetWindowText(sDlg->m_SendSpeech);
	sDlg->GetDlgItem(IDC_STARTDOWNLOAD2)->SetWindowText(L"start upfile");
	delete[] read_buf;
	file.close();
	return 0;
}


void CTranceDialog::OnBnClickedStartdownload2()//up file
{
	if (m_SendState != L"Free ...")
	{
	
		m_SendState = L"Free ...";
		GetDlgItem(IDC_EDOWNLOADSTATE2)->SetWindowText(m_SendState);
		GetDlgItem(IDC_STARTDOWNLOAD2)->SetWindowText(L"start upfile");
		return;
	}
	m_FileSendThread = AfxBeginThread(FileSendThread, this);
	m_FileSendThread->m_bAutoDelete = TRUE;

}


void CTranceDialog::OnBnClickedStartdownload()
{
	if (m_ReceiveState != "Free ...")
	{
		
		send(socket, "weiyibiaoshifu@stop send to me", 40, 0);
		send(socket, "weiyibiaoshifu@stop send to me", 40, 0);
		return;
	}
	//send command to client host 
	//command="start to send weiyibiaoshifu"+"#"+file_path+"$"+speech+"@"+save_path+"&"+now_size
	char szStartCommand[MAX_PATH*2];
	char szFilePath[MAX_PATH];
	char szSavePath[MAX_PATH];
	DWORDLONG speech = this->m_DownloadSpeech;
	char szSpeech[30];

	if (this->m_DownloadPath.GetLength() == 0)this->m_DownloadPath=strDownloadDstPath;
	if (this->m_SrcPath.GetLength() == 0)this->m_SrcPath = strDownloadDstPath;
	this->m_SrcPath.Replace(L"\\", L"/");
	/*std::wcout.imbue(std::locale("vi-VN"));
	std::wcout << L"wide:" << m_SrcPath << L":" << m_DownloadPath << std::endl;*/
	StringToChar(this->m_SrcPath, szFilePath);
	
	StringToChar(this->m_DownloadPath, szSavePath);
	//std::wcout << "char:" << szFilePath << ":" << szSavePath << std::endl;

	if (speech < 1)speech = 1;			
	MyString::DwordlongToChar(speech, szSpeech);
	
	char MergePath[MAX_PATH] = { 0 };
	char MergeName[MAX_PATH] = { 0 };
	MyString::myStrcp(MergePath, szSavePath);
	MyString::myStrcat(MergePath, "/");
	MyString::myStrSubSplitR(szFilePath, MergeName, '/');
	MyString::myStrcat(MergePath, MergeName);
	std::fstream temp_file(MergePath, std::ios_base::in);
	temp_file.seekg(0, std::ios_base::end);
	long long size = temp_file.tellg();
	

	MyString::myStrcp(szStartCommand, "start to send weiyibiaoshifu#");
	MyString::myStrcat(szStartCommand, szFilePath);
	MyString::myStrcat(szStartCommand, "$");
	MyString::myStrcat(szStartCommand, szSpeech);
	MyString::myStrcat(szStartCommand, "@");
	MyString::myStrcat(szStartCommand, szSavePath);
	if (szStartCommand[MyString::myStrLen(szStartCommand)] != '/')
		MyString::myStrcat(szStartCommand, "/");

	if (size <= 0)
	{
		MyString::myStrcp(szFilePath, "0");		//file size
	}
	else
	{
		MyString::DwordlongToChar(size, szFilePath);
	}

	MyString::myStrcat(szStartCommand, "&");
	MyString::myStrcat(szStartCommand, szFilePath);

	send(this->socket, szStartCommand, MyString::myStrLen(szStartCommand) + 1, 0);
	
}


void CTranceDialog::OnEnKillfocusEsrcpath2()
{
	this->UpdateData(TRUE);
	// TODO:  在此添加控件通知处理程序代码
}


void CTranceDialog::OnEnKillfocusEdownloadpath()
{
	// TODO:  在此添加控件通知处理程序代码
	this->UpdateData(TRUE);
}


void CTranceDialog::OnEnKillfocusEsetspeech()
{
	
	this->UpdateData(TRUE);
	if (m_DownloadSpeech == 0)
	{
		MessageBox(L"Don`t set it equal 0 or too big");
		m_DownloadSpeech = 1024;
	}
	this->UpdateData(FALSE);
}


void CTranceDialog::OnEnKillfocusEdownloadpath2()
{
	// TODO:  在此添加控件通知处理程序代码
	this->UpdateData(TRUE);
}


void CTranceDialog::OnEnKillfocusEsetspeech2()
{
	this->UpdateData(TRUE);
	char temp[MAX_PATH];
	StringToChar(m_SendSpeech, temp);
	long long speech=MyString::charToDwordlong(temp);
	if (speech==0)
	{
		MessageBox(L"Don`t set it equal 0 or too big");
		m_SendSpeech = L"1024";
	}
	this->UpdateData(FALSE);

}



void CTranceDialog::OnOK()
{	
}


void CTranceDialog::OnCancel()
{
}


void CTranceDialog::OnDropFiles(HDROP hDropInfo)
{
	WCHAR szPath[MAX_PATH] = L"";
	char szPathTemp[MAX_PATH],szFileName[MAX_PATH];
	CStringA szNameTemp;
	UINT totalfile = DragQueryFile(hDropInfo, 0xFFFFFFFF, szPath, MAX_PATH);
	for (int filenum = 0; filenum < totalfile; filenum++)
	{
		UINT pathlenght = DragQueryFile(hDropInfo, filenum, szPath, MAX_PATH);//读入第totalfile+1个被拖入文件的路径到szPath中
		m_FilePath.Format(L"%s", szPath);
		m_FilePath.Replace(L"\\", L"/");
		StringToChar(m_FilePath, szPathTemp);
		MyString::myStrSubSplitR(szPathTemp, szFileName, '/');
		szNameTemp.Format("%s", szFileName);
		m_FileInfo.FileName = szNameTemp;
		CFile file(m_FilePath, CFile::modeRead);	//open file	
		m_FileInfo.FileSize = file.GetLength();		//get file zie	
		file.Close();
		//update view 
		CStringA strTempSize;
		strTempSize.Format("%0.2lf", m_FileInfo.FileSize / 1024.0);
		m_FileSize = strTempSize;
		GetDlgItem(IDC_EFILESIZE2)->SetWindowText(m_FileSize + L" k");
		GetDlgItem(IDC_EDOWNLOADPATH2)->SetWindowText(m_FilePath);
		if (m_FileInfo.FileSize / 1024.0 == 0)GetDlgItem(IDC_EFILESIZE2)->SetWindowText(L"0 k");
		GetDlgItem(IDC_ECOMPELETESIZE2)->SetWindowText(L"0 k");
		GetDlgItem(IDC_EINFACTSPEECH2)->SetWindowText(L"0 kb/s");
		
	}
	CDialogEx::OnDropFiles(hDropInfo);
}

void CTranceDialog::OnTimer(UINT_PTR nIDEvent)
{
	
	if(!GetParent()->IsWindowVisible())ShowWindow(SW_HIDE);
	CDialogEx::OnTimer(nIDEvent);
}


LRESULT CTranceDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
	case WM_SYSCOMMAND:
	{
		  if (wParam == SC_CLOSE)
		  {
			  if (m_ReceiveState != L"Free ..." || m_SendState != L"Free ...")
			  {

				  MessageBox(L"please wait translate!!!" + m_ReceiveState + m_SendState);
				  return 0;
			  }
			  this->OnClose();
			  return 0;
		  }
		  else if (wParam == SC_MINIMIZE)
		  {
			  ShowWindow(SW_HIDE);
			  return 0;
		  }
		  break;
	}
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}

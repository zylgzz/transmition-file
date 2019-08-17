// CloserServer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TranceFile001.h"
#include "CloserServer.h"
#include "afxdialogex.h"


// CCloserServer �Ի���

IMPLEMENT_DYNAMIC(CCloserServer, CDialogEx)

CCloserServer::CCloserServer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCloserServer::IDD, pParent)
	, m_port(0)
{

}

CCloserServer::~CCloserServer()
{
}

void CCloserServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_port);
	DDV_MinMaxLong(pDX, m_port, 1, 65536);
}


BEGIN_MESSAGE_MAP(CCloserServer, CDialogEx)
END_MESSAGE_MAP()


// CCloserServer ��Ϣ�������


void CCloserServer::OnOK()
{
	
	UpdateData(TRUE);
	WPARAM port = m_port;
	GetParent()->SendMessage(WM_CLOSESERVER, port,0);
	CDialogEx::OnOK();
}


void CCloserServer::OnCancel()
{
	
	CDialogEx::OnCancel();
}

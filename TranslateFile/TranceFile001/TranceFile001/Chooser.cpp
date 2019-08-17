// Chooser.cpp : 实现文件
//

#include "stdafx.h"
#include "TranceFile001.h"
#include "Chooser.h"
#include "afxdialogex.h"


// CChooser 对话框

IMPLEMENT_DYNAMIC(CChooser, CDialogEx)

CChooser::CChooser(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChooser::IDD, pParent)
	, m_port(0)
	, m_num(0)
{
	m_port = 5657;
	m_num = 1000;

}

CChooser::~CChooser()
{
}

void CChooser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_port);
	DDV_MinMaxInt(pDX, m_port, 1, 65536);
	DDX_Text(pDX, IDC_EDIT1, m_num);
	DDV_MinMaxInt(pDX, m_num, 1, 65536);
}


BEGIN_MESSAGE_MAP(CChooser, CDialogEx)
	ON_BN_CLICKED(IDC_SURE2, &CChooser::OnBnClickedSure)
	ON_BN_CLICKED(IDC_CANNEL, &CChooser::OnBnClickedCannel)
END_MESSAGE_MAP()


// CChooser 消息处理程序


void CChooser::OnBnClickedSure()
{
	OnOK();
}


void CChooser::OnOK()
{
	UpdateData(TRUE);
	WPARAM port = m_port;
	LPARAM num = m_num;
	GetParent()->SendMessage(WM_ADDSERVER, port,num);
	CDialogEx::OnOK();
}


void CChooser::OnCancel()
{
	CDialogEx::OnCancel();
}


void CChooser::OnBnClickedCannel()
{
	OnCancel();
}

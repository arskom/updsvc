
// SettingsDlg.h : header file
//

#pragma once


// CSettingsDlg dialog
class CSettingsDlg : public CDialogEx
{
// Construction
public:
	CSettingsDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
        afx_msg void OnPaint();
        afx_msg HCURSOR OnQueryDragIcon();
        DECLARE_MESSAGE_MAP()
    public:
        afx_msg void OnBnClickedOk();
        afx_msg void OnCbnSelchangeComboPeriod();
        afx_msg void OnCbnSelchangeComboRelchan();
        CComboBox selected_relchan;
        CComboBox selected_period;
        afx_msg void OnEnChangeEdit1();
        afx_msg void OnCbnSelchangeCombo1();
        CComboBox combo_pname;
};

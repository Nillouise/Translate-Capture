
// Translate CaptureView.cpp : implementation of the CTranslateCaptureView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Translate Capture.h"
#endif

#include "Translate CaptureDoc.h"
#include "Translate CaptureView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTranslateCaptureView

IMPLEMENT_DYNCREATE(CTranslateCaptureView, CView)

BEGIN_MESSAGE_MAP(CTranslateCaptureView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTranslateCaptureView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTranslateCaptureView construction/destruction

CTranslateCaptureView::CTranslateCaptureView()
{
	// TODO: add construction code here

}

CTranslateCaptureView::~CTranslateCaptureView()
{
}

BOOL CTranslateCaptureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTranslateCaptureView drawing

void CTranslateCaptureView::OnDraw(CDC* /*pDC*/)
{
	CTranslateCaptureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTranslateCaptureView printing


void CTranslateCaptureView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTranslateCaptureView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTranslateCaptureView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTranslateCaptureView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTranslateCaptureView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTranslateCaptureView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTranslateCaptureView diagnostics

#ifdef _DEBUG
void CTranslateCaptureView::AssertValid() const
{
	CView::AssertValid();
}

void CTranslateCaptureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTranslateCaptureDoc* CTranslateCaptureView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTranslateCaptureDoc)));
	return (CTranslateCaptureDoc*)m_pDocument;
}
#endif //_DEBUG


// CTranslateCaptureView message handlers

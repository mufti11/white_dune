/*
 * dragdrop.cpp
 *
 * Copyright (C) 1999 Stephen F. White
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file "COPYING" for details); if 
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
 * Cambridge, MA 02139, USA.
 */

#include <windows.h>
#include <ole2.h>

#include "swt.h"
#include "swt_private.h"

static int
FromDROPEFFECT(int effect)
{
    int rc = 0;

    if (effect & DROPEFFECT_COPY)
        rc |= SW_DRAG_COPY;
    if (effect & DROPEFFECT_MOVE)
        rc |= SW_DRAG_MOVE;
    if (effect & DROPEFFECT_LINK)
        rc |= SW_DRAG_LINK;
    return rc;
}

static DWORD
ToDROPEFFECT(int effect)
{
    DWORD rc = DROPEFFECT_NONE;

    if (effect & SW_DRAG_COPY)
        rc |= DROPEFFECT_COPY;
    if (effect & SW_DRAG_MOVE)
        rc |= DROPEFFECT_MOVE;
    if (effect & SW_DRAG_LINK)
        rc |= DROPEFFECT_LINK;
    return rc;
}

/*****************/
/* drag und drop */
/*****************/

class swDropSource : public IDropSource, public IDataObject {
public:
    swDropSource(SWND wnd)
    {
        _wnd = wnd;
        _ref = 0;
        _advise = NULL;
    }
    ULONG __stdcall AddRef()
    {
        return ++_ref;
    }
    ULONG __stdcall Release()
    {
        int rc = --_ref;
        if (!rc) delete this;
        return rc;
    }
    STDMETHODIMP QueryInterface( REFIID iid, void ** ppvObject )
    {
        if ( iid == IID_IUnknown || iid == IID_IDropSource || 
             iid == IID_IDataObject) {
            AddRef();
            *ppvObject = this;
            return S_OK;
        } else {
            *ppvObject = NULL;
            return E_NOINTERFACE;
        }
    }
    STDMETHODIMP QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
    {
        if (fEscapePressed)
            return DRAGDROP_S_CANCEL;
        else if (grfKeyState & MK_LBUTTON)
            return S_OK;
        else
            return DRAGDROP_S_DROP;
    }
    
    STDMETHODIMP GiveFeedback(DWORD dwEffect)
    { return DRAGDROP_S_USEDEFAULTCURSORS; }
    STDMETHODIMP DAdvise(FORMATETC FAR* pFormatetc, DWORD advf,
                         LPADVISESINK pAdvSink, DWORD FAR* pdwConnection)
    {
        if (!_advise) CreateDataAdviseHolder(&_advise);
        return _advise->Advise(this, pFormatetc, advf, pAdvSink, pdwConnection);
    }
    STDMETHODIMP DUnadvise(DWORD dwConnection)
    { return _advise->Unadvise(dwConnection); }
    STDMETHODIMP EnumDAdvise(LPENUMSTATDATA FAR* ppenumAdvise)
    { return _advise->EnumAdvise(ppenumAdvise); }
    STDMETHODIMP EnumFormatEtc(DWORD dwDirection,
                               LPENUMFORMATETC FAR* ppenumFormatEtc)
    { *ppenumFormatEtc = NULL; return E_NOTIMPL; }
    STDMETHODIMP GetCanonicalFormatEtc(LPFORMATETC pformatetc,
                                       LPFORMATETC pformatetcOut)
    { pformatetcOut->ptd = NULL; return E_NOTIMPL; }
    STDMETHODIMP GetData(LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium)
    { return DV_E_FORMATETC; }
    STDMETHODIMP GetDataHere(LPFORMATETC pformatetc, LPSTGMEDIUM pmedium)
    { return DV_E_FORMATETC; }
    STDMETHODIMP QueryGetData(LPFORMATETC pformatetc )
    { return E_NOTIMPL; }
    STDMETHODIMP SetData(LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
                         BOOL fRelease)
    { return E_NOTIMPL; }

private:
    SWND _wnd;
    int _ref;
    IDataAdviseHolder *_advise;
};

class swDropTarget : public IDropTarget
{
public:
    swDropTarget(SWND wnd, DropCallback dropCallback)
    {
        _wnd = wnd;
        _dropCallback = dropCallback;
        _ref = 0;
    }
    ULONG __stdcall AddRef()
    {
        return ++_ref;
    }
    ULONG __stdcall Release()
    {
        int rc = --_ref;
        if (!rc) delete this;
        return rc;
    }

    HRESULT __stdcall QueryInterface( REFIID iid, void ** ppvObject )
    {
        if ( iid == IID_IUnknown || iid == IID_IDropTarget) {
            AddRef();
            *ppvObject = this;
            return S_OK;
        } else {
            *ppvObject = NULL;
            return E_NOINTERFACE;
        }
    }
    virtual HRESULT __stdcall DragEnter( IDataObject *pDataObj,
        DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
    {
        POINT p = {pt.x, pt.y};
        ScreenToClient(_wnd->hWnd, &p);
        int rc = _dropCallback(_wnd->data, SW_DRAG_ENTER, p.x, p.y,
                               MODIFIERS(grfKeyState));
        *pdwEffect = ToDROPEFFECT(rc);
        return S_OK;
    }
    virtual HRESULT __stdcall DragOver( DWORD grfKeyState,
        POINTL pt, DWORD *pdwEffect)
    {
        POINT p = {pt.x, pt.y};
        ScreenToClient(_wnd->hWnd, &p);
        int rc = _dropCallback(_wnd->data, SW_DRAG_OVER, p.x, p.y,
                               MODIFIERS(grfKeyState));
        *pdwEffect = ToDROPEFFECT(rc);
        return S_OK;
    }
    virtual HRESULT __stdcall DragLeave(void)
    {
        _dropCallback(_wnd->data, SW_DRAG_LEAVE, 0, 0, 0);
        return S_OK;
    }
    virtual HRESULT __stdcall Drop( IDataObject *pDataObj,
        DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
    {
        POINT p = {pt.x, pt.y};
        ScreenToClient(_wnd->hWnd, &p);
        int rc = _dropCallback(_wnd->data, SW_DROP, p.x, p.y,
                               MODIFIERS(grfKeyState));
        *pdwEffect = ToDROPEFFECT(rc);
        return S_OK;
    }
private:
    SWND         _wnd;
    DropCallback _dropCallback;
    int          _ref;
};

void
swRegisterDropTarget(SWND wnd, DropCallback dropCallback)
{
    if (!wnd || !dropCallback) return;

    wnd->dropTarget = new swDropTarget(wnd, dropCallback);
    wnd->dropTarget->AddRef();
    RegisterDragDrop(wnd->hWnd, wnd->dropTarget);
}

void
swUnregisterDropTarget(SWND wnd)
{
    if (!wnd || !wnd->dropTarget) return;

    wnd->dropTarget->Release();
    RevokeDragDrop(wnd->hWnd);
}

static int inDragDrop;

int
swDragDrop(SWND wnd, int operations, SBITMAP bitmap, SBITMAP mask,
           int x, int y, int width, int height)
{
    DWORD effect; // return code, windows format
    int rc = 0;   // return code, sw format

    swDropSource *dropSource = new swDropSource(wnd);
    dropSource->AddRef();
    DoDragDrop(dropSource, dropSource, ToDROPEFFECT(operations), &effect);
    dropSource->Release();
    return FromDROPEFFECT(effect);
}

/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#ifndef CRYINCLUDE_EDITOR_CONTROLS_SLIDERCTRLEX_H
#define CRYINCLUDE_EDITOR_CONTROLS_SLIDERCTRLEX_H
#pragma once

//////////////////////////////////////////////////////////////////////////
class CSliderCtrlEx
    : public CSliderCtrl
{
public:
    typedef Functor1<CSliderCtrlEx*>    UpdateCallback;

    DECLARE_DYNAMIC(CSliderCtrlEx)
    CSliderCtrlEx();

    //////////////////////////////////////////////////////////////////////////
    virtual void EnableUndo(const CString& undoText);
    virtual void SetUpdateCallback(const UpdateCallback& cb) { m_updateCallback = cb; };

    virtual void SetRangeFloat(float min, float max, float step = 0.f);
    virtual void SetValue(float val);
    virtual float GetValue() const;
    virtual CString GetValueAsString() const;

protected:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()

    bool SetThumb(const CPoint& pt);
    void PostMessageToParent(const int nTBCode);

protected:
    bool m_bDragging;
    bool m_bDragChanged;

    float m_min, m_max;
    mutable float m_value;
    float m_lastUpdateValue;
    CPoint m_mousePos;

    bool m_noNotify;
    bool m_integer;

    bool m_bUndoEnabled;
    bool m_bUndoStarted;
    bool m_bDragged;
    CString m_undoText;
    bool m_bLocked;
    bool m_bInNotifyCallback;

    UpdateCallback m_updateCallback;
};

//////////////////////////////////////////////////////////////////////////
class CSliderCtrlCustomDraw
    : public CSliderCtrlEx
{
public:
    DECLARE_DYNAMIC(CSliderCtrlCustomDraw)

    CSliderCtrlCustomDraw() { m_tickFreq = 1; m_selMin = m_selMax = 0; }
    void SetTicFreq(int nFreq) { m_tickFreq = nFreq; };
    void SetSelection(int nMin, int nMax);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

    void DrawTicks(CDC* pDC);
    void DrawTick(CDC* pDC, int x, bool bMajor = false);
    int ValueToPos(int n);

private:
    int m_selMin, m_selMax;
    int m_tickFreq;
    CRect m_channelRc;
};

#endif // CRYINCLUDE_EDITOR_CONTROLS_SLIDERCTRLEX_H

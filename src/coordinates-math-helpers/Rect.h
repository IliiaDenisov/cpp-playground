#pragma once

#include "Points.h"

struct RECT
{
    long left;
    long top;
    long right;
    long bottom;
};

class CRect : public RECT
{
public:
    CRect()
    {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    }
    CRect(int l, int t, int r, int b)
    {
        left = l;
        top = t;
        right = r;
        bottom = b;
    }
};

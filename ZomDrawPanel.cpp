//
//  ZomDrawPanel.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include "Machine.h"

BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel)
	EVT_PAINT(ZomDrawPanel::PaintEvent)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	PaintNow();
}

void ZomDrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void ZomDrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void ZomDrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw the grid
	DrawGrid(dc);
	DrawWorld(dc);
}

void ZomDrawPanel::DrawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);
	for (int i = 1; i < 20; i++)
	{
		dc.DrawLine(wxPoint(10 + (30 * i), 10), wxPoint(10 + (30 * i), 610));
		dc.DrawLine(wxPoint(10, 10 + (30 * i)), wxPoint(610, (30 * i) + 10));
	}
}

void ZomDrawPanel::DrawWorld(wxDC & dc)
{
	dc.SetPen(*wxBLACK_PEN);
	for (unsigned int i = 0; i < mWorld->GetHumans().size(); i++)
	{
		//calculate head, tail locations
		wxPoint pointList[3];
		wxPoint head;
		wxPoint lTail;
		wxPoint rTail;
		int TLX = (mWorld->GetHumans()[i]->GetXLoc() * 30) + 10;
		int TLY = (mWorld->GetHumans()[i]->GetYLoc() * 30) + 10;
		switch (mWorld->GetHumans()[i]->mFacing)
		{
		case 0:
			head = wxPoint((TLX + 15), TLY);
			lTail = wxPoint(TLX, TLY + 30);
			rTail = wxPoint(TLX + 30, TLY + 30);
			break;
		case 1:
			head = wxPoint(TLX + 30, TLY + 15);
			lTail = wxPoint(TLX, TLY);
			rTail = wxPoint(TLX, TLY + 30);
			break;
		case 2:
			head = wxPoint(TLX + 15, TLY + 30);
			lTail = wxPoint(TLX + 30, TLY);
			rTail = wxPoint(TLX, TLY);
			break;
		case 3:
			head = wxPoint(TLX, TLY + 15);
			lTail = wxPoint(TLX + 30, TLY + 30);
			rTail = wxPoint(TLX + 30, TLY);
			break;
		}
		pointList[0] = head;
		pointList[1] = lTail;
		pointList[2] = rTail;
		dc.SetBrush(*wxBLACK_BRUSH);
		dc.DrawPolygon(3, pointList);
	}
	for (unsigned int i = 0; i < mWorld->GetZombies().size(); i++)
	{
		//calculate head, tail locations
		wxPoint pointList[3];
		wxPoint head;
		wxPoint lTail;
		wxPoint rTail;
		int TLX = (mWorld->GetZombies()[i]->GetXLoc() * 30) + 10;
		int TLY = (mWorld->GetZombies()[i]->GetYLoc() * 30) + 10;
		switch (mWorld->GetZombies()[i]->mFacing)
		{
		case 0:
			head = wxPoint((TLX + 15), TLY);
			lTail = wxPoint(TLX, TLY + 30);
			rTail = wxPoint(TLX + 30, TLY + 30);
			break;
		case 1:
			head = wxPoint(TLX + 30, TLY + 15);
			lTail = wxPoint(TLX, TLY);
			rTail = wxPoint(TLX, TLY + 30);
			break;
		case 2:
			head = wxPoint(TLX + 15, TLY + 30);
			lTail = wxPoint(TLX + 30, TLY);
			rTail = wxPoint(TLX, TLY);
			break;
		case 3:
			head = wxPoint(TLX, TLY + 15);
			lTail = wxPoint(TLX + 30, TLY + 30);
			rTail = wxPoint(TLX + 30, TLY);
			break;
		}
		pointList[0] = head;
		pointList[1] = lTail;
		pointList[2] = rTail;
		dc.SetBrush(*wxGREEN_BRUSH);
 		dc.DrawPolygon(3, pointList);
	}

	dc.DrawText("Human file loaded: " + mWorld->GetHumFile(), wxPoint(615, 100));
	dc.DrawText("Zombie file loaded: " + mWorld->GetZomFile(), wxPoint(615, 120));

	dc.DrawText("Humans in world: " + std::to_string(mWorld->GetHumans().size()), wxPoint(615, 160));
	dc.DrawText("Zombies in world: " + std::to_string(mWorld->GetZombies().size()), wxPoint(615, 180));

	dc.DrawText("Current month: " + std::to_string(mWorld->GetMonth()), wxPoint(615, 220));
}

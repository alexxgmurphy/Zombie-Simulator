//
//  ZomFrame.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wx/dc.h>
enum
{
	ID_SImSTART = 1000,
	ID_RESET = 2000,
	ID_LOADZOM = 3000,
	ID_LOADHUM = 4000,
	ID_RANDOMIZE = 5000,
	ID_TURN_TIMER,
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame)
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SImSTART, ZomFrame::OnSimStart)
	EVT_MENU(ID_LOADZOM, ZomFrame::OnLoadZom)
	EVT_MENU(ID_RESET, ZomFrame::OnReset)
	EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
	EVT_MENU(ID_LOADHUM, ZomFrame::OnLoadHum)
wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
, mIsActive(false)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(wxID_EXIT);
	
	// Simulation menu
	mSimMenu = new wxMenu;
	mSimMenu->Append(ID_SImSTART, "Start/stop\tSpace", "Start or stop the simulation");
	mSimMenu->Enable(ID_SImSTART, false);
	mSimMenu->Append(ID_LOADZOM, "Load Zombie...");
	mSimMenu->Append(ID_LOADHUM, "Load Human...");
	mSimMenu->Append(ID_RANDOMIZE, "Randomize");
	mSimMenu->Enable(ID_RANDOMIZE, false);
	mSimMenu->Append(ID_RESET, "Reset");
	mSimMenu->Enable(ID_RESET, false);
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(mSimMenu, "&Simulation");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new ZomDrawPanel(this);
	mPanel->SetWorld(mWorld);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	mTurnTimer = new wxTimer(this, ID_TURN_TIMER);
}

void ZomFrame::OnReset(wxCommandEvent &event)
{
	//delete current zomMachine, humanMachine and all states; delete world; create new of all
	mWorld->SetMonth(0);
	for (int i = 0; i < 20; i++)
	{
		for (int v = 0; v < 20; v++)
		{
			mWorld->RemoveAtLoc(i, v);
		}
	}
	OnRandomize(event);


	mPanel->PaintNow();

	mTurnTimer->Stop();
}

ZomFrame::~ZomFrame()
{
	delete mTurnTimer;
}

void ZomFrame::OnLoadZom(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Choose zombie zom file"), "./data", "", "zom files (*.zom)|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	zomFile = std::string(openFileDialog.GetPath());

	mZomMach->SetWorld(mWorld);
	mZomMach->LoadMachine(zomFile);
	mWorld->SetZomFile(zomFile);

	mPanel->PaintNow();

	mSimMenu->Enable(ID_RANDOMIZE, true);
}

void ZomFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::OnLoadHum(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Choose human zom file"), "./data", "", "zom files (*.zom)|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	humFile = std::string(openFileDialog.GetPath());
	mHumMach->SetWorld(mWorld);
	mHumMach->LoadMachine(humFile);
	mWorld->SetHumFile(humFile);

	mPanel->PaintNow();

	mSimMenu->Enable(ID_RANDOMIZE, true);
}

void ZomFrame::OnNew(wxCommandEvent& event)
{
	//delete current zomMachine, humanMachine and all states; delete world; create new of all
	mZomMach.~shared_ptr();
	mZomMach = std::make_shared<Machine<ZombieTraits>>();

	mHumMach.~shared_ptr();
	mHumMach = std::make_shared<Machine<HumanTraits>>();

	mWorld->ClearWorld();

	mPanel->PaintNow();

	mTurnTimer->Stop();
	mSimMenu->Enable(ID_SImSTART, false);
	mSimMenu->Enable(ID_RESET, false);
	mSimMenu->Enable(ID_RANDOMIZE, false);
}

void ZomFrame::OnSimStart(wxCommandEvent& event)
{
	mPanel->PaintNow();
	srand(time(NULL));
	if (!mIsActive)
	{
		mPanel->PaintNow();
		// Add timer to run once per second
		mTurnTimer->Start(1000);
		mWorld->SetMonth(0);
		mIsActive = true;
		mEverActive = true;
	}
	else
	{
		mTurnTimer->Stop();
		mIsActive = false;
	}
}

void ZomFrame::OnTurnTimer(wxTimerEvent& event)
{
	try
	{
		if (mWorld->GetHumans().size() == 0 || mWorld->GetZombies().size() == 0)
		{
			OnSimStart(wxCommandEvent());
			return;
		}
		mWorld->SetMonth(mWorld->GetMonth() + 1);
		for (unsigned int i = 0; i < mZomMach->GetCharacters().size(); i++)
		{
			mZomMach->TakeTurn(*mZomMach->GetCharacters()[i]);
		}
		mZomMach->SetCharacters(mZomMach->GetWorld()->GetZombies());
		mPanel->PaintNow();

		for (unsigned int i = 0; i < mHumMach->GetCharacters().size(); i++)
		{
			mHumMach->TakeTurn(*mHumMach->GetCharacters()[i]);
		}
		mHumMach->SetCharacters(mHumMach->GetWorld()->GetHumans());
		mPanel->PaintNow();
	}
	catch (InvalidOp &e)
	{
		mTurnTimer->Stop();
		mIsActive = false;
		wxMessageBox(e.what(), "Error Running Simulation");
	}
}

void ZomFrame::OnRandomize(wxCommandEvent& event)
{
	//Populate location vector
	std::vector<std::vector<bool>> availableLocs;
	for (int i = 0; i < 20; i++)
	{
		availableLocs.push_back({});
		for (int v = 0; v < 20; v++)
		{
			availableLocs[i].push_back(true);
		}
	}
	for (int i = 0; i < 20; i++)
	{
		availableLocs.push_back({});
		for (int v = 0; v < 20; v++)
		{
			availableLocs[i].push_back(true);
		}
	}
	int x = rand() % 20;
	int y = rand() % 20;

	for (unsigned int i = 0; i < 10; i++)
	{
		auto newHum = std::make_shared<MachineState>();
		int direction = rand() % 4;
		switch (direction)
		{
		case (0) :
			newHum->mFacing = MachineState::RIGHT;
			break;
		case (1) :
			newHum->mFacing = MachineState::DOWN;
			break;
		case (2) :
			newHum->mFacing = MachineState::LEFT;
			break;
		default:
		case (3) :
			newHum->mFacing = MachineState::UP;
			break;
		}
		while (!availableLocs[y][x])
		{
			x = rand() % 20;
			y = rand() % 20;
		}
		newHum->SetXLoc(x);
		newHum->SetYLoc(y);
		availableLocs[y][x] = false;
		newHum->SetHMachine(mHumMach);
		newHum->SetZMachine(mZomMach);
		newHum->SetWorld(mWorld);
		mHumMach->AddCharacter(newHum);
		mHumMach->BindState(*newHum);
		mWorld->AddHuman(newHum);
	}

	//Populate location vector
	availableLocs.clear();
	for (int i = 0; i < 20; i++)
	{
		availableLocs.push_back({});
		for (int v = 0; v < 20; v++)
		{
			availableLocs[i].push_back(true);
		}
	}

	for (unsigned int i = 0; i < 20; i++)
	{
		// Create new zombie
		auto newZom = std::make_shared<MachineState>();
		int direction = rand() % 4;
		switch (direction)
		{
		case (0) :
			newZom->mFacing = MachineState::RIGHT;
			break;
		case (1) :
			newZom->mFacing = MachineState::DOWN;
			break;
		case (2) :
			newZom->mFacing = MachineState::LEFT;
			break;
		default:
		case (3) :
			newZom->mFacing = MachineState::UP;
			break;
		}
		int x = rand() % 20;
		int y = rand() % 20;
		while (!availableLocs[y][x])
		{
			x = rand() % 20;
			y = rand() % 20;
		}
		newZom->SetXLoc(x);
		newZom->SetYLoc(y);
		availableLocs[y][x] = false;
		newZom->SetHMachine(mHumMach);
		newZom->SetZMachine(mZomMach);
		newZom->SetWorld(mWorld);
		mZomMach->AddCharacter(newZom);
		mZomMach->BindState(*newZom);
		mWorld->AddZombie(newZom);
	}
	
	mPanel->PaintNow();
	mSimMenu->Enable(ID_SImSTART, true);
	mSimMenu->Enable(ID_RESET, true);
}

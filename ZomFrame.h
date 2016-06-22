//
//  ZomFrame.h
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/frame.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include "Machine.h"
#include "Traits.h"
#include "World.h"

class ZomFrame : public wxFrame
{
public:
	ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~ZomFrame();

private:
	void OnExit(wxCommandEvent& event);
	void OnNew(wxCommandEvent& event);
	void OnSimStart(wxCommandEvent& event);
	void OnTurnTimer(wxTimerEvent& event);
	void OnLoadZom(wxCommandEvent& event);
	void OnReset(wxCommandEvent& event);
	void OnLoadHum(wxCommandEvent& event);
	void OnRandomize(wxCommandEvent& event);
	
	wxDECLARE_EVENT_TABLE();
private:
	// Panel for drawing
	class ZomDrawPanel* mPanel;
	
	// Simulation menu
	wxMenu* mSimMenu;
	
	// Turn timer
	wxTimer* mTurnTimer;

	//Files to load
	std::string zomFile = " ";
	std::string humFile = " ";
	
	// Initiate vectors for machine states, machines, and world singleton
	std::shared_ptr<Machine<ZombieTraits>> mZomMach = std::make_shared<Machine<ZombieTraits>>();
	std::shared_ptr<Machine<HumanTraits>> mHumMach = std::make_shared<Machine<HumanTraits>>();

	std::shared_ptr<World> mWorld = std::make_shared<World>();

	bool mEverActive = false;
	
	bool mIsActive;
};

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "Op.h"
#include "Exceptions.h"
#include <fstream>
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>
#include "World.h"
#include "Traits.h"

#ifndef MACHINE_H
#define MACHINE_H

class World;
// Defines state data accessible by the machine and ops
struct MachineState
{
	// Friend Machine so BindState works
	template <typename MachineTraits>
	friend class Machine;

	//directional enum
	enum Facing { UP, RIGHT, DOWN, LEFT };

	//default constructor
	MachineState()
		: mProgramCounter(1)
		, mActionsTaken(0)
		, mFacing(UP)
		, mTest(false)
	{ }

	//deconstructor
	~MachineState()
	{

	}

	// Active line number in behavior program
	int mProgramCounter;
	// Number of actions taken this turn
	int mActionsTaken;
	// Current facing of this character
	Facing mFacing;
	// Test flag for branches
	bool mTest;

	//getters
	int GetActionsPerTurn() const noexcept { return mActionsPerTurn; }
	bool GetInfect() const noexcept { return mInfectOnAttack; }
	int GetXLoc() const noexcept { return mLocation.first; }
	int GetYLoc() const noexcept { return mLocation.second; }
	std::shared_ptr<World> GetWorld() { return mWorld; }
	std::shared_ptr<Machine<ZombieTraits>> GetZMachine() { return mZomMachine; }
	std::shared_ptr<Machine<HumanTraits>> GetHMachine() { return mHumMachine; }

	//setters
	void SetWorld(std::shared_ptr<World> &world) { mWorld = world; }
	void SetXLoc(int newX) { mLocation.first = newX; }
	void SetYLoc(int newY) { mLocation.second = newY; }
	void SetZMachine(std::shared_ptr<Machine<ZombieTraits>> zMachine) { mZomMachine = zMachine; }
	void SetHMachine(std::shared_ptr<Machine<HumanTraits>> hMachine) { mHumMachine = hMachine; }

private:
	// Data which is set by the traits
	int mActionsPerTurn;
	bool mInfectOnAttack;

	// Data which is set by setters
	std::pair<int, int> mLocation = std::make_pair(0, 0);
	std::shared_ptr<World> mWorld = nullptr;
	std::shared_ptr<Machine<ZombieTraits>> mZomMachine = nullptr;
	std::shared_ptr<Machine<HumanTraits>> mHumMachine = nullptr;
};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
public:
	// Load in all the ops for this machine from the specified file
	void LoadMachine(const std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);

	// Getters
	std::vector<std::shared_ptr<MachineState>> GetCharacters() { return mCharacters; }
	std::shared_ptr<World> GetWorld() { return mWorld; }

	// Setters
	void SetCharacters(std::vector<std::shared_ptr<MachineState>> &newChars) { mCharacters = newChars; }
	void SetWorld(std::shared_ptr<World> &world) { mWorld = world; }

	// Add character
	void AddCharacter(std::shared_ptr<MachineState> &character) { mCharacters.push_back(character); }

	// Destructor
	~Machine();
private:
	std::vector<std::shared_ptr<Op>> mOps;
	std::vector<std::shared_ptr<MachineState>> mCharacters;
	std::shared_ptr<World> mWorld = std::make_shared<World>();
};

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const std::string& filename)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit);
	try
	{
		file.open(filename);
		if (file.is_open())
		{
			std::string line;
			std::vector<std::string> ops;
			while (!file.eof())
			{
				std::getline(file, line);
				wxStringTokenizer tok(line, ",");
				int count = 0;
				std::string finalOp;
				while (tok.HasMoreTokens())
				{
					count++;
					wxString token = tok.NextToken();
					std::string tokString = token.ToStdString();
					std::size_t comment = tokString.find(";");
					if (comment == std::string::npos)
					{
						finalOp = static_cast<std::string>(token.mb_str());
						for (unsigned int i = 0; i < finalOp.size(); i++)
						{
							if (finalOp[i] == ' ' || finalOp[i] == '\r' || finalOp[i] == '\n' || finalOp[i] == '\t')
							{
								finalOp.erase(finalOp.begin() + i);
							}
						}
						if (ops.size() < 2)
						{
							ops.push_back(finalOp);
						}
					}
					else
					{
						wxStringTokenizer sepComm(tokString, ";");
						wxString nonComm = sepComm.NextToken();
						finalOp = static_cast<std::string>(nonComm.mb_str());
						for (unsigned int i = 0; i < finalOp.size(); i++)
						{
							if (finalOp[i] == ' ' || finalOp[i] == '\r' || finalOp[i] == '\n' || finalOp[i] == '\t')
							{
								finalOp.erase(finalOp.begin() + i);
							}
						}
						if (ops.size() < 2)
						{
							ops.push_back(finalOp);
						}
					}
				}
				if (ops.size() == 1)
				{
					ops.push_back("-1");
				}
				for (unsigned int i = 0; i < ops.size(); i++)
				{
					std::string key = ops[i];
					if (key == "goto")
					{
						i++;
						auto curr = std::make_shared<OpGoto>(stoi(ops[i]));
						mOps.push_back(curr);
					}
					else if (key == "rotate")
					{
						i++;
						auto curr = std::make_shared<OpRotate>(stoi(ops[i]));
						mOps.push_back(curr);
					}
					else if (key == "forward")
					{
						i++;
						auto curr = std::make_shared<OpForward>();
						mOps.push_back(curr);
					}
					else if (key == "endturn")
					{
						i++;
						auto curr = std::make_shared<OpEndTurn>();
						mOps.push_back(curr);
					}
					else if (key == "test_human")
					{
						i++;
						auto curr= std::make_shared<OpTestHuman>(stoi(ops[i]));
						mOps.push_back(curr);
					}
					else if (key == "test_wall")
					{
						i++;
						auto curr = std::make_shared<OpTestWall>();
						mOps.push_back(curr);
					}
					else if (key == "test_zombie")
					{
						i++;
						auto curr = std::make_shared<OpTestZombie>(stoi(ops[i]));
						mOps.push_back(curr);
					}
					else if (key == "test_random")
					{
						i++;
						auto curr = std::make_shared<OpTestRandom>();
						mOps.push_back(curr);
					}
					else if (key == "test_passable")
					{
						i++;
						auto curr = std::make_shared<OpTestPassable>();
						mOps.push_back(curr);
					}
					else if (key == "je")
					{
						i++;
						auto curr = std::make_shared<OpJe>(stoi(ops[i]));
						mOps.push_back(curr);
					}
					else if (key == "jne")
					{
						i++;
						auto curr = std::make_shared<OpJne>(stoi(ops[i]));
						mOps.push_back(curr);
					}
					else if (key == "attack")
					{
						i++;
						auto curr = std::make_shared<OpAttack>();
						mOps.push_back(curr);
					}
					else if (key == "ranged_attack")
					{
						i++;
						auto curr = std::make_shared<OpRangedAttack>();
						mOps.push_back(curr);
					}
				}
				ops.clear();
			}
		}
		else
		{
			throw FileOpenErr();
		}
	}
	catch (std::ifstream::failure &e)
	{
		e.what(); //I don't like the error text but it won't run if e is unused
		throw FileLoadErr();
	}
	catch (FileLoadErr &e)
	{
		wxMessageBox(e.what(), "File Not Found");
	}
	catch (FileOpenErr &e)
	{
		wxMessageBox(e.what(), "Error Opening File");
	}
	catch (std::exception &e)
	{
		wxMessageBox(e.what(), "Error");
	}
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.mActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.mInfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	try
	{
		state.mActionsTaken = 0;
		while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
		{
			if (state.mProgramCounter == mOps.size())
			{
				state.mProgramCounter = 1;
			}
			mOps.at(state.mProgramCounter - 1)->Execute(state);
		}
	}
	catch (std::exception &e)
	{
		throw InvalidOp();
		e.what();
	}
}

template <typename MachineTraits>
Machine<MachineTraits>::~Machine()
{
	mOps.clear();
}
#endif;
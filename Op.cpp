#include "Op.h"
#include "Machine.h"
#include <iostream>
#include <random>

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	std::cout << state.mProgramCounter << ":" << " Character at location (" << state.GetXLoc() << ", " << state.GetYLoc() << ") " << mOpName << "," << mParam << std::endl;
}

void OpRotate::Execute(MachineState& state)
{
	//DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::RIGHT;
		}
		else
		{
			state.mFacing = MachineState::LEFT;
		}
		break;
	case (MachineState::RIGHT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::DOWN;
		}
		else
		{
			state.mFacing = MachineState::UP;
		}
		break;
	case (MachineState::DOWN) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::LEFT;
		}
		else
		{
			state.mFacing = MachineState::RIGHT;
		}
		break;
	default:
	case (MachineState::LEFT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::UP;
		}
		else
		{
			state.mFacing = MachineState::DOWN;
		}
		break;
	}

	/*std::cout << "Now facing ";
	switch (state.mFacing)
	{
	case (MachineState::UP):
		std::cout << "UP";
		break;
	case (MachineState::RIGHT):
		std::cout << "RIGHT";
		break;
	case (MachineState::DOWN):
		std::cout << "DOWN";
		break;
	case (MachineState::LEFT):
		std::cout << "LEFT";
		break;
	}
	std::cout << std::endl;*/
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
	//DebugOutput(state);
	state.mProgramCounter = mParam;
}

void OpForward::Execute(MachineState & state)
{
	//DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;
	switch (state.mFacing)
	{
	case 0:
		if (state.GetYLoc() > 0 && state.GetWorld()->CheckLoc(state.GetXLoc(), state.GetYLoc() - 1) == "none")
		{
			state.SetYLoc(state.GetYLoc() - 1);
			break;
		}
		else
		{
			break;
		}
	case 1:
		if (state.GetXLoc() < 20 && state.GetWorld()->CheckLoc(state.GetXLoc() + 1, state.GetYLoc()) == "none")
		{
			state.SetXLoc(state.GetXLoc() + 1);
		}
		break;
	case 2:
		if (state.GetYLoc() < 20 && state.GetWorld()->CheckLoc(state.GetXLoc(), state.GetYLoc() + 1) == "none")
		{
			state.SetYLoc(state.GetYLoc() + 1);
		}
		break;
	case 3:
		if (state.GetXLoc() > 0 && state.GetWorld()->CheckLoc(state.GetXLoc() - 1, state.GetYLoc()) == "none")
		{
			state.SetXLoc(state.GetXLoc() - 1);
		}
		break;
	}
}

void OpEndTurn::Execute(MachineState& state)
{
	//DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken = state.GetActionsPerTurn();
}

void OpTestHuman::Execute(MachineState& state)
{
	//DebugOutput(state);
	state.mProgramCounter++;
	try
	{
		switch (state.mFacing)
		{
		case 0:
			if (state.GetWorld()->CheckLoc(state.GetXLoc(), state.GetYLoc() - mParam) == "human")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 1:
			if (state.GetWorld()->CheckLoc(state.GetXLoc() + mParam, state.GetYLoc()) == "human")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 2:
			if (state.GetWorld()->CheckLoc(state.GetXLoc(), state.GetYLoc() + mParam) == "human")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 3:
			if (state.GetWorld()->CheckLoc(state.GetXLoc() - mParam, state.GetYLoc()) == "human")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		}
	}
	catch (std::exception &e)
	{
		e.what();
		throw InvalidOp();
	}
}

void OpTestWall::Execute(MachineState &state)
{
	//DebugOutput(state);
	state.mProgramCounter++;
	try
	{
		switch (state.mFacing)
		{
		case 0:
			if (state.GetWorld()->CheckLoc(state.GetXLoc(), state.GetYLoc() - 1) == "wall")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 1:
			if (state.GetWorld()->CheckLoc(state.GetXLoc() + 1, state.GetYLoc()) == "wall")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 2:
			if (state.GetWorld()->CheckLoc(state.GetXLoc(), state.GetYLoc() + 1) == "wall")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 3:
			if (state.GetWorld()->CheckLoc(state.GetXLoc() - 1, state.GetYLoc()) == "wall")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		}
	}
	catch (std::exception &e)
	{
		e.what();
		throw InvalidOp();
	}
}

void OpTestZombie::Execute(MachineState& state)
{
	//DebugOutput(state);
	state.mProgramCounter++;
	try
	{
		switch (state.mFacing)
		{
		case 0:
			if (state.GetWorld()->CheckLoc(state.GetXLoc(), state.GetYLoc() - mParam) == "zombie")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 1:
			if (state.GetWorld()->CheckLoc(state.GetXLoc() + mParam, state.GetYLoc()) == "zombie")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 2:
			if (state.GetWorld()->CheckLoc(state.GetXLoc(), state.GetYLoc() + mParam) == "zombie")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 3:
			if (state.GetWorld()->CheckLoc(state.GetXLoc() - mParam, state.GetYLoc()) == "zombie")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		}
	}
	catch (std::exception &e)
	{
		e.what();
		throw InvalidOp();
	}
}

void OpTestRandom::Execute(MachineState& state)
{
	//DebugOutput(state);
	state.mProgramCounter++;
	srand(time(NULL));
	int random = rand() % 2;
	if (random == 0)
	{
		state.mTest = true;
	}
	else if (random == 1)
	{
		state.mTest = false;
	}
}

void OpTestPassable::Execute(MachineState& state)
{
	//DebugOutput(state);
	state.mProgramCounter++;
	try
	{
		switch (state.mFacing)
		{
		case 0:
			if (state.GetWorld()->CheckLoc(state.GetXLoc(), state.GetYLoc() - 1) == "none")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 1:
			if (state.GetWorld()->CheckLoc(state.GetXLoc() + 1, state.GetYLoc()) == "none")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 2:
			if (state.GetWorld()->CheckLoc(state.GetXLoc(), state.GetYLoc() + 1) == "none")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		case 3:
			if (state.GetWorld()->CheckLoc(state.GetXLoc() - 1, state.GetYLoc()) == "none")
			{
				state.mTest = true;
			}
			else
			{
				state.mTest = false;
			}
			break;
		}
	}
	catch (std::exception &e)
	{
		e.what();
		throw InvalidOp();
	}
}

void OpJe::Execute(MachineState& state)
{
	//DebugOutput(state);
	try
	{
		if (state.mTest)
		{
			state.mProgramCounter = mParam;
			state.mTest = false;
		}
		else
		{
			state.mProgramCounter++;
		}
	}
	catch (std::exception &e)
	{
		e.what();
		throw InvalidOp();
	}
}

void OpJne::Execute(MachineState &state)
{
	//DebugOutput(state);
	try
	{
		if (!state.mTest)
		{
			state.mProgramCounter = mParam;
		}
		else
		{
			state.mProgramCounter++;
			state.mTest = false;
		}
	}
	catch (std::exception &e)
	{
		e.what();
		throw InvalidOp();
	}
}

void OpAttack::Execute(MachineState &state)
{
	//DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;
	std::shared_ptr<MachineState> enemy = nullptr;
	switch (state.mFacing)
	{
	case 0:
		enemy = state.GetWorld()->AtLoc(state.GetXLoc(), state.GetYLoc() - 1);
		if (enemy != nullptr)
		{
			//if enemy is a zombie and state is not a zombie, zombie disappears
			if (enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc(), state.GetYLoc() - 1);
			}
			//if enemy is not a zombie and state is a zombie, human disappears and zombie appears in its place
			else if (!enemy->GetInfect() && state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc(), state.GetYLoc() - 1);
				//create zombie at location state.GetXLoc(), state.GetYLoc()-1
				std::shared_ptr<MachineState> newZombie = std::make_shared<MachineState>();
				newZombie->SetXLoc(state.GetXLoc());
				newZombie->SetYLoc(state.GetYLoc() - 1);
				newZombie->SetWorld(state.GetWorld());
				state.GetZMachine()->AddCharacter(newZombie);
				state.GetWorld()->AddZombie(newZombie);
				state.GetZMachine()->BindState(*newZombie);
				newZombie->SetHMachine(state.GetHMachine());
				newZombie->SetZMachine(state.GetZMachine());
			}
			//if enemy and state are both zombies, do nothing
			//if enemy and state are humans, enemy disappears
			else if (!enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc(), state.GetYLoc() - 1);
			}
		}
		break;
	case 1:
		enemy = state.GetWorld()->AtLoc(state.GetXLoc() + 1, state.GetYLoc());
		if (enemy != nullptr)
		{
			//if enemy is a zombie and state is not a zombie, zombie disappears
			if (enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc() + 1, state.GetYLoc());
			}
			//if enemy is not a zombie and state is a zombie, human disappears and zombie appears in its place
			else if (!enemy->GetInfect() && state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc() + 1, state.GetYLoc());
				//create zombie at location state.GetXLoc() + 1, state.GetYLoc()
				std::shared_ptr<MachineState> newZombie = std::make_shared<MachineState>();
				newZombie->SetXLoc(state.GetXLoc() + 1);
				newZombie->SetYLoc(state.GetYLoc());
				newZombie->SetWorld(state.GetWorld());
				state.GetZMachine()->AddCharacter(newZombie);
				state.GetWorld()->AddZombie(newZombie);
				state.GetZMachine()->BindState(*newZombie);
				newZombie->SetHMachine(state.GetHMachine());
				newZombie->SetZMachine(state.GetZMachine());
			}
			//if enemy and state are both zombies, do nothing
			//if enemy and state are humans, enemy disappears
			else if (!enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc() + 1, state.GetYLoc());
			}
		}
		break;
	case 2:
		enemy = state.GetWorld()->AtLoc(state.GetXLoc(), state.GetYLoc() + 1);
		if (enemy != nullptr)
		{
			//if enemy is a zombie and state is not a zombie, zombie disappears
			if (enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc(), state.GetYLoc() + 1);
			}
			//if enemy is not a zombie and state is a zombie, human disappears and zombie appears in its place
			else if (!enemy->GetInfect() && state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc(), state.GetYLoc() + 1);
				//create zombie at location state.GetXLoc(), state.GetYLoc()-1
				std::shared_ptr<MachineState> newZombie = std::make_shared<MachineState>();
				newZombie->SetXLoc(state.GetXLoc());
				newZombie->SetYLoc(state.GetYLoc() + 1);
				newZombie->SetWorld(state.GetWorld());
				state.GetZMachine()->AddCharacter(newZombie);
				state.GetWorld()->AddZombie(newZombie);
				state.GetZMachine()->BindState(*newZombie);
				newZombie->SetHMachine(state.GetHMachine());
				newZombie->SetZMachine(state.GetZMachine());
			}
			//if enemy and state are both zombies, do nothing
			//if enemy and state are humans, enemy disappears
			else if (!enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc(), state.GetYLoc() + 1);
			}
		}
		break;
	case 3:
		enemy = state.GetWorld()->AtLoc(state.GetXLoc() - 1, state.GetYLoc());
		if (enemy != nullptr)
		{
			//if enemy is a zombie and state is not a zombie, zombie disappears
			if (enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc() - 1, state.GetYLoc());
			}
			//if enemy is not a zombie and state is a zombie, human disappears and zombie appears in its place
			else if (!enemy->GetInfect() && state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc() - 1, state.GetYLoc());
				//create zombie at location state.GetXLoc(), state.GetYLoc()-1
				std::shared_ptr<MachineState> newZombie = std::make_shared<MachineState>();
				newZombie->SetXLoc(state.GetXLoc() - 1);
				newZombie->SetYLoc(state.GetYLoc());
				newZombie->SetWorld(state.GetWorld());
				state.GetZMachine()->AddCharacter(newZombie);
				state.GetWorld()->AddZombie(newZombie);
				state.GetZMachine()->BindState(*newZombie);
				newZombie->SetHMachine(state.GetHMachine());
				newZombie->SetZMachine(state.GetZMachine());
			}
			//if enemy and state are both zombies, do nothing
			//if enemy and state are humans, enemy disappears
			else if (!enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc() - 1, state.GetYLoc());
			}
		}
		break;
	}
}

void OpRangedAttack::Execute(MachineState &state)
{
	//DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;
	std::shared_ptr<MachineState> enemy = nullptr;
	switch (state.mFacing)
	{
	case 0:
		enemy = state.GetWorld()->AtLoc(state.GetXLoc(), state.GetYLoc() - 2);
		if (enemy != nullptr)
		{
			//if enemy is a zombie and state is not a zombie, zombie disappears
			if (enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc(), state.GetYLoc() - 2);
			}
			//if enemy is not a zombie and state is a zombie, exception thrown
			else if (!enemy->GetInfect() && state.GetInfect())
			{
				throw InvalidOp();
			}
			//if enemy and state are both zombies, exception thrown
			else if (enemy->GetInfect() && state.GetInfect())
			{
				throw InvalidOp();
			}
			//if enemy and state are humans, enemy disappears
			else if (!enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc(), state.GetYLoc() - 2);
			}
		}
		break;
	case 1:
		enemy = state.GetWorld()->AtLoc(state.GetXLoc() + 2, state.GetYLoc());
		if (enemy != nullptr)
		{
			//if enemy is a zombie and state is not a zombie, zombie disappears
			if (enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc() + 2, state.GetYLoc());
			}
			//if enemy is not a zombie and state is a zombie, exception thrown
			else if (!enemy->GetInfect() && state.GetInfect())
			{
				throw InvalidOp();
			}
			//if enemy and state are both zombies, exception thrown
			else if (enemy->GetInfect() && state.GetInfect())
			{
				throw InvalidOp();
			}
			//if enemy and state are humans, enemy disappears
			else if (!enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc() + 2, state.GetYLoc());
			}
		}
		break;
	case 2:
		enemy = state.GetWorld()->AtLoc(state.GetXLoc(), state.GetYLoc() + 2);
		if (enemy != nullptr)
		{
			//if enemy is a zombie and state is not a zombie, zombie disappears
			if (enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc(), state.GetYLoc() + 2);
			}
			//if enemy is not a zombie and state is a zombie, exception thrown
			else if (!enemy->GetInfect() && state.GetInfect())
			{
				throw InvalidOp();
			}
			//if enemy and state are both zombies, exception thrown
			else if (enemy->GetInfect() && state.GetInfect())
			{
				throw InvalidOp();
			}
			//if enemy and state are humans, enemy disappears
			else if (!enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc(), state.GetYLoc() + 2);
			}
		}
		break;
	case 3:
		enemy = state.GetWorld()->AtLoc(state.GetXLoc() - 2, state.GetYLoc());
		if (enemy != nullptr)
		{
			//if enemy is a zombie and state is not a zombie, zombie disappears
			if (enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc() - 2, state.GetYLoc());
			}
			//if enemy is not a zombie and state is a zombie, exception thrown
			else if (!enemy->GetInfect() && state.GetInfect())
			{
				throw InvalidOp();
			}
			//if enemy and state are both zombies, exception thrown
			else if (enemy->GetInfect() && state.GetInfect())
			{
				throw InvalidOp();
			}
			//if enemy and state are humans, enemy disappears
			else if (!enemy->GetInfect() && !state.GetInfect())
			{
				state.GetWorld()->RemoveAtLoc(state.GetXLoc() - 2, state.GetYLoc());
			}
		}
		break;
	}
}
#include "World.h"


void World::AddZombie(std::shared_ptr<MachineState>& zom)
{
	mZombies.push_back(zom);
}

void World::AddHuman(std::shared_ptr<MachineState>& hum)
{
	mHumans.push_back(hum);
}

void World::SetZomFile(std::string & zomFile)
{
	std::size_t found = zomFile.find_last_of("/\\");
	mZomFile = zomFile.substr(found + 1);
}

void World::SetHumFile(std::string & humFile)
{
	std::size_t found = humFile.find_last_of("/\\");
	mHumFile = humFile.substr(found + 1);
}

std::string World::CheckLoc(int x, int y)
{
	if (x == -1 || y == -1 || x == 20 || y == 20)
	{
		return "wall";
	}
	else
	{
		for (unsigned int i = 0; i < mZombies.size(); i++)
		{
			if (mZombies[i]->GetXLoc() == x && mZombies[i]->GetYLoc() == y)
			{
				return "zombie";
			}
		}
		for (unsigned int v = 0; v < mHumans.size(); v++)
		{
			if (mHumans[v]->GetXLoc() == x && mHumans[v]->GetYLoc() == y)
			{
				return "human";
			}
		}
		return "none";
	}
}

std::shared_ptr<MachineState> World::AtLoc(int x, int y)
{
	if (x == -1 || y == -1 || x == 20 || y == 20)
	{
		return nullptr;
	}
	else
	{
		for (unsigned int i = 0; i < mZombies.size(); i++)
		{
			if (mZombies[i]->GetXLoc() == x && mZombies[i]->GetYLoc() == y)
			{
				return mZombies[i];
			}
		}
		for (unsigned int v = 0; v < mHumans.size(); v++)
		{
			if (mHumans[v]->GetXLoc() == x && mHumans[v]->GetYLoc() == y)
			{
				return mHumans[v];
			}
		}
		return nullptr;
	}
}

void World::RemoveAtLoc(int x, int y)
{
	for (unsigned int i = 0; i < mZombies.size(); i++)
	{
		if (mZombies[i]->GetXLoc() == x && mZombies[i]->GetYLoc() == y)
		{
			mZombies.erase(mZombies.begin() + i);
		}
	}
	for (unsigned int v = 0; v < mHumans.size(); v++)
	{
		if (mHumans[v]->GetXLoc() == x && mHumans[v]->GetYLoc() == y)
		{
			mHumans.erase(mHumans.begin() + v);
		}
	}
}

std::vector<std::shared_ptr<MachineState>> World::GetZombies()
{
	for (unsigned int i = 0; i < mZombies.size(); i++)
	{
		if (mZombies[i] == nullptr)
		{
			mZombies.erase(mZombies.begin() + i);
		}
	}
	return mZombies;
}

std::vector<std::shared_ptr<MachineState>> World::GetHumans()
{
	for (unsigned int i = 0; i < mHumans.size(); i++)
	{
		if (mHumans[i] == nullptr)
		{
			mHumans.erase(mHumans.begin() + i);
		}
	}
	return mHumans;
}

void World::ClearWorld()
{
	mZombies.clear();
	mHumans.clear();
	mHumFile = " ";
	mZomFile = " ";
	mMonth = 0;
}
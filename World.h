#pragma once

#define DECLARE_WORLD(WorldClass) friend class Singleton<SingletonClass>;
#include <vector>
#include <string>
#include "Machine.h"


class World
{
public:
	// Default constructor
	World() {  };

	// Add individuals
	void AddZombie(std::shared_ptr<MachineState> &zom);
	void AddHuman(std::shared_ptr<MachineState> &hum);
	void SetZomFile(std::string &zomFile);
	void SetHumFile(std::string &humFile);
	void SetMonth(int month) { mMonth = month; }

	// Getters / checkers
	std::string CheckLoc(int x, int y);
	std::shared_ptr<MachineState> AtLoc(int x, int y);
	std::vector<std::shared_ptr<MachineState>> GetZombies();
	std::vector<std::shared_ptr<MachineState>> GetHumans();
	std::string GetZomFile() { return mZomFile; }
	std::string GetHumFile() { return mHumFile; }
	int GetMonth() { return mMonth; }

	// Remove at [location]
	void RemoveAtLoc(int x, int y);

	//Remove all
	void ClearWorld();

private:
	std::vector<std::shared_ptr<MachineState>> mZombies;
	std::vector<std::shared_ptr<MachineState>> mHumans;
	std::string mZomFile = " ";
	std::string mHumFile = " ";
	int mMonth = 0;
};
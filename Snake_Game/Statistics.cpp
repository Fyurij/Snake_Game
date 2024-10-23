#include "Statistics.h"

#include <fstream>
#include <exception>



void Statistics::ReadFromFile()
{
	std::ifstream File("Config/Statistics.txt");
	if (!File.is_open())
	{
		throw std::runtime_error("Stats file is not opened...(Read)");
	}
	while (!File.eof())
	{
		std::string tempName;
		int tempScore;
		File >> tempName >> tempScore;
		if (!tempName.empty())
		{
			leaderboard[tempName] = tempScore;
		}
	}
}



bool Statistics::UpdatingScore()
{
	bool update = false;
	if (totalScore * koefficent > leaderboard[name])
	{
		leaderboard[name] = totalScore * koefficent;
		update = true;
	}
	return update;
}



void Statistics::WriteToFile()
{
	std::ofstream File("Config/Statistics.txt", std::ios::trunc);
	if (!File.is_open())
	{
		throw std::runtime_error("Stats file is not opened...(Write)");
	}
	for (std::map<std::string, int>::iterator i = leaderboard.begin(); i != leaderboard.end(); ++i)
	{
		File << i->first << " " << i->second << std::endl;
	}
}
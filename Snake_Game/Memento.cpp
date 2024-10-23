#include "Memento.h"

#include <fstream>
#include <exception>
#include <vector>
#include <string>
#include <filesystem>

void SnapshotManager::CheckSavesLimit()
{
	if (snapshots.size() >= savesLimit)
	{
		std::string temp = "Config/Saves/" + name + "/" + snapshots[0] + ".txt";
		std::filesystem::remove(temp);
		snapshots.pop_front();
	}
}

void SnapshotManager::WriteToSnapshot(Memento& snapshot)
{
	CheckSavesLimit();
	snapshot.WriteToFile();
	snapshots.push_back(snapshot.GetNameOfFile());
}

std::pair<Memento, bool> SnapshotManager::CreateMemento(int index)
{
	if (index - 1 >= snapshots.size())
	{
		throw std::runtime_error("Unknown snapshot...");
	}
	Memento snapshot(snapshots[index - 1], name);
	return std::make_pair(snapshot, snapshot.ReadFromFile());
}

void SnapshotManager::FindAllSnapshots()
{
	std::filesystem::path saves{ path };
	if (std::filesystem::exists(path))
	{
		for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator{ saves })
		{
			snapshots.push_back(dir_entry.path().stem().string());
		}
	}
}

void Memento::WriteToFile()
{
	std::filesystem::create_directory(path);
	std::fstream File(fileName, std::ios::out | std::ios::trunc);
	if (!File.is_open())
	{
		throw std::runtime_error("Snapshot file is not opened (write)");
	}
	File << name << std::endl << eatenFood << std::endl << totalScore << std::endl << static_cast<int>(difficulty) << std::endl << "snake" << std::endl;
	for (std::deque<Koordinates>::iterator i = snake.begin(); i != snake.end(); ++i)
	{
		File << i->x << " " << i->y << std::endl;
	}
	File << "food" << std::endl << food.x << " " << food.y << std::endl << "obstacle" << std::endl;
	for (std::vector<Koordinates>::iterator i = obstacles.begin(); i != obstacles.end(); ++i)
	{
		File << i->x << " " << i->y << std::endl;
	}
}

bool Memento::ReadFromFile()
{
	std::fstream File(fileName, std::ios::in);
	if (!File.is_open())
	{
		return false;
	}
	int level;
	std::string temp;
	File >> name >> eatenFood >> totalScore >> level >> temp;
	difficulty = static_cast<Level>(level);
	while (temp != "food")
	{
		std::getline(File, temp);
		Koordinates koordinatesSnake;
		int pos = 0;
		int finish = temp.find(" ");
		if (finish != std::string::npos)
		{
			koordinatesSnake.x = std::stoi(temp.substr(pos, finish));
			pos = finish + 1;
			koordinatesSnake.y = std::stoi(temp.substr(pos));
			snake.push_back(koordinatesSnake);
		}
	}
	File >> food.x >> food.y >> temp;
	while (!File.eof())
	{
		Koordinates koordinatesObstacles;
		File >> koordinatesObstacles.x >> koordinatesObstacles.y;
		if (File.eof())
		{
			break;
		}
		obstacles.push_back(koordinatesObstacles);
	}
	return true;
}

std::string Memento::GetNameOfFile()
{
	std::filesystem::path saves{ fileName };
	return saves.stem().string();
}
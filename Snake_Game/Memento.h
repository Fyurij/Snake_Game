#pragma once
#include "Structs.h"

#include <string>
#include <vector>
#include <deque>
#include <time.h>
#include <chrono>
#include <filesystem>
#include <utility>



class Memento
{
	friend class Field;
private:
	std::string name;
	std::filesystem::path path;
	int eatenFood;
	int totalScore;
	Level difficulty;
	std::deque<Koordinates> snake;
	std::vector<Koordinates> obstacles;
	Koordinates food;
	std::string fileName;

private:
	void SetFileName()
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), "%d-%m-%Y_%H-%M-%S");
		fileName = path.string() + ss.str() + ".txt";
	}

public:
	Memento(const std::string& name_)
		:name(name_)
		, path("Config/Saves/" + name + "/")
	{
		SetFileName();
	}

	Memento(const std::string& fileName_, const std::string& name_)
		:path("Config/Saves/" + name_ + "/")
		, fileName("Config/Saves/" + name_ + "/" + fileName_ + ".txt")
		, name(name_)
	{}

	Memento(const std::string& name_,
	int eatenFood_,
	int totalScore_,
	Level difficulty_,
	const std::deque<Koordinates>& snake_,
	const std::vector<Koordinates>& obstacles_,
	const Koordinates& food_)
		:name(name_)
		, eatenFood(eatenFood_)
		, totalScore(totalScore_)
		, difficulty(difficulty_)
		, snake(snake_)
		, obstacles(obstacles_)
		, food(food_)
		, path("Config/Saves/" + name + "/")
	{
		SetFileName();
	}

	void WriteToFile();
	bool ReadFromFile();
	std::string GetNameOfFile();

	Level GetDifficulty()
	{
		return difficulty;
	}
};



class SnapshotManager
{
private:
	int savesLimit;
	std::deque<std::string> snapshots;
	std::string name;
	std::filesystem::path path;
public:
	SnapshotManager(const std::string& name_, int limit)
		:name(name_)
		, path("Config/Saves/" + name + "/")
		, savesLimit(limit)
	{}

	void WriteToSnapshot(Memento& snapshot);
	std::pair<Memento, bool> CreateMemento(int index);
	void CheckSavesLimit();
	void FindAllSnapshots();

	bool IsEmpty()
	{
		return snapshots.empty();
	}

	std::deque<std::string> GetSnapshotsList()
	{
		return snapshots;
	}
};
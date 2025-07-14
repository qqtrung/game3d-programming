#pragma once
#include <string>
#include <fstream>

class Map 
{
public:

	const static int max_cel_x = 100; 
	const static int max_cel_y = 100; 
	const static int max_cel_z = 100; 	
	std::vector<std::vector<std::vector<int>>> map;

public:

	Map() = default; 

	Map(const std::string& FILE) 
	{
		map.assign(max_cel_x, std::vector<std::vector<int>>(max_cel_y, std::vector<int>(max_cel_z, 0)));
		int x, y, z; 
		std::ifstream file; 
		file.open(FILE);
		while (file >> x >> y >> z) {
			map[x][y][z] = 1; 
		}
		file.close();
	}

	int getCell(int x, int y, int z) {
		return map[x][y][z];
	}

}; 
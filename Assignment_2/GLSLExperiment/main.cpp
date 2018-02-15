#include "PlyFile.h"
#include <string>

int main(int argc, char* argv[])
{
	std::string airplane("ply_files/airplane.ply");
	assignment2::Ply * plyModel = assignment2::Ply::Load(airplane);
	int i = 0;
	return 0;
}
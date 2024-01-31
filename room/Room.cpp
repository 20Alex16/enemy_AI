#include <string.h>
#include <iomanip>
#include <algorithm>
#include <Room.h>

void Room::printAdjency(std::ostream& out, bool printCells)
{
	out << "Room " << roomNumber << " (" << centerX << ", " << centerY << "): ";
	if (adjency == nullptr)
	{
		out << "(none)\n";
		return;
	}
	out << '\n';
	adjency->printInorderRooms(out, printCells);
	out << '\n';
}

void Room::addAdjacentRoom(Room* room, typePath& path)
{
	if (this->adjency == nullptr)
	{
		this->adjency = new RoomNodeBST(room, path);
		return;
	}

	RoomNodeBST* currentNode = this->adjency;
	while (true)
	{
		currentNode->totalDescendants++;
		if (currentNode->room->roomNumber > room->roomNumber)
		{
			currentNode->leftNodeContainingDescendants++;
			if (currentNode->left != nullptr)
				currentNode = currentNode->left;
			else
			{
				currentNode = currentNode->left = new RoomNodeBST(room, path);
				break;
			}
		}
		else
		{
			if (currentNode->right != nullptr)
				currentNode = currentNode->right;
			else
			{
				currentNode = currentNode->right = new RoomNodeBST(room, path);
				break;
			}
		}
	}
}

bool Room::isAdjacentTo(Room* room)
{
	if (room == nullptr)
		return false;

	RoomNodeBST* currentRoom = this->adjency;
	if (currentRoom == nullptr)
		return false;

	while (true)
	{
		if (currentRoom->room->roomNumber == room->roomNumber)
			return true;

		if (currentRoom->room->roomNumber > room->roomNumber)
		{
			if (currentRoom->left != nullptr)
				currentRoom = currentRoom->left;
			else
				return false;
		}
		else
		{
			if (currentRoom->right != nullptr)
				currentRoom = currentRoom->right;
			else
				return false;
		}
	}
}

void MatrixRooms::connectPath(typeCell start, typeCell end, typePath& path)
{
	unsigned room1Number = mx[start.second][start.first];
	unsigned room2Number = mx[end.second][end.first];

	if (room1Number == room2Number)
		return;

	if (room1Number > room2Number)
		return connectPath(end, start, path);

	path.push_back(start);
	while (start != end)
	{
		if (start.first < end.first && isValidCell(start.first + 1, start.second, room1Number, room2Number))
		{
			start.first++;
			path.push_back(start);
		}
		else if (start.first > end.first && isValidCell(start.first - 1, start.second, room1Number, room2Number))
		{
			start.first--;
			path.push_back(start);
		}
		else if (start.second < end.second && isValidCell(start.first, start.second + 1, room1Number, room2Number))
		{
			start.second++;
			path.push_back(start);
		}
		else if (start.second > end.second && isValidCell(start.first, start.second - 1, room1Number, room2Number))
		{
			start.second--;
			path.push_back(start);
		}
	}
}

void MatrixRooms::formBSTs()
{
	for (Room& r : rooms)
	{
		// std::cout << "Room " << r.roomNumber << '\n';
		// scan topline
		if (r.y > 0)
			for (unsigned i = r.x; i < r.x + r.w; i++)
			{
				const unsigned numberAbove = mx[r.y - 1][i];
				if (numberAbove == 1)
					continue;
				Room& roomAbove = rooms[numberAbove - 2];
				if (!r.isAdjacentTo(&roomAbove))
				{
					// std::cout << "Number above: " << numberAbove << '\n';
					typeCell center1 = r.getCenter();
					typeCell center2 = roomAbove.getCenter();
					typePath path = typePath();
					connectPath(center1, center2, path);
					r.addAdjacentRoom(&roomAbove, path);
					std::reverse(path.begin(), path.end());
					roomAbove.addAdjacentRoom(&r, path);
				}
			}
		if (r.x > 0)
		{
			for (unsigned i = r.y; i < r.y + r.h; i++)
			{
				const unsigned numberLeft = mx[i][r.x - 1];
				if (numberLeft != 1)
				{
					Room& roomLeft = rooms[numberLeft - 2];
					if (!r.isAdjacentTo(&roomLeft))
					{
						// std::cout << "Number left: " << numberLeft << '\n';
						typeCell center1 = r.getCenter();
						typeCell center2 = roomLeft.getCenter();
						typePath path = typePath();
						connectPath(center1, center2, path);
						r.addAdjacentRoom(&roomLeft, path);
						std::reverse(path.begin(), path.end());
						roomLeft.addAdjacentRoom(&r, path);
					}
				}
			}
		}
		if (r.x + r.w < mx.nrCols())
		{
			for (unsigned i = r.y; i < r.y + r.h; i++)
			{
				const unsigned numberRight = mx[i][r.x + r.w];
				if (numberRight != 1)
				{
					Room& roomRight = rooms[numberRight - 2];
					if (!r.isAdjacentTo(&roomRight))
					{
						// std::cout << "Number right: " << numberRight << '\n';
						typeCell center1 = r.getCenter();
						typeCell center2 = roomRight.getCenter();
						typePath path = typePath();
						connectPath(center1, center2, path);
						r.addAdjacentRoom(&roomRight, path);
						std::reverse(path.begin(), path.end());
						roomRight.addAdjacentRoom(&r, path);
					}
				}
			}
		}
		if (r.y + r.h < mx.nrLines())
		{
			for (unsigned i = r.x; i < r.x + r.w; i++)
			{
				const unsigned numberBelow = mx[r.y + r.h][i];
				if (numberBelow == 1)
					continue;
				Room& roomBelow = rooms[numberBelow - 2];
				if (!r.isAdjacentTo(&roomBelow))
				{
					// std::cout << "Number down: " << numberBelow << '\n';
					typeCell center1 = r.getCenter();
					typeCell center2 = roomBelow.getCenter();
					typePath path = typePath();
					connectPath(center1, center2, path);
					r.addAdjacentRoom(&roomBelow, path);
					std::reverse(path.begin(), path.end());
					roomBelow.addAdjacentRoom(&r, path);
				}
			}
		}
		// std::cout << '\n';
	}
}

MatrixRooms::MatrixRooms(unsigned size) : mx(size)
{
	rooms = std::vector<Room>();
	{ // important pentru a face roomTuples sa dispara
		std::vector<RoomTuple> roomTuples = std::vector<RoomTuple>();

		// mx.generateWalls(40, 5.f);
		mx.generateWalls(40, 7.f);
		mx.generateRooms(roomTuples);

		for (auto& roomTuple : roomTuples)
		{
			rooms.push_back(Room(roomTuple, mx[roomTuple.y][roomTuple.x]));
		}
	}

	formBSTs();
}

void MatrixRooms::printRooms(std::ostream& out)
{
	for (unsigned i = 0; i < rooms.size(); i++)
	{
		out << "Room " << rooms[i].roomNumber << " ("
			<< rooms[i].centerX << ", " << rooms[i].centerY << "): ";
		rooms[i].print(out);
	}
	out << '\n';
}

void MatrixRooms::displayRooms(SDL_Renderer* renderer) {
	mx.display(renderer);
}

void MatrixRooms::displayRooms(std::ostream& out)
{
	mx.display(out);
}

void MatrixRooms::displayPaths(SDL_Renderer* renderer)
{
	// create a dummy vector of the size of the matrix and print each line with colour
	unsigned* printMatrix = new unsigned[mx.nrLines() * mx.nrCols()];
	memset(printMatrix, 0, mx.nrLines() * mx.nrCols() * sizeof(unsigned));

	// traverse all rooms and print their paths
	for (Room& r : rooms)
	{
		RoomNodeBST* currentNode = r.adjency;
		while (currentNode != nullptr)
		{
			for (typeCell& c : currentNode->path)
			{
				printMatrix[c.second * mx.nrCols() + c.first] = 2;
			}

			printMatrix[currentNode->path[0].second * mx.nrCols() + currentNode->path[0].first] =
				printMatrix[currentNode->path.back().second * mx.nrCols() + currentNode->path.back().first] = 1;
			currentNode = currentNode->left;
		}
	}

	unsigned biggestRoomNumber = rooms.size() + 1;
	unsigned roomNumberDigits = 0;
	while (biggestRoomNumber > 0)
	{
		biggestRoomNumber /= 10;
		roomNumberDigits++;
	}

	char wall[10];
	memset(wall, '#', 10);
	wall[roomNumberDigits] = '\0';

	SDL_Rect rect;
	rect.h = rect.w = CELL_WIDTH;

	for (unsigned i = 0; i < mx.nrLines(); i++)
	{
		for (unsigned j = 0; j < mx.nrCols(); j++)
		{
			if (printMatrix[i * mx.nrCols() + j] == 0)
			{
				if (mx[i][j] == 1)
				{
					rect.x = j * CELL_WIDTH;
					rect.y = i * CELL_WIDTH;
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderFillRect(renderer, &rect);
				}
				else {
					rect.x = j * CELL_WIDTH;
					rect.y = i * CELL_WIDTH;
					SDL_SetRenderDrawColor(renderer, 100, 150, 200, 255);
					SDL_RenderFillRect(renderer, &rect);
				}
			}
			else if (printMatrix[i * mx.nrCols() + j] == 1)
			{
				rect.x = j * CELL_WIDTH;
				rect.y = i * CELL_WIDTH;
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
			else if (printMatrix[i * mx.nrCols() + j] == 2)
			{
				rect.x = j * CELL_WIDTH;
				rect.y = i * CELL_WIDTH;
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}

	delete[] printMatrix;
}

void MatrixRooms::displayPaths(std::ostream& out)
{
	// create a dummy vector of the size of the matrix and print each line with colour
	unsigned* printMatrix = new unsigned[mx.nrLines() * mx.nrCols()];
	memset(printMatrix, 0, mx.nrLines() * mx.nrCols() * sizeof(unsigned));

	// traverse all rooms and print their paths
	for (Room& r : rooms)
	{
		RoomNodeBST* currentNode = r.adjency;
		while (currentNode != nullptr)
		{
			for (typeCell& c : currentNode->path)
			{
				printMatrix[c.second * mx.nrCols() + c.first] = 2;
			}

			printMatrix[currentNode->path[0].second * mx.nrCols() + currentNode->path[0].first] =
				printMatrix[currentNode->path.back().second * mx.nrCols() + currentNode->path.back().first] = 1;
			currentNode = currentNode->left;
		}
	}

	unsigned biggestRoomNumber = rooms.size() + 1;
	unsigned roomNumberDigits = 0;
	while (biggestRoomNumber > 0)
	{
		biggestRoomNumber /= 10;
		roomNumberDigits++;
	}

	char wall[10];
	memset(wall, '#', 10);
	wall[roomNumberDigits] = '\0';

	for (unsigned i = 0; i < mx.nrLines(); i++)
	{
		for (unsigned j = 0; j < mx.nrCols(); j++)
		{
			if (printMatrix[i * mx.nrCols() + j] == 0)
			{
				if (mx[i][j] == 1)
					out << "\033[0;37m"; // wall
				else
					out << "\033[0;32m"; // room
			}
			else if (printMatrix[i * mx.nrCols() + j] == 1)
				out << "\033[0;34m"; // room center
			else if (printMatrix[i * mx.nrCols() + j] == 2)
				out << "\033[0;31m"; // path

			if (mx[i][j] == 1)
				out << wall;
			else
				out << std::setw(roomNumberDigits) << mx[i][j];
			out << " ";
		}
		out << "\033[0m\n";
	}

	delete[] printMatrix;
}

void MatrixRooms::printRoomsAdjency(std::ostream& out, bool printPath)
{
	for (Room& r : rooms)
	{
		r.printAdjency(out, printPath);
	}
}

void MatrixRooms::displayRoomsWithNumbers(std::ostream& out)
{
	mx.displayWithNumbers(out);
}

/*
void MatrixRooms::displayRoomsWithNumbers(SDL_Renderer * renderer)
{
	mx.displayWithNumbers(renderer);
}
*/

RoomNodeBST::~RoomNodeBST()
{
	if (left != nullptr)
		delete left;
	if (right != nullptr)
		delete right;
}

unsigned RoomNodeBST::getKthAdjacentRoomNumber(unsigned roomLeft)
{
	//std::cout<< "RoomNodeBST::getKthAdjacentRoomNumber(" << roomLeft << ")\n";

	//if (totalDescendants == 0)
		//return room->getRoomNumber();

	if (roomLeft == leftNodeContainingDescendants)
		return room->getRoomNumber();

	if (roomLeft < leftNodeContainingDescendants)
		return left->getKthAdjacentRoomNumber(roomLeft);
	else
		return right->getKthAdjacentRoomNumber(roomLeft - leftNodeContainingDescendants - 1);
}

void RoomNodeBST::printInorderRooms(std::ostream& out, bool printCells)
{
	if (left != nullptr)
		left->printInorderRooms(out, printCells);

	if (printCells)
	{
		out << room->getRoomNumber() << ": ";
		for (typeCell& c : path)
		{
			out << "(" << c.first << ", " << c.second << ") ";
		}
		out << '\n';
	}
	else
		out << room->getRoomNumber() << " ";

	if (right != nullptr)
		right->printInorderRooms(out, printCells);
}

typePath MatrixRooms::getPath(unsigned room1, unsigned room2)
{
	if (room1 == room2)
		return typePath();

	Room& r1 = rooms[room1 - 2];
	Room& r2 = rooms[room2 - 2];
	RoomNodeBST* currentNode = r1.adjency;
	while (currentNode != nullptr)
	{
		if (currentNode->room->roomNumber == room2)
		{
			return currentNode->path;
		}
		else if (currentNode->room->roomNumber > room2)
			currentNode = currentNode->left;
		else
			currentNode = currentNode->right;
	}
	return typePath();
}

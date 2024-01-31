#include <Matrix.h>
#include <string.h>
#include <stdlib.h>
#include <iomanip>
#include "windows.h"

// using std::cout;

float getScore(unsigned x, unsigned y)
{
	unsigned min = x < y ? x : y;
	unsigned max = x > y ? x : y;
	return min * min * min + max;
}

RoomTuple Matrix::getBestRoom()
{
	float bestScore = 0;
	RoomTuple room = { 0, 0, 0, 0 };
	for (unsigned line = 0; line < nrLines(); line++)
	{
		for (unsigned col = 0; col < nrCols(); col++)
		{
			if (table[line][col] == 0)
			{
				ScoredRoomTuple scoredRoom = getScoredRoom(line, col);
				if (scoredRoom.score > bestScore)
				{
					bestScore = scoredRoom.score;
					room = scoredRoom.room;
				}
			}
		}
	}
	return room;
}

ScoredRoomTuple Matrix::getScoredRoom(unsigned line, unsigned col)
{
	float bestScore = 0;
	RoomTuple room = { 0, 0, 0, 0 };
	unsigned maxLine = nrLines();
	for (unsigned crtCol = col; crtCol < nrCols(); crtCol++)
	{
		if (table[line][crtCol] != 0)
		{
			break;
		}
		else
		{
			unsigned crtLine = line;
			while (crtLine < maxLine && table[crtLine][crtCol] == 0)
			{
				crtLine++;
			}
			maxLine = crtLine;
			unsigned width = crtCol - col + 1;
			unsigned height = crtLine - line;
			float score = getScore(height, width);
			if (bestScore == 0 || score > bestScore)
			{
				bestScore = score;
				room = { col, line, width, height };
			}
		}
	}
	return { room, bestScore };
}

Matrix::Matrix(unsigned width, unsigned height)
{
	roomsGenerated = -1;
	w = width;
	h = height;
	table = new int* [h];
	for (unsigned i = 0; i < h; i++)
	{
		table[i] = new int[w];
		memset(table[i], 0, w * sizeof(int));
	}
	// generatedRooms = std::vector<Room>();
}

Matrix::Matrix(unsigned l) : Matrix(l, l) {}

Matrix::~Matrix()
{
	for (unsigned i = 0; i < h; i++)
	{
		delete[] table[i];
	}
	delete[] table;
}

int* Matrix::operator[](unsigned i)
{
	return table[i];
}

void Matrix::generateWalls(unsigned thresholdPercent, float perlinScale, unsigned perlinSeed)
{
	pn = PerlinNoise(perlinSeed);
	for (unsigned i = 0; i < h; i++)
	{
		for (unsigned j = 0; j < w; j++)
		{
			// use perlin noise insted
			// table[i][j] = rand() % 100 < thresholdPercent ? 1 : 0;
			table[i][j] = pn.noise(i / (float)perlinScale, j / (float)perlinScale, 0.8) * 100 < thresholdPercent ? 1 : 0;
		}
	}
}

void Matrix::generateRooms(std::vector<RoomTuple>& generatedRooms)
{
	roomsGenerated = 2;
	while (true)
	{
		RoomTuple room = getBestRoom();
		if (room.w == 0 || room.h == 0)
		{
			break;
		}
		for (unsigned line = room.y; line < room.y + room.h; line++)
		{
			for (unsigned col = room.x; col < room.x + room.w; col++)
			{
				table[line][col] = roomsGenerated;
			}
		}
		generatedRooms.push_back(room);
		roomsGenerated++;
	}
}

void setConsoleColor(int foregroundColor, int backgroundColor = -1)
{
	if (backgroundColor != -1)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), foregroundColor + backgroundColor * 16);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), foregroundColor % 15);
}

unsigned numberWidth(int number)
{
	unsigned width = 0;
	while (number)
	{
		number /= 10;
		width++;
	}
	return width;
}

#define RESET "\033[0m"

void Matrix::displayWithNumbers(std::ostream& out)
{
	out << RESET << '\n';
	unsigned width = numberWidth(roomsGenerated);
	char wall[5];
	memset(wall, '#', 5);
	wall[width] = '\0';

	for (unsigned i = 0; i < h; i++)
	{
		for (unsigned j = 0; j < w; j++)
		{
			if (table[i][j] == 1)
				out << wall;
			else
			{
				setConsoleColor(table[i][j] + 1);
				out << std::setw(width) << table[i][j] << RESET;
			}
			out << ' ';
		}
		out << '\n';
	}
	out << std::endl;
}

void SDL_SetRendererRandomDrawColor(SDL_Renderer* renderer, int color) {
	uint8_t r, g, b;
	r = (color * 103) % 255;
	g = (color * 113) % 255;
	b = (color * 139) % 255;
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

/*
void Matrix::displayWithNumbers(SDL_Renderer* renderer)
{
	SDL_Rect rect;
	rect.w = rect.h = 10;

	unsigned width = numberWidth(roomsGenerated);

	for (unsigned i = 0; i < h; i++)
	{
		for (unsigned j = 0; j < w; j++)
		{
			if (table[i][j] == 1)
			{
				rect.x = j * 10;
				rect.y = i * 10;
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
			else
			{
				rect.x = j * 10;
				rect.y = i * 10;
				SDL_SetRendererRandomDrawColor(renderer, table[i][j]);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}
*/

void Matrix::display(std::ostream& out)
{

	out << RESET << '\n';
	for (unsigned i = 0; i < h; i++)
	{
		for (unsigned j = 0; j < w; j++)
		{
			if (table[i][j] == 1)
				setConsoleColor(0);
			else
				setConsoleColor(0, table[i][j] + 1);
			out << "  "; // << RESET;
			setConsoleColor(0);
		}
		out << '\n';
	}
	out << RESET << std::endl;
}

void Matrix::display(SDL_Renderer* renderer)
{
	SDL_Rect rect;
	rect.w = rect.h = CELL_WIDTH;
	for (unsigned i = 0; i < h; i++)
	{
		for (unsigned j = 0; j < w; j++)
		{
			if (table[i][j] == 1)
			{
				rect.x = j * CELL_WIDTH;
				rect.y = i * CELL_WIDTH;
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
			else
			{
				rect.x = j * CELL_WIDTH;
				rect.y = i * CELL_WIDTH;
				// put text here

				SDL_SetRendererRandomDrawColor(renderer, table[i][j]);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}

/*
void Matrix::printRooms(std::ostream &out){
	for (unsigned i = 0; i < generatedRooms.size(); i++)
	{
		out << "Room " << i << ": ";
		generatedRooms[i].print(out);
	}
	out << RESET << std::endl;
}
*/
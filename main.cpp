#include <iostream>
#include <Matrix.h>
#include <string>
#include <Room.h>
#include <tuple>

#include <SDL.h>

#pragma region Matrix
using namespace std;

void matrixDemo()
{
	Matrix m = Matrix(20);
	// m.generateWalls(40, 10.f);
	m.generateWalls(40, 5.f);

	system("cls");
	m.displayWithNumbers(cout);
	cout << "\n\n";
	m.display(cout);
}

void roomDemo(unsigned length = 20)
{
	MatrixRooms rooms(length);
	rooms.displayRooms(cout);
	rooms.displayRoomsWithNumbers(cout);
	// rooms.printRooms(cout);
	// rooms.printRoomsAdjency(cout, true);
	rooms.displayPaths(cout);

	// print path from room 2 to 4
	/*
	for(auto cell : rooms.getPath(4,2)){
		cout << cell.first << ", " << cell.second << endl;
	}
	*/

	// get kth room from room 0

	cin.get();
}
#pragma endregion

#define ENEMY_MOVE_INTERVAL 20

void drawSquare(SDL_Renderer* renderer, float x, float y, float width, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Rect rect;
	rect.x = x * CELL_WIDTH;
	rect.y = y * CELL_WIDTH;
	rect.w = width;
	rect.h = width;
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char* argv[])
{
	/*
	int length = 20;
	if (argc == 2)
	{
		length = stoi(argv[1]);
		cout << "Length: " << length << endl;
	}
	// matrixDemo();
	roomDemo(length);
	*/

	//roomDemo(20);

	MatrixRooms mr(45);
	mr.displayRoomsWithNumbers(cout);
	//mr.displayPaths(cout);
	//mr.printRoomsAdjency(cout);

	unsigned heroX, heroY;
	unsigned enemyX, enemyY;

	unsigned enemyLastRoom = mr.randomRoomNumber();
	unsigned enemyCurrentRoom = enemyLastRoom;
	unsigned enemyMovingToRoom = mr.getRandomAdjacentRoom(enemyCurrentRoom);

	std::tie(heroX, heroY) = mr.roomCenter(mr.randomRoomNumber());
	std::tie(enemyX, enemyY) = mr.roomCenter(enemyCurrentRoom);

	typePath enemyPath = mr.getPath(enemyCurrentRoom, enemyMovingToRoom);
	unsigned pathIndex = 0;


	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "Nu se poate initializa" << endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Enemy AI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Uint32 lastMovedEnemy = 0;

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		mr.displayRooms(renderer);
		//mr.displayPaths(renderer);
		//mr.displayRoomsWithNumbers(renderer);

		if (SDL_GetTicks() - lastMovedEnemy > ENEMY_MOVE_INTERVAL) {
			pathIndex++;
			if (pathIndex == enemyPath.size()) {
				pathIndex = 0;
				enemyLastRoom = enemyCurrentRoom;
				enemyCurrentRoom = enemyMovingToRoom;
				do {
					enemyMovingToRoom = mr.getRandomAdjacentRoom(enemyCurrentRoom);
				} while (enemyLastRoom == enemyMovingToRoom && mr.getNumberOfAdjacentRooms(enemyCurrentRoom) != 1);

				enemyPath = mr.getPath(enemyCurrentRoom, enemyMovingToRoom);
			}

			enemyX = std::get<0>(enemyPath[pathIndex]);
			enemyY = std::get<1>(enemyPath[pathIndex]);


			lastMovedEnemy = SDL_GetTicks();
			SDL_Delay(2);
		}

		drawSquare(renderer, heroX, heroY, CELL_WIDTH, 0, 125, 255);
		drawSquare(renderer, enemyX, enemyY, CELL_WIDTH, 255, 0, 0);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

#pragma once
#include <Matrix.h>
#include <vector>

typedef std::pair<unsigned, unsigned> typeCell;
typedef std::vector<typeCell> typePath;

struct RoomNodeBST;

class Room
{
private:
    void addAdjacentRoom(Room *room, typePath &path);
    bool isAdjacentTo(Room * room);
    RoomNodeBST *adjency;

    unsigned roomNumber;

public:
    unsigned x, y, w, h;
    unsigned centerX, centerY;
    typeCell getCenter()
    {
        return std::make_pair(centerX, centerY);
    }
    void setCenter()
    {
        centerX = x + w / 2;
        centerY = y + h / 2;
    }

    Room(unsigned col, unsigned row, unsigned width, unsigned height, unsigned number) : x(col), y(row), w(width), h(height)
    {
        roomNumber = number;
        adjency = nullptr;
        setCenter();
    }

    Room(RoomTuple r, unsigned number) : Room(r.x, r.y, r.w, r.h, number) {}

    unsigned getRoomNumber()
    {
        return roomNumber;
    }

    void printAdjency(std::ostream &out, bool printCells = false);
    void print(std::ostream &out)
    {
        out << x << ", " << y << ", " << w << ", " << h
            << ", " << centerX << ", " << centerY << '\n';
    }

    friend class MatrixRooms;
};

struct RoomNodeBST
{ // utilizat pt room
    Room *room;
    std::vector<typeCell> path;
    RoomNodeBST *left, *right;
    RoomNodeBST(Room *other, typePath &cellPath)
        : room(other), left(nullptr), right(nullptr), path(cellPath){};
    void printInorderRooms(std::ostream &out, bool printCells = false);
};

class MatrixRooms // utilizat ca structura globala ce stocheaza toate camerle
{
private:
    Matrix mx;
    std::vector<Room> rooms;
    void formBSTs();
    void connectPath(typeCell start, typeCell end, typePath &path);
    bool isValidCell(unsigned x, unsigned y, unsigned rN1, unsigned rN2){
        return mx[y][x] == rN1 || mx[y][x] == rN2;
    }

public:
    MatrixRooms(unsigned size = 20);

    // path between center of two rooms
    typePath getPath(unsigned room1, unsigned room2);

    // path between 2 cells given they somehow not obstructed
    typePath getPath(typeCell cell1, typeCell cell2);

    void printRoomsAdjency(std::ostream &out, bool printPath = false);
    void printRooms(std::ostream &out);
    void displayRooms(std::ostream &out);
    void displayPaths(std::ostream &out);
    void displayRoomsWithNumbers(std::ostream &out);
};
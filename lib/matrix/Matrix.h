#pragma once
#include <iostream>
#include <vector>
#include <PerlinNoise.h>

typedef struct RoomTuple
{
    unsigned x, y, w, h;
} RoomTuple;

typedef struct ScoredRoomTuple
{
    RoomTuple room;
    float score;
} ScoredRoomTuple;

class Matrix
{
private:
    unsigned w, h;
    int **table;
    int roomsGenerated;
    PerlinNoise pn;

    RoomTuple getBestRoom();
    ScoredRoomTuple getScoredRoom(unsigned line, unsigned col);

public:
    Matrix(unsigned width, unsigned height);
    Matrix(unsigned l);
    ~Matrix();

    unsigned nrLines()
    {
        return h;
    }

    unsigned nrCols()
    {
        return w;
    }

    int *operator[](unsigned i);
    void displayWithNumbers(std::ostream &out);
    void display(std::ostream &out);

    // generation
    void generateWalls(unsigned thresholdPercent = 30, float perlinScale = 1.f, unsigned perilinSeed = 30);
    void generateRooms(std::vector<RoomTuple> & generatedRooms); // this func will also create BST with DLL
};
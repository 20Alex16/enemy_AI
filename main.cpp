#include <iostream>
#include <Matrix.h>
#include <Room.h>

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
    //*
    for (auto cell : rooms.getPath(4, 2))
    {
        cout << cell.first << ", " << cell.second << endl;
    }
    //*/

    // get path between a margin and a neighbor room
    //*
    for (auto cell : rooms.getPath({7, 7}, {10, 10}))
    {
        cout << cell.first << ", " << cell.second << endl;
    }
    //*/
}

// you can run this with ./main 20
int main(int argc, char *argv[])
{
    int length = 20;
    if (argc == 2)
    {
        length = stoi(argv[1]);
        // cout << "Length: " << length << endl;
    }

    // matrixDemo();
    roomDemo(length);
}

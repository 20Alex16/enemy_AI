import random
import sys

# MAT = [
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 1, 1, 1, 1, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 1, 1, 1, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 1, 0, 0, 1, 1, 1, 1],
#     [0, 0, 0, 1, 0, 0, 0, 0, 1, 0],
#     [0, 0, 0, 1, 0, 0, 0, 0, 1, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
# ]

def generate(n):
    res = []
    for _ in range(n):
        row = [1 if random.randint(0, 5) == 0 else 0 for _ in range(n)]
        res.append(row)
    return res

MAT = generate(int(sys.argv[1]))

def getScore(width, height):
    x = min(width, height)
    y = max(width, height)
    return x**3 + y

def getBestRoom(mat, nrLines, nrCols, line, col):
    bestScore = None
    room = None
    maxLine = nrLines
    for crtCol in range(col, nrCols):
        if mat[line][crtCol] != 0:
            break
        else:
            crtLine = line
            while crtLine < maxLine and mat[crtLine][crtCol] == 0:
                crtLine += 1
            maxLine = crtLine
            score = getScore(crtLine - line, crtCol - col + 1)
            if bestScore is None or score > bestScore:
                bestScore = score
                room = (line, col, crtLine - 1, crtCol)
    return room, bestScore

def getBestRoom1(mat, nrLines, nrCols):
    bestScore = None
    bestRoom = None
    for line in range(nrLines):
        for col in range(nrCols):
            if mat[line][col] == 0:
                room, score = getBestRoom(mat, nrLines, nrCols, line, col)
                if score is not None and (bestScore is None or score > bestScore):
                    bestScore = score
                    bestRoom = room
    return bestRoom

def main():
    crtRoom = 2
    nrLines = len(MAT)
    nrCols = len(MAT[0])
    while True:
        room = getBestRoom1(MAT, nrLines, nrCols)
        if room is None:
            break
        for line in range(room[0], room[2] + 1):
            for col in range(room[1], room[3] + 1):
                MAT[line][col] = crtRoom
        crtRoom += 1

    for row in MAT:
        print(" ".join(f"{x:3d}" if x != 1 else "###" for x in row))


if __name__ == "__main__":
    main()
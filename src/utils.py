BOARD_SIZE = 10

def in_bounds(x, y):
    return 0 <= x < BOARD_SIZE and 0 <= y < BOARD_SIZE


def cells_touch(c1, c2):
    return abs(c1[0] - c2[0]) <= 1 and abs(c1[1] - c2[1]) <= 1


def ship_touches_existing(ship_cells, ships):
    for s in ships:
        for c1 in s:
            for c2 in ship_cells:
                if cells_touch(c1, c2):
                    return True
    return False
def print_board(board, title=""):
    if title:
        print("\n" + title)

    print("  " + " ".join(str(i) for i in range(10)))
    for i, row in enumerate(board):
        print(f"{i} " + " ".join(row))


def load_ships(path):
    ships = []
    with open(path) as f:
        for line in f:
            cells = []
            for pair in line.strip().split(";"):
                x, y = map(int, pair.split(","))
                cells.append((x, y))
            ships.append(cells)
    return ships

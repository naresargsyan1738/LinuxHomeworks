import csv
import random

BOARD_SIZE = 10

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

def create_board():
    return [["." for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)]

def print_board(board, title):
    print("\n" + title)
    print("  0 1 2 3 4 5 6 7 8 9")
    for i, row in enumerate(board):
        print(i, " ".join(row))

def player_shot(bot_ships, board):
    x, y = map(int, input("Ваш выстрел (x y): ").split())

    for ship in bot_ships:
        if (x, y) in ship:
            board[x][y] = "X"
            ship.remove((x, y))
            print("Попадание!")

            if len(ship) == 0:
                print("Корабль уничтожен!")
                bot_ships.remove(ship)

            return (x, y), "hit"

    board[x][y] = "O"
    print("Промах")
    return (x, y), "miss"

def bot_shot(player_ships, board):
    while True:
        x = random.randint(0, 9)
        y = random.randint(0, 9)
        if board[x][y] == ".":
            break

    for ship in player_ships:
        if (x, y) in ship:
            board[x][y] = "X"
            ship.remove((x, y))
            print(f"Бот стреляет {x} {y} — ПОПАДАНИЕ")

            if len(ship) == 0:
                print("Бот уничтожил ваш корабль!")
                player_ships.remove(ship)

            return (x, y), "hit"

    board[x][y] = "O"
    print(f"Бот стреляет {x} {y} — промах")
    return (x, y), "miss"


def start_game():
    player_ships = load_ships("data/player_ships.csv")
    bot_ships = load_ships("data/bot_ships.csv")

    player_board = create_board()
    bot_board = create_board()

    with open("data/game_state.csv", "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["turn", "player_move", "bot_move"])

    turn = 1
    while player_ships and bot_ships:
        print(f"\n===== ХОД {turn} =====")

        p_move, _ = player_shot(bot_ships, bot_board)
        print_board(bot_board, "Поле бота")

        if not bot_ships:
            print("ВЫ ПОБЕДИЛИ!")
            break

        b_move, _ = bot_shot(player_ships, player_board)
        print_board(player_board, "Ваше поле")

        with open("data/game_state.csv", "a", newline="") as f:
            writer = csv.writer(f)
            writer.writerow([turn, p_move, b_move])

        if not player_ships:
            print("БОТ ПОБЕДИЛ")
            break

        turn += 1 

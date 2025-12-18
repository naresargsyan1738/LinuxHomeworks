import random
from src.utils import in_bounds, ship_touches_existing

SHIP_SIZES = [4, 3, 3, 2, 2, 2, 1, 1, 1, 1]


def generate_bot_ships():
    ships = []

    for size in SHIP_SIZES:
        while True:
            x = random.randint(0, 9)
            y = random.randint(0, 9)
            orient = random.choice(["H", "V"])

            cells = []
            for i in range(size):
                cx = x + i if orient == "V" else x
                cy = y + i if orient == "H" else y
                cells.append((cx, cy))

            if not all(in_bounds(cx, cy) for cx, cy in cells):
                continue

            if ship_touches_existing(cells, ships):
                continue

            ships.append(cells)
            break

    save_bot_ships(ships)
    print(" Корабли бота сгенерированы")


def save_bot_ships(ships):
    with open("data/bot_ships.csv", "w") as f:
        for ship in ships:
            line = ";".join(f"{x},{y}" for x, y in ship)
            f.write(line + "\n")

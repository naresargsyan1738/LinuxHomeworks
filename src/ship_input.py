import csv
from src.utils import in_bounds, ship_touches_existing

SHIP_SIZES = [4, 3, 3, 2, 2, 2, 1, 1, 1, 1]


def get_player_ships():
    ships = []

    print("Ввод кораблей игрока")
    print("Формат: x y orientation(H/V)")

    for size in SHIP_SIZES:
        while True:
            try:
                raw = input(f"Корабль размера {size}: ").split()
                x = int(raw[0])
                y = int(raw[1])
                orient = raw[2].upper()

                if orient not in ("H", "V"):
                    print("Ориентация должна быть H или V")
                    continue

                cells = []
                for i in range(size):
                    cx = x + i if orient == "V" else x
                    cy = y + i if orient == "H" else y
                    cells.append((cx, cy))

                if not all(in_bounds(cx, cy) for cx, cy in cells):
                    print("Выход за пределы поля")
                    continue

                if ship_touches_existing(cells, ships):
                    print("Корабли касаются друг друга")
                    continue

                ships.append(cells)
                break

            except:
                print("Ошибка ввода")

    save_player_ships(ships)
    print("Корабли сохранены")


def save_player_ships(ships):
    with open("data/player_ships.csv", "w") as f:
        for ship in ships:
            line = ";".join(f"{x},{y}" for x, y in ship)
            f.write(line + "\n")

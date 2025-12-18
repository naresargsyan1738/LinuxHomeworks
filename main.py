from src.ship_input import get_player_ships
from src.bot_generation import generate_bot_ships
from src.gameplay import start_game

def main():
    print("=== Battleship ===")

    get_player_ships()
    generate_bot_ships()
    start_game()

if __name__ == "__main__":
    main()

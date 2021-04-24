import pandas as pd
import numpy as np
import argparse


parser = argparse.ArgumentParser(description='Pass the rows, columns and diffulty of the board')
parser.add_argument('--rows',  '-r', type=int, help='Rows for the board', default=10)
parser.add_argument('--columns', '-c', type=int, help='Columns for the board', default=10)
parser.add_argument('--difficulty', '-d', type=int, help='Board diffulty level', default=2)
args = parser.parse_args()

rows = args.rows
cols = args.columns
difficulty = args.difficulty


def create_random_grid(rows, cols, difficulty):
    df = pd.DataFrame(np.random.randint(0, 10, (rows, cols)))
    df = df.applymap(lambda x: 0 if x >= difficulty else 1)
    df.iloc[0, 0], df.iloc[-1, -1] = 0, 0

    df.to_csv('board.csv', header=None, index=False, line_terminator=',\n')


if __name__ == '__main__':
    create_random_grid(rows, cols, difficulty)
    print("Board Created!")

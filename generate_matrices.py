import numpy as np
import scipy as sp
import pandas as pd
import sklearn

import argparse


def main():
    parser = argparse.ArgumentParser(description='Matrix generator')
    
    parser.add_argument('-s', '--size', default=100, type=int, help='Size of the matrix')
    parser.add_argument('-f', '--file_name', default=None, type=str, help='file name of the matrix')
    
    args = parser.parse_args()
    size = args.size
    name = args.file_name
    if name is None:
        name = f'{size}x{size}'
    name = name.split('.')[0]
    name = name + '.txt'
    
    matrix = generate_matrix(size)
    
    df = pd.DataFrame(data=matrix.astype(float))
    df.to_csv(name, sep=' ', header=False, float_format='%.3f', index=False)
    print(f'Matrix of size {size} was generated and saved into file {name}')


def generate_matrix(size):
    mat = np.random.random_sample((size, size)) - np.eye(size)
    return mat


if __name__ == '__main__':
    main()
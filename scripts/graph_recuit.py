import pandas as pd
import numpy as np


input_file = "../data.csv"
data = pd.read_csv(input_file, delimiter="\t", columns=[['temperature', 'current', 'best']])
print("Data read.")
data.plot(x="temperature", y="current",kind="line")

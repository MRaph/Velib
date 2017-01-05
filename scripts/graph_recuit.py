import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

input_file = "../build/data1.csv"
data = pd.read_csv(input_file, delimiter="\t")
data.columns=['temperature', 'current', 'best']
print("Data read.")
plt.figure()
data.plot(x="temperature", y="current",kind="line")
plt.show()

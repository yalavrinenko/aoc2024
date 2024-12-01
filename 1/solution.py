import numpy as np

data = np.loadtxt("./1/data.in")

data[:, 0] = np.sort(data[:, 0])
data[:, 1] = np.sort(data[:, 1])

distance = np.abs(data[:, 0] - data[:, 1])
total_distance =np.sum(distance)

print("Part 1: {}".format(int(total_distance)))

similarity_score = 0
for value in data[:, 0]:
  similarity_score += value * np.count_nonzero(data[:, 1] == value)

print("Part 2: {}".format(int(similarity_score)))
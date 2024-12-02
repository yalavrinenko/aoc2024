import numpy as np

def is_good(data):
  data = data[:-1] - data[1:]

  if np.count_nonzero(abs(data) > 3) == 0 and 0 not in data:
    data = np.sign(data)
    return np.all(data == data[0])
  
  return False

count = 0
for line in open("./2/data.in"):
  data = np.array([int(v) for v in line.split(' ')])

  count += is_good(data)

print("Part 1: {}".format(count))


count = 0
for line in open("./2/data.in"):
  data = np.array([int(v) for v in line.split(' ')])

  if (is_good(data)):
    count += 1
  else:
    for i in range(len(data)):
      reduce_seq = np.delete(data, i)
      if (is_good(reduce_seq)):
        count += 1
        break

  
print("Part 2: {}".format(count))
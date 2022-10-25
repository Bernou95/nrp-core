import numpy as np
import matplotlib.pyplot as plt

o1 = np.genfromtxt("output.csv", delimiter=',', skip_header=0)
o2 = np.genfromtxt("output_nrp.csv", delimiter=',', skip_header=1)

# t1 = np.genfromtxt("target.csv", delimiter=',', skip_header=0)
# t2 = np.genfromtxt("target_nrp.csv", delimiter=',', skip_header=1)

ii1 = np.genfromtxt("input.csv", delimiter=',', skip_header=0)
ii2 = np.genfromtxt("input_nrp.csv", delimiter=',', skip_header=1)

s1 = np.genfromtxt("spikes.csv", delimiter=',', skip_header=0)
s2 = np.genfromtxt("spikes_nrp.csv", delimiter=',', skip_header=1)

r71 = []
r72 = []
r81 = []
r82 = []

i1 = 0
i2 = 0
for i in range(100):
    r71.append(0)
    r72.append(0)
    r81.append(0)
    r82.append(0)

    while i1 < len(s1[:,0]) and s1[i1,1] < (i+1) * 100:
        if s1[i1,0] < 7.5:
            r71[i] += 0.1
        else:
            r81[i] += 0.1
        i1 += 1

    while i2 < len(s2[:,0]) and s2[i2,1] < (i+1) * 100:
        if s2[i2,0] < 7.5:
            r72[i] += 0.1
        else:
            r82[i] += 0.1
        i2 += 1


# plt.plot([0.1 * i for i in range(100)], r71, color='tab:blue', linestyle='-')
# plt.plot([0.1 * i for i in range(100)], r72, color='tab:blue', linestyle='--')

# plt.plot([0.1 * i for i in range(100)], r81, color='tab:red', linestyle='-')
# plt.plot([0.1 * i for i in range(100)], r82, color='tab:red', linestyle='--')

# plt.plot(o1[:,0], o1[:,1], color='tab:green', linestyle='-')
# plt.plot(o2[:,0], o2[:,1], color='tab:green', linestyle='--')

plt.plot(o1[:,0], o1[:,2], color='tab:blue', linestyle='-')
plt.plot(o2[:,0], o2[:,2], color='tab:blue', linestyle='--')

# plt.plot(o1[:,0], o1[:,3], color='tab:red', linestyle='-')
# plt.plot(o2[:,0], o2[:,3], color='tab:red', linestyle='--')

# plt.plot(t1[:,0], t1[:,1]/3, color='tab:gray', linestyle='-')
# plt.plot(t2[:,0], t2[:,1]/3, color='tab:gray', linestyle='--')

# plt.plot(ii1[:,0], (ii1[:,1]-500)/10000, color='tab:orange', linestyle='-')
# plt.plot(ii2[:,0], (ii2[:,1]-500)/10000, color='tab:orange', linestyle='--')

# plt.plot(ii1[:,0], (ii1[:,2]-500)/10000, color='tab:purple', linestyle='-')
# plt.plot(ii2[:,0], (ii2[:,2]-500)/10000, color='tab:purple', linestyle='--')

plt.show()

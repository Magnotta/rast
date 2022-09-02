import matplotlib.pyplot as plt
import numpy as np

alldata = np.genfromtxt("../dados/dados.txt", delimiter=',', dtype=np.float64)
datalen, datawid = alldata.shape

x = np.arange(datalen, dtype=np.float64)

data1 = []
data2 = []
data3 = []
data4 = []

for tup in alldata:
       data1.append(tup[0])
       data2.append(tup[1])
       data3.append(tup[2])
       data4.append(tup[3])

data1 = np.array(data1, dtype=np.float64)
data2 = np.array(data2, dtype=np.float64)
data3 = np.array(data3, dtype=np.float64)

fig, axes1 = plt.subplots()

line1, = axes1.plot(x, data1, label="LDR esq cima")
line2, = axes1.plot(x, data2, label="LDR dir cima")
line3, = axes1.plot(x, data3, label="LDR esq baixo")
line4, = axes1.plot(x, data4, label="LDR dir baixo")

axes1.set_xlabel("Tempo (min)")
axes1.set_ylabel("Leitura LDR")
axes1.set_title("Rastreamento solar 30/08 - Teste 1")
axes1.set_ylim(bottom=4, top=5.5)
axes1.set_xlim(left=0, right=datalen)

axes1.legend(loc="lower center")

plt.show()
fig.savefig("../dados/graph.jpg")
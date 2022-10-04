from matplotlib import pyplot as plt   
from matplotlib import style   


fig, axs = plt.subplots(2, 1)

# seq mul
mat_sizes = [1000, 1500, 2000, 3000]
times = [0.21, 0.8, 2.4, 8.57]
axs[0].plot(mat_sizes, times)   

#shtrasen
mat_sizes = [256, 512, 1024, 2048]
times = [0.026, 0.19, 0.914, 5.8]
axs[1].plot(mat_sizes, times)  
   
plt.show()
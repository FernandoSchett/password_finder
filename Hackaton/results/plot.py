from matplotlib import pyplot as plt
import numpy as np
import sys

def Sort(sub_li):
    l = len(sub_li)
    for i in range(0, l):
        for j in range(0, l-i-1):
            if (sub_li[j][1] > sub_li[j + 1][1]):
                tempo = sub_li[j]
                sub_li[j]= sub_li[j + 1]
                sub_li[j + 1]= tempo
    return sub_li

arguments = sys.argv
plotType = arguments[1]

f=open("seq_time.txt", "r")
lines=f.readlines()
seq_time=[]
for line in lines:
    seq_time.append(line.split(' ')[0])
    seq_time.append(float(line.split(' ')[1]))
f.close()
#print(seq_time)

#OMP
if(plotType == 'omp'):
    print("BUILDING OMP GRAPH")
    f=open("omp_time.txt", "r")
    lines=f.readlines()
    x_axis = []
    y_axis = []
    omp_time=[]
    for line in lines:
        #omp_time.append(line.split(' ')[0])
        omp_time.append([int(line.split(' ')[1]), float(line.split(' ')[2])])
        
        x_axis.append(int(line.split(' ')[1]))
        y_axis.append(float(line.split(' ')[2]))
    f.close()   
    
    omp_time = Sort(omp_time)
    #print("OMP TIMES: {}".format(omp_time))
    
    x_size=[]
    y_size=[]
    
    # for values in omp_time:
    #     x_axis.append(str(values[0]))
    #     y_axis.append(str(values[1]))
    
    plt.plot(x_axis, y_axis)

    plt.xticks(x_axis)
    plt.yticks(y_axis)
     
    plt.grid(True)
    
    plt.xlabel('Threads')
    plt.ylabel('Times')
    plt.title('OMP Threads x Times')
    
    plt.savefig('omp_graph.png')

#MPI
if(plotType == 'mpi'):
    print("BUILDING MPI GRAPH")
    f=open("mpi_time.txt", "r")
    lines=f.readlines()
    x_axis = []
    y_axis = []
    mpi_time=[]
    for line in lines:
        #omp_time.append(line.split(' ')[0])
        mpi_time.append([int(line.split(' ')[1]), float(line.split(' ')[2])])
        
        x_axis.append(int(line.split(' ')[1]))
        y_axis.append(float(line.split(' ')[2]))
    f.close()   
    
    mpi_time = Sort(mpi_time)
        
    x_size=[]
    y_size=[]
    
    plt.plot(x_axis, y_axis)

    plt.xticks(x_axis)
    plt.yticks(y_axis)
     
    plt.grid(True)
    
    plt.xlabel('Processes')
    plt.ylabel('Times')
    plt.title('MPI Processes x Times')
    
    plt.savefig('mpi_graph.png')

#HYBRID
if(plotType == 'hybrid'):
    print("BUILDING HYBRID GRAPH")
    f=open("hybrid_time.txt", "r")
    lines=f.readlines()
    x_axis = []
    y_axis = []
    hybrid_time=[]
    for line in lines:
        hybrid_time.append([[int(line.split(' ')[1]), int(line.split(' ')[2])], float(line.split(' ')[3])])
        
        x_axis.append(str([int(line.split(' ')[1]), int(line.split(' ')[2])]))
        y_axis.append(float(line.split(' ')[3]))
    f.close()   
    
    hybrid_time = Sort(hybrid_time)
    #print("HYBRID TIMES: {}".format(hybrid_time))
    
    x_size=[]
    y_size=[]
    
    # for values in omp_time:
    #     x_axis.append(str(values[0]))
    #     y_axis.append(str(values[1]))
    
    fig = plt.figure()
    fig.set_figwidth(10)
    fig.set_figheight(10)
    plt.plot(x_axis, y_axis)

    plt.xticks(x_axis, rotation=90)
    plt.yticks(y_axis)
     
    plt.grid(True)
    
    plt.xlabel('[PROCESS, THREAD]')
    plt.ylabel('Times')
    plt.title('HYBRID [PROCESS, THREAD] x Times')
    
    plt.savefig('hybrid_graph.png')

#CUDA
if(plotType == 'cuda'):
    print("BUILDING CUDA GRAPH")
    f=open("cuda_time.txt", "r")
    lines=f.readlines()
    x_axis = []
    y_axis = []
    cuda_time=[]
    for line in lines:
        cuda_time.append([int(line.split(' ')[1]), float(line.split(' ')[2])])
        
        x_axis.append(int(line.split(' ')[1]))
        y_axis.append(float(line.split(' ')[2]))
    f.close()   
    
    cuda_time = Sort(cuda_time)
    
    x_size=[]
    y_size=[]
    
    fig = plt.figure()
    fig.set_figwidth(5)
    fig.set_figheight(7)
    plt.plot(x_axis, y_axis)

    plt.xticks(x_axis, rotation=90)
    plt.yticks(y_axis)
     
    plt.grid(True)
    
    plt.xlabel('Blocks')
    plt.ylabel('Times')
    plt.title('CUDA Blocks x Times')
    
    plt.savefig('cuda_graph.png')

#Time and SpeedUp
if(plotType == "speedup"):
    #print("READING VALUES")
    
    #print("OMP...")
    f=open("omp_time.txt", "r")
    lines=f.readlines()
    omp_time=[]
    for line in lines:
        omp_time.append([int(line.split(' ')[1]), float(line.split(' ')[2])])
    f.close()
    
    #print("MPI...")
    f=open("mpi_time.txt", "r")
    lines=f.readlines()
    mpi_time=[]
    for line in lines:
        mpi_time.append([int(line.split(' ')[1]), float(line.split(' ')[2])])
    f.close() 
    
    #print("HYBRID...")
    f=open("hybrid_time.txt", "r")
    lines=f.readlines()
    hybrid_time=[]
    for line in lines:
        hybrid_time.append([[int(line.split(' ')[1]), int(line.split(' ')[2])], float(line.split(' ')[3])])
    f.close() 
    
    #print("CUDA...")
    f=open("cuda_time.txt", "r")
    lines=f.readlines()
    cuda_time=[]
    for line in lines:
        cuda_time.append([int(line.split(' ')[1]), float(line.split(' ')[2])])
    f.close()   
    
    #print("SORTING VALUES")
    omp_time = Sort(omp_time)
    mpi_time = Sort(mpi_time)
    hybrid_time = Sort(hybrid_time)
    cuda_time = Sort(cuda_time)
    
    sequential = int(seq_time[1]/seq_time[1])
    best_omp = int(seq_time[1]/omp_time[0][1])
    best_mpi = int(seq_time[1]/mpi_time[0][1])
    best_hybrid = int(seq_time[1]/hybrid_time[0][1])
    best_cuda = int(seq_time[1]/1)
    
    #print("PLOTING SPEEDUPS")
    
    xLabels = ['Sequential', 'OMP', 'MPI', 'HYBRID', 'CUDA']
    speedups = [sequential, best_omp, best_mpi, best_hybrid, best_cuda]
    yLabels = [str(sequential)+'X', str(best_omp)+'X', str(best_mpi)+'X', str(best_hybrid)+'X', str(best_cuda)+'X']
    print("SPEEDUPTS: {}".format(speedups))
    
    plt.plot(xLabels, speedups)
    plt.xticks(xLabels)
    plt.yticks(speedups, yLabels)
    #plt.yticks(speedups)

    plt.grid(True)
    
    plt.xlabel('METHODS')
    plt.ylabel('VALUES')
    plt.title('SPEEDUPS')
    
    plt.savefig('speedup_graph.png')
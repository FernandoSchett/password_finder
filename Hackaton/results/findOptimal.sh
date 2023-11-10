#!/bin/bash

METHOD=$1
WORD=$2
MIN_OMP=4
MIN_MPI=32
MIN_CUDA=4
MAX_OMP=256
MAX_MPI=32 #MAX NUMBER FOR OGBON ON NODE c039 is 36
MAX_CUDA=128
LASTTIME=10000
CURRENTTIME=10000
BEST=0

if [ $METHOD = 'sequential' ]
then
    ../bin/seq_brute $WORD
else
    if [ $METHOD = 'omp' ]
    then
        #for i in $(seq 1 $MAX_OMP)
        for ((i = $MIN_OMP; i <= $MAX_OMP; i*=2));
        do
            ../bin/omp_brute $WORD $i
            CURRENTTIME=$(awk 'END{printf "%f", $3}' omp_time.txt)
            if [ 1 -eq "$(echo "$CURRENTTIME <= $LASTTIME" | bc)" ]
            then
                LASTTIME=$CURRENTTIME
                CURRENTTIME=10000
            else
                #$BEST = $(awk 'END{print $2}' omp_time.txt) #SE FOR VOLTAR A USAR ESSA OPÇÃO, NECESSARIO CORRIGIR END PARA 1 LINHA ACIMA DO END
                BEST=$(($i/2))
                #echo "BEST OMP THREAD NUM >> $BEST" >> omp_time.txt
                #break
            fi
        done
    else
        if [ $METHOD = 'mpi' ]
        then
            for ((i = $MIN_MPI; i <= $MAX_MPI; i*=2));
            do
                mpirun -np $i --allow-run-as-root ../bin/mpi_brute $WORD
                CURRENTTIME=$(awk 'END{printf "%f", $3}' mpi_time.txt)
                if [ 1 -eq "$(echo "$CURRENTTIME <= $LASTTIME" | bc)" ]
                then
                    LASTTIME=$CURRENTTIME
                    CURRENTTIME=10000
                else
                    #$BEST = $(awk 'END{print $2}' mpi_time.txt)
                    BEST=$(($i/2))
                    #echo "BEST MPI NP >> $BEST" >> mpi_time.txt
                    #break
                fi
            done
        else
            if [ $METHOD = 'hybrid' ]
            then
                for ((i = $MIN_MPI; i <= $MAX_MPI; i*=2));
                do
                    for ((j = $MIN_OMP; j <= $MAX_OMP; j*=2));
                    do
                        mpirun -np $i --allow-run-as-root ../bin/hybrid_brute $WORD $j
                        CURRENTTIME=$(awk 'END{print "%f", $4}' hybrid_time.txt)
                        if [ 1 -eq "$(echo "$CURRENTTIME <= $LASTTIME" | bc)" ]
                        then
                            LASTTIME=$CURRENTTIME
                            CURRENTTIME=10000
                        else
                            #$BEST = $(awk 'END{print $2}' hybrid_time.txt)
                            BEST=$(($j/2))
                            #echo "BEST HYBRID FOR NP = $i IS THREAD = $BEST WITH TIME = $LASTTIME" >> hybridBests_time.txt
                            #break
                        fi
                    done
                done
            else
                if [ $METHOD = 'cuda' ]
                then
                    for ((i = $MIN_CUDA; i <= $MAX_CUDA; i*=2));
                    do
                        ../bin/cuda_brute $WORD $i
                        CURRENTTIME=$(awk 'END{printf "%f", $3}' omp_time.txt)
                        if [ 1 -eq "$(echo "$CURRENTTIME <= $LASTTIME" | bc)" ]
                        then
                            LASTTIME=$CURRENTTIME
                            CURRENTTIME=10000
                        else
                            #$BEST = $(awk 'END{print $2}' omp_time.txt) #SE FOR VOLTAR A USAR ESSA OPÇÃO, NECESSARIO CORRIGIR END PARA 1 LINHA ACIMA DO END
                            BEST=$(($i/2))
                            #echo "BEST CUDA NUM >> $BEST" >> omp_time.txt
                            #break
                        fi
                    done
                fi
            fi
        fi
    fi
fi

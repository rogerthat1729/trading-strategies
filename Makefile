.PHONY: all DMA DMA++ MACD RSI ADX LINEAR_REGRESSION BASIC PAIRS PAIRS_WITH_STOP_LOSS

all: clean

symbol	?=	"SBIN"
strategy ?= "BASIC"
n ?= 4
x ?= 4
p ?= 1
max_hold_days ?= 28
c1 ?= 2
c2 ?= 0.2
start_date ?= 01/01/2019
end_date ?= 01/01/2020
oversold_threshold ?= 30
overbought_threshold ?= 70
adx_threshold ?= 25
train_start_date ?= 01/01/2018
train_end_date ?= 01/01/2019
symbol1 ?= "SBIN"
symbol2 ?= "ADANIENT"
threshold ?= 2
stop_loss_threshold ?= -5 # garbage value

clean: $(strategy)
	#  rm -f data.csv

BASIC:
	python3 fetch.py $(symbol) $(start_date) $(end_date) $(n) 
	g++ main.cpp executer.cpp -o executer
	./executer $(strategy) $(n) $(x)
	rm -f executer
	
DMA:
	python3 fetch.py $(symbol) $(start_date) $(end_date) $(n) 
	g++ main.cpp executer.cpp -o executer
	./executer $(strategy) $(n) $(x) $(p)
	rm -f executer

DMA++:
	python3 fetch.py $(symbol) $(start_date) $(end_date) $(n) 
	g++ main.cpp executer.cpp -o executer
	./executer $(strategy) $(n) $(x) $(p) $(max_hold_days) $(c1) $(c2)
	rm -f executer

MACD:
	python3 fetch.py $(symbol) $(start_date) $(end_date) 0 
	g++ main.cpp executer.cpp -o executer
	./executer $(strategy) 0 $(x)
	rm -f executer

RSI:
	python3 fetch.py $(symbol) $(start_date) $(end_date) $(n) 
	g++ main.cpp executer.cpp -o executer
	./executer $(strategy) $(n) $(x) $(overbought_threshold) $(oversold_threshold)
	rm -f executer

ADX:
	python3 fetch.py $(symbol) $(start_date) $(end_date) $(n)
	g++ main.cpp executer.cpp -o executer
	./executer $(strategy) $(n) $(x) $(adx_threshold)
	rm -f executer

LINEAR_REGRESSION:
	python3 fetchLR.py $(symbol) $(train_start_date) $(train_end_date) $(start_date) $(end_date) 
	g++ LR.cpp LRexecuter.cpp -o LR
	./LR $(x) $(p)
	rm -f LR
	rm -f traindata.csv
	# rm -f testdata.csv

BEST_OF_ALL:
	python3 fetchLRforBest.py $(symbol) $(start_date) $(end_date)
	python3 fetch.py $(symbol) $(start_date) $(end_date) 50
	g++ -c LR.cpp
	g++ -c main.cpp
	g++ -fopenmp -c best.cpp
	g++ -fopenmp LR.o main.o best.o -o best
	./best
	rm -f best
	rm -f best.o
	rm -f main.o
	rm -f LR.o
	rm -f testdata.csv
	rm -f traindata.csv

PAIRS:
	python3 fetchMP.py $(symbol1) $(start_date) $(end_date) $(n)
	python3 fetchMP.py $(symbol2) $(start_date) $(end_date) $(n)
	g++ -std=c++20 -o MRP MRP.cpp
	./MRP $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(stop_loss_threshold) $(start_date) $(end_date)
	rm -f MRP
	rm -f $(symbol1).csv 
	rm -f $(symbol2).csv


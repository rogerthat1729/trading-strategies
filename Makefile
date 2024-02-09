.PHONY: all DMA DMA++ MACD RSI ADX LINEAR_REGRESSION BASIC PAIRS PAIRS_WITH_STOP_LOSS

all: clean

SYMBOL	?=	"SBIN"
strategy ?= "BASIC"
n ?= 4
x ?= 4
p ?= 1
max_hold_days ?= 28
c1 ?= 2
c2 ?= 0.2
start_date ?= "01/01/2019"
end_date ?= "01/01/2020"
oversold_threshold ?= 30
overbought_threshold ?= 70
adx_threshold ?= 25
train_start_date ?= "01/01/2018"
train_end_date ?= "01/01/2019"
symbol1 ?= "SBIN"
symbol2 ?= "ADANIENT"
threshold ?= 2
stop_loss_threshold ?= 4

clean: $(strategy)
	rm -f main
	rm -f data.csv

BASIC:
	python3 fetch.py $(SYMBOL) $(start_date) $(end_date) $(n) 
	g++ -std=c++20 -o main main.cpp
	./main $(strategy) $(n) $(x) $(start_date) $(end_date)
	
DMA:
	python3 fetch.py $(SYMBOL) $(start_date) $(end_date) $(n) 
	g++ -std=c++20 -o main main.cpp
	./main $(strategy) $(n) $(x) $(p) $(start_date) $(end_date)

DMA++:
	python3 fetch.py $(SYMBOL) $(start_date) $(end_date) $(n) 
	g++ -std=c++20 -o main main.cpp
	./main $(strategy) $(x) $(p) $(n) $(max_hold_days) $(c1) $(c2) $(start_date) $(end_date)

MACD:
	python3 fetch.py $(SYMBOL) $(start_date) $(end_date) 0 
	g++ -std=c++20 -o main main.cpp
	./main $(strategy) 0 $(x) $(start_date) $(end_date) 

RSI:
	python3 fetch.py $(SYMBOL) $(start_date) $(end_date) $(n) 
	g++ -std=c++20 -o main main.cpp
	./main $(strategy) $(x) $(n) $(oversold_threshold) $(overbought_threshold) $(start_date) $(end_date)

ADX:
	python3 fetch.py $(SYMBOL) $(start_date) $(end_date) $(n)
	g++ -std=c++20 -o main main.cpp
	./main $(strategy) $(n) $(x) $(adx_threshold) $(start_date) $(end_date)

LINEAR_REGRESSION:
	python3 fetch.py $(SYMBOL) $(start_date) $(end_date) 1
	g++ -std=c++20 -o main main.cpp
	./main $(strategy) $(x) $(p) $(train_start_date) $(train_end_date) $(start_date) $(end_date)

PAIRS:
	python3 fetch.py $(symbol1) $(start_date) $(end_date) $(n) 1
	python3 fetch.py $(symbol2) $(start_date) $(end_date) $(n) 1
	g++ -std=c++20 -o main main.cpp
	./main $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(start_date) $(end_date)

PAIRS_WITH_STOP_LOSS:
	python3 fetch.py $(symbol1) $(start_date) $(end_date) $(n) 1
	python3 fetch.py $(symbol2) $(start_date) $(end_date) $(n) 1
	g++ -std=c++20 -o main main.cpp
	./main $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(stop_loss_threshold) $(start_date) $(end_date)

all:clean

SYMBOL	?=	"SBIN"
strategy ?= "BASIC"
n ?= 4
x ?= 4
start_date ?= "01/01/2019"
end_date ?= "01/01/2020"

run:
	pip3 install -r requirements.txt
	python3 fetch.py $(SYMBOL) $(start_date) $(end_date) $(n)
	g++ -std=c++20 -o main main.cpp
	./main $(strategy) $(n) $(x)

clean: run
	rm -f main
	rm -f data.csv

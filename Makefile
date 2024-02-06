SYMBOL	?=	"SBIN"
strategy ?= "BASIC"
n ?= 1
x ?= 1
start_date ?= "2019-01-01"
end_date ?= "2020-01-01"

.DO_IT:run

run:
	pip3 install -r requirements.txt
	python3 fetch.py $(SYMBOL) $(start_date) $(end_date)
	g++ -std=c++20 -o main main.cpp
	./main $(strategy) $(n) $(x)

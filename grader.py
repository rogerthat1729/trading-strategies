import sys, os
import random
import datetime

def best_of_all(ticker,start_date,end_date):
    return "make strategy=BEST_OF_ALL symbol="+ticker+"start_date="+start_date+"end_date="+end_date
def basic(ticker,start_date,end_date, n, x):
    return "make strategy=BASIC symbol="+ticker+" start_date="+start_date+" end_date="+end_date+" n="+str(n)+" x="+str(x)
def dma(ticker,start_date,end_date, n, x, p):
    return "make strategy=DMA symbol="+ticker+" start_date="+start_date+" end_date="+end_date+" n="+str(n)+" x="+str(x)+" p="+str(p)
def dmapp(ticker,start_date,end_date, n, x, p, max_hold_days, c1,c2):
    return "make strategy=DMA++ symbol="+ticker+" start_date="+start_date+" end_date="+end_date+" n="+str(n)+" x="+str(x)+" p="+str(p)+" max_hold_days="+str(max_hold_days)+" c1="+str(c1)+" c2="+str(c2)
def macd(ticker,start_date,end_date,x):
    return "make strategy=MACD symbol="+ticker+" start_date="+start_date+" end_date="+end_date+" x="+str(x)
def adx(ticker,start_date,end_date,n,x,adx_threshold):
    return "make strategy=ADX symbol="+ticker+" start_date="+start_date+" end_date="+end_date+" n="+str(n)+" x="+str(x)+" adx_threshold="+str(adx_threshold)
def rsi(ticker,start_date,end_date,n,x,overbought_threshold,oversold_threshold):
    return "make strategy=RSI symbol="+ticker+" start_date="+start_date+" end_date="+end_date+" n="+str(n)+" x="+str(x)+" overbought_threshold="+str(overbought_threshold)+" oversold_threshold="+str(oversold_threshold)
def linear_regression(ticker,start_date,end_date,train_start_date, train_end_date, x,p):
    return "make strategy=LINEAR_REGRESSION symbol="+ticker+" start_date="+start_date+" end_date="+end_date+" train_start_date="+train_start_date+" train_end_date="+train_end_date+" x="+str(x)+" p="+str(p)
def pairs(ticker1,ticker2,start_date,end_date,n,x, threshold):
    return "make strategy=PAIRS symbol1="+ticker1+" symbol2="+ticker2+" start_date="+start_date+" end_date="+end_date+" n="+str(n)+" x="+str(x)+" threshold="+str(threshold)
def pair_stop(ticker1,ticker2,start_date,end_date,n,x,threshold,stop_loss_threshold):
    return "make strategy=PAIRS symbol1="+ticker1+" symbol2="+ticker2+" start_date="+start_date+" end_date="+end_date+" n="+str(n)+" x="+str(x)+" threshold="+str(threshold)+" stop_loss_threshold="+str(stop_loss_threshold)

tickers_list=["SBIN","ADANIENT","RELIANCE","TATASTEEL"]

start_date = datetime.date(2017, 2, 1)
end_date = datetime.date(2023, 1, 31)
train_start_date = datetime.date(2015, 1, 1)
train_end_date = datetime.date(2017, 1, 31)
n = 10
x = 5
p = 5
c1 = 0.2
c2 = 2
max_hold_days = 10
adx_threshold = 25
overbought_threshold = 70
oversold_threshold = 30
threshold = 0.5
stop_loss_threshold = 4

def date_string(date):
    return date.strftime("%d/%m/%Y")

def read_final_pnl():
    with open("final_pnl.txt", "r") as file:
        return file.read()

results = {ticker:{"BASIC":0,"DMA":0,"DMA++":0,"MACD":0,"ADX":0,"RSI":0,"LINEAR_REGRESSION":0,"PAIRS":0,"PAIR_STOP":0} for ticker in tickers_list}
for ticker in tickers_list:
    os.system(basic(ticker,date_string(start_date),date_string(end_date),n,x))
    results[ticker]["BASIC"] = read_final_pnl()
    os.system(dma(ticker,date_string(start_date),date_string(end_date),n,x,p))
    results[ticker]["DMA"] = read_final_pnl()
    os.system(dmapp(ticker,date_string(start_date),date_string(end_date),n,x,p,max_hold_days,c1,c2))
    results[ticker]["DMA++"] = read_final_pnl()
    os.system(macd(ticker,date_string(start_date),date_string(end_date),x))
    results[ticker]["MACD"] = read_final_pnl()
    os.system(adx(ticker,date_string(start_date),date_string(end_date),n,x,adx_threshold))
    results[ticker]["ADX"] = read_final_pnl()
    os.system(rsi(ticker,date_string(start_date),date_string(end_date),n,x,overbought_threshold,oversold_threshold))
    results[ticker]["RSI"] = read_final_pnl()
    os.system(linear_regression(ticker,date_string(start_date),date_string(end_date),date_string(train_start_date),date_string(train_end_date),x,p))
    results[ticker]["LINEAR_REGRESSION"] = read_final_pnl()
    os.system(pairs(ticker,"HDFCBANK",date_string(start_date),date_string(end_date),n,x,threshold))
    results[ticker]["PAIRS"] = read_final_pnl()
    os.system(pair_stop(ticker,tickers_list[0],date_string(start_date),date_string(end_date),n,x,threshold,stop_loss_threshold))
    results[ticker]["PAIR_STOP"] = read_final_pnl()
import csv

output_file = "results.csv"

with open(output_file, "w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow(["ticker", "strategy", "result"])
    for ticker in results:
        for strategy in results[ticker]:
            writer.writerow([ticker, strategy, results[ticker][strategy]])
print("Results written to", output_file)
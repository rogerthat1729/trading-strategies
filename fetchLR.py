from jugaad_data.nse import stock_df
from datetime import datetime
from dateutil.relativedelta import relativedelta
import pandas as pd
import sys

sym = sys.argv[1]
tsd = sys.argv[2]
ted = sys.argv[3]
sd = sys.argv[4]
ed = sys.argv[5]
cols = ["DATE", "HIGH", "LOW", "PREV. CLOSE", "CLOSE", "OPEN", "VWAP", "NO OF TRADES"]

def give_df(sym, sad, ead, cols):
    start_date = datetime.strptime(sad, "%d/%m/%Y").date()
    end_date = datetime.strptime(ead, "%d/%m/%Y").date()
    csv_start_date = start_date - relativedelta(days = 5)

    df = stock_df(symbol=sym, from_date=csv_start_date,to_date=end_date, series="EQ")
    df = df[cols]
    df["DATE"] = pd.to_datetime(df["DATE"]).dt.strftime("%d/%m/%Y")
    df = df.iloc[::-1].reset_index(drop=True)
    idx_start = df[df["DATE"] == sad].index[0]
    idx_start -= 1
    idx_start = max(0, idx_start)
    df1 = df.iloc[idx_start:]
    return df1

df_train = give_df(sym, tsd, ted, cols)
df_train.to_csv("traindata.csv")

df_test = give_df(sym, sd, ed, cols)
df_test.to_csv("testdata.csv")
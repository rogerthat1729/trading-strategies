from jugaad_data.nse import stock_df
from datetime import datetime
from dateutil.relativedelta import relativedelta
import pandas as pd
import sys

sym = sys.argv[1]
sd = sys.argv[2]
ed = sys.argv[3]
n = int(sys.argv[4])
cols = ["DATE", "HIGH", "LOW", "PREV. CLOSE", "CLOSE"]
    
start_date = datetime.strptime(sd, "%d/%m/%Y").date()
end_date = datetime.strptime(ed, "%d/%m/%Y").date()
csv_start_date = start_date - relativedelta(days = n+7)

#get correct start date
temp_df = stock_df(symbol=sym, from_date=start_date,to_date=start_date + relativedelta(days = 7), series="EQ")
temp_df = temp_df.iloc[::-1].reset_index(drop=True)
temp_df["DATE"] = pd.to_datetime(temp_df["DATE"]).dt.strftime("%d/%m/%Y")
sad = temp_df.iloc[0]["DATE"]
# got the correct start date

df = stock_df(symbol=sym, from_date=csv_start_date,to_date=end_date, series="EQ")
df = df[cols]
df["DATE"] = pd.to_datetime(df["DATE"]).dt.strftime("%d/%m/%Y")
df = df.iloc[::-1].reset_index(drop=True)
idx_start = df[df["DATE"] == sad].index[0]
idx_start -= n
idx_start = max(0, idx_start)
df1 = df.iloc[idx_start:]
df1.to_csv("data.csv")

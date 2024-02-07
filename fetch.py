from jugaad_data.nse import stock_df
from datetime import datetime
from dateutil.relativedelta import relativedelta
import pandas as pd
import sys

sym = sys.argv[1]
sd = sys.argv[2]
ed = sys.argv[3]
n = int(sys.argv[4])

start_date = datetime.strptime(sd, "%d/%m/%Y").date()
end_date = datetime.strptime(ed, "%d/%m/%Y").date()
csv_start_date = start_date - relativedelta(days = n+7)

df = stock_df(symbol=sym, from_date=csv_start_date,to_date=end_date, series="EQ")
df = df[["DATE", "CLOSE"]]
df["DATE"] = pd.to_datetime(df["DATE"]).dt.strftime("%d/%m/%Y")
df = df.iloc[::-1].reset_index(drop=True)
idx_start = df[df["DATE"] == sd].index[0]
idx_start -= n+1
idx_start = max(0, idx_start)
df1 = df.iloc[idx_start:]
df1.to_csv("data.csv")

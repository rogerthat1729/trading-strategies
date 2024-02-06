from jugaad_data.nse import stock_df
from datetime import datetime
import pandas as pd
import sys

sym = sys.argv[1]
sd = sys.argv[2]
ed = sys.argv[3]


# Convert to date object
start_date = datetime.strptime(sd, "%d/%m/%Y").date()
end_date = datetime.strptime(ed, "%d/%m/%Y").date()


df = stock_df(symbol=sym, from_date= start_date,to_date=end_date, series="EQ")
df = df[["DATE", "CLOSE"]]
df = df.iloc[::-1].reset_index(drop=True)
df.to_csv("data.csv")

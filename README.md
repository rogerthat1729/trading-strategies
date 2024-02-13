# trading-strategies

I have explained my strategy with respect to what is happening to S1 for ease of understanding.

# Our Understanding/ Assumptions  : 
1. Our strategy works/ we can make a resonable prediction when abs(zscore) less than abs(stop_loss_threshold). 
2. Our "resonable prediction"/"motivation for the trade" is that abs(zscore) should return to 0. buy
3. Say today's zscore is > threshold we can say that if we generate a sell signal then we can be in profit in the future. 
4. But if today's zscore is > stop_loss_threshold too then we are not sure about the behaviour of the stock therefore we do not generate a signal.
5. Now in generating a sell signal if we have open buy positions (i.e we have bought the stocks of S1 ) then we clear of the oldest bought position with our todays sell signal. else we add this signal among our open sell positions.
6. After this has been done, we clear off all the old positions that are behaving unexpectedly(i.e their abs(zscore) is greater than abs(stop_loss_threshold) ).

7. There are certain edge cases that have also been handled here. Say my porfolio is -x right now (i.e x stocks of S1 have been sold/shorted) then if we are getting a sell signal today according to the zscore then we "can" move forward with it provided we know that certain old sell positions need to be cleared off today thereby making room for today's sell.
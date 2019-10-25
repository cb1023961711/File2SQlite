This project is for cnverting File to SQL db

Filename is sender position + receiver position + sender sector + receiver sector;
There are MatrixH and channel fading in files and MatrixH is 355x2 channel fading is in line 356. 

SQL db: Table is named ChannelMap,all the elements are ID,position(filename),MatrixH,channel fading.
MatrixH is stored in blob.

TODO:
1. simplify codes
2. Add multithread to speed up the converting rate
3. Try this in ubuntu 
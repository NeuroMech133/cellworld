# This file is used to read and print the Q tables generated from the training code
# Created for Assignment-6 for BME 468
# Author: Nish Mohith Kurukuti

import pickle

#Importing the Qtable - Change this for the specific table being used
d = pickle.load(open("Q_table_World_10_08.sav", "rb"))

# Printing Qtable
for k,v in d.items():
    print(k, v)

# This code computes distance between two given points
# Created for Assignment-7 for BME 468
# Author: Nish Mohith Kurukuti


def find_distance(loc_1, loc_2):
    #print(loc_1)
    #print(loc_2)

    x1 = loc_1.x
    y1 = loc_1.y

    x2 = loc_2.x
    y2 = loc_2.y

    distance = ((x1 - x2)**2 + (y1 - y2)**2)**0.5
    #print(distance)
    return distance
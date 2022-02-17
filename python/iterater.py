# This file computes the best possible policy through given iterations of RL and generates Qtables
# Created for Assignment-7 for BME 468
# Author: Nish Mohith Kurukuti

from testing import *
from training import *

iter_thresh = 5
world_occlusion = "10_08"
iter_status = []
iter = 1000
while iter <= 100000:
#for iter in range(50):
    iter_status_vals = []
    for i in range(iter_thresh):
        d = training(world_occlusion,iter)
        status = testing(world_occlusion,d)
        iter_status_vals.append(status)
    iter_status.append(sum(iter_status_vals)/iter_thresh)
    iter = iter+1000

print(iter_status)
# This file computes the best possible policy through given iterations of RL and generates Qtables
# Created for Assignment-7 for BME 468
# Author: Nish Mohith Kurukuti
# Dependency: navigate_function.py

import random
from time import sleep
#from cellworld import *
from navigate_functions import *
import pickle

def training(occlusions,iter):
    # create an instance of World class
    current_cell_id = 0
    #occlusions = "10_08" # World used for training
    w = World.get_from_parameters_names('hexagonal','mice',occlusions)
    w_cells_ids = w.cells.get('id')
    current_cell_location = w.cells[current_cell_id].location
    rotation = 90

    # create instance of display class
    #dis = Display(w, fig_size=(6,6), padding=0, cell_edge_color="lightgrey", background_color="black", habitat_edge_color="lightgrey", animated=True)

    # create the barebones for the qtable
    d = {
        i:{"ne":0, "e":0, "se":0, "sw":0, "w":0, "nw":0} 
        for i in range(len(w_cells_ids)) 
    }
    #print(d[current_cell_id].values())
    direction = "e" # Starting direction
    search_type = 1 # 1
    counter = 0

    # Training starts here
    NUM_ITERATIONS = iter
    SHOW_STEPS = False # Change this to True for plotting the world
    for i in range(NUM_ITERATIONS):
        if SHOW_STEPS:
            # add agent and circle to figure using method from Display class
            dis.agent(Step(agent_name="prey", location = current_cell_location, rotation = rotation))
            dis.circle(current_cell_location, .03, 'red')
            dis.update()
            print(i)
            sleep(0.01)
        
        else:
            progress = int((i+1)/(NUM_ITERATIONS)*100)
            print(progress*"#"+(100-progress)*"-", i, end="\r")

        direction_list = ["ne", "nw", "se", "sw", "e", "w"]
        if search_type == 2:
            direction = random.choice(direction_list)
            counter = counter + 1
            #search_type = 1
        if counter == 6:
            search_type = 1
            counter = 0
        new_pos, rotation = move_cell(current_cell_id, direction, w) # Obtaining the new position and rotation
        new_cell_id = new_pos.get('id')
        new_cell_location = new_pos.get('location')
        obj = new_pos.get('event')

        if obj == "reward":
            # Successfully reached the goal
            score = d[current_cell_id][direction]
            d[current_cell_id][direction] = (score + 10)/2
        elif obj == "stationary":
            # never move - probably invalid move
            score = d[current_cell_id][direction]
            d[current_cell_id][direction] = (score-1)/2
        elif obj == "move":
            # moved
            score = d[current_cell_id][direction]
            d[current_cell_id][direction] = (score + max(list(d[new_cell_id].values())) * 0.8)/2

        current_cell_id = new_cell_id
        current_cell_location = new_cell_location

    return d
    # Save the Q table
    #pickle.dump(d, open("Q_table_World_"+occlusions+".sav","wb"))
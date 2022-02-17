# This file computes the best possible policy through given iterations and prior information of the world using RL and generates Qtables
# Created for Assignment-7 for BME 468
# Author: Nish Mohith Kurukuti
# Dependency: navigate_function.py

import random
from time import sleep
#from cellworld import *
from navigate_functions import *
from find_distance import *
import pickle

# create an instance of World class
current_cell_id = 0
goal_cell_id = 330
occlusions = "10_08" # World used for training
w = World.get_from_parameters_names('hexagonal','mice',occlusions)
w_cells_ids = w.cells.get('id')
current_cell_location = w.cells[current_cell_id].location
goal_cell_location = w.cells[goal_cell_id].location
rotation = 90

# create instance of display class
dis = Display(w, fig_size=(6,6), padding=0, cell_edge_color="lightgrey", background_color="black", habitat_edge_color="lightgrey", animated=True)

# create the barebones for the qtable
d = {
    i:{"ne":0, "e":0, "se":0, "sw":0, "w":0, "nw":0} 
    for i in range(len(w_cells_ids)) 
}
#print(d[current_cell_id].values())

distances = {"ne":0, "e":0, "se":0, "sw":0, "w":0, "nw":0}
#print(distances["ne"])
direction = "e" # Starting direction
search_type = 1 # 1 = Takes the same direction until it reaches a wall, then switches to 2. 2 = random direction
# Training starts here
NUM_ITERATIONS = 100000
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
        search_type = 1
    #print(direction)
    #for directions in direction_list:
        #print(directions)
        #intermediate_pos,intermediate_rotation = move_cell(current_cell_id,directions,w)
        #intermediate_new_cell_location = intermediate_pos.get('location')
        #distance_to_target = find_distance(intermediate_new_cell_location,goal_cell_location) # Compute distance between current position to target
        #distances[directions] = distance_to_target

    #direction_val = min(list(distances.values())) # find the direction that has the least distance to the target and use that as  your direction
    #direction_vals = list(distances.values())
    #direction_indx = list(distances.keys())
    #direction = direction_indx[direction_vals.index(direction_val)]
    #print(direction)
    new_pos, rotation = move_cell(current_cell_id, direction, w) # Obtaining the new position and rotation
    new_cell_id = new_pos.get('id')
    new_cell_location = new_pos.get('location')
    obj = new_pos.get('event')
    current_cell_distance = find_distance(current_cell_location,goal_cell_location)
    new_cell_distance = find_distance(new_cell_location,goal_cell_location)
    #print(search_type)
    if obj == "reward":
        # Successfully reached the goal
        score = d[current_cell_id][direction]
        d[current_cell_id][direction] = (score + 10)/2
    elif obj == "stationary":
        # never move - probably invalid move
        score = d[current_cell_id][direction]
        d[current_cell_id][direction] = (score-1)/2
        search_type = 2
    elif obj == "move":
        # moved
        score = d[current_cell_id][direction]
        
        #if new_cell_distance < current_cell_distance:
        #    d[current_cell_id][direction] = (score + 5 + max(list(d[new_cell_id].values())) * 0.8)/3
        #else:
        d[current_cell_id][direction] = (score + max(list(d[new_cell_id].values())) * 0.8)/2

    current_cell_id = new_cell_id
    current_cell_location = new_cell_location

# Save the Q table
pickle.dump(d, open("Q_table_World_"+occlusions+".sav","wb"))
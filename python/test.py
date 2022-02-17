# This file computes and displays the optimal policy that has been generated from the training code
# Created for Assignment-6 for BME 468
# Author: Nish Mohith Kurukuti
# Dependency: navigate_function.py

import pickle
from time import sleep
from navigate_functions import *

current_cell_id = 0
# create an instance of World class - Change this when testing different worlds
occlusions = "10_08"
w = World.get_from_parameters_names('hexagonal','mice',occlusions)
w_cells_ids = w.cells.get('id')
current_cell_location = w.cells[current_cell_id].location

#Importing the Q table 
d = pickle.load(open("Q_table_World_"+occlusions+".sav", "rb"))

# create instance of display class
dis = Display(w, fig_size=(6,6), padding=0, cell_edge_color="lightgrey", background_color="black", habitat_edge_color="lightgrey", animated=True)

# Function to select the best move
def select_move(possibilities):
    bestmove, bestscore = "w", -1 * 10**10
    for move, score in possibilities.items():
        if score > bestscore:
            bestscore = score
            bestmove = move
    return bestmove

while True:
    # add agent and circle to figure using method from Display class
    dis.agent(Step(agent_name="prey", location = current_cell_location, rotation = 90))
    dis.circle(current_cell_location, .03, 'red')
    dis.update()
    sleep(0.1)

    direction = select_move(d[current_cell_id])
    new_pos, rotation = move_cell(current_cell_id, direction, w)
    new_cell_id = new_pos.get('id')
    new_cell_location = new_pos.get('location')
    obj = new_pos.get('event')

    if obj == "reward":
        dis.agent(Step(agent_name="prey", location = current_cell_location, rotation = 90))
        dis.circle(current_cell_location, .03, 'red')
        dis.update()
        print("Reached the Goal!")
        break

    current_cell_id = new_cell_id
    current_cell_location = new_cell_location
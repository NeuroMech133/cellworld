# This file displays a given cell based on the inputted ID
# Created for Assignment-6 for BME 468
# Author: Nish Mohith Kurukuti
# Dependency: navigate_function.py

from navigate_functions import *

# create an instance of World class
occlusions = "14_08"
w = World.get_from_parameters_names('hexagonal','mice',occlusions)

# create instance of display class
d = Display(w, fig_size=(6,6), padding=0, cell_edge_color="lightgrey", background_color="black", habitat_edge_color="lightgrey", animated=True)

# agent starts at cell id = 0
current_cell_id = 0
start_location = w.cells[current_cell_id].location

# add agent and circle to figure using method from Display class
d.agent(Step(agent_name="prey", location = start_location, rotation = 90))
d.circle(start_location, .03, 'red')
d.update()

w_cells_ids = w.cells.get('id')

while True:
    # ask user for move
    new_id = int(input('id: '))

    # exit loop if user inputs stop
    if new_id == -1:
        break
    # catch invalid user inputs
    elif new_id not in w_cells_ids:
        print('invalid input')

    new_location = w.cells[new_id].location
    d.agent(Step(agent_name="prey", location = new_location, rotation = 90))
    d.circle(new_location, .03, 'red')
    d.update()
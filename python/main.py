"""
Test agent movement in world with manual user inputs

run in terminal: <python main.py>
"""
from navigate_functions import *

# create an instance of World class
occlusions = "10_05"
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


while True:
    # ask user for move
    direction = input('direction: ')

    # exit loop if user inputs stop
    if direction == 'stop':
        break
    # catch invalid user inputs
    elif direction not in direction_list:
        print('invalid input')

    else:
        # get new agent position based on user input
        new_pos, rotation = move_cell(current_cell_id, direction, w)

        # add new move to plot
        d.agent(Step(agent_name="prey", location = new_pos['location'], rotation = rotation))
        d.circle(new_pos['location'], .03, 'red')
        d.update()

        # update current position id
#        new_cell_id = new_pos['id']
#        current_cell_id = new_cell_id


"""
Functions used across all files
"""

from cellworld import *
import random
from time import sleep
import pickle


direction_list = ['ne', 'e', 'se','sw', 'w','nw']


def get_new_cell(cell_id, delta, w):
    '''
    DESCRIPTION
        (1) Computes the new coordinate position of the agent after
        making the specified move. (2) If the new coordinate position is not on the
        map or is an occluded cell the position of the agent will not change.
    PARAMETERS
        cell_id (int): id of current location in cell world
        delta (dict): horizontal and vertical increment to get to next location
                      ex. {'x': -1, 'y': -1}
        w (World): instance of world class
    RETURNS
        (dict): information about the agent's action and position
    '''
    reward_cell_id = 330

    # find new coordinates of agent specified move
    current_coord = w.cells[cell_id].coordinates
    new_coord = Coordinates(x = current_coord.x + delta['x'], y = current_coord.y + delta['y'])

    # check if the new cell exists in the world and is not occlude
    if new_coord in w.cells.get('coordinates'):
        # get new_coordinate cell id from list of coordinates in world
        new_cell_id = w.cells.get('coordinates').index(new_coord)

        # (1) agent moves to new cell (event = "move")]
        event = "move"
        # (2) agent moves to goal cell and is rewarded (event = "reward")
        if new_cell_id == reward_cell_id:
            event = "reward"
        # (3) cell is occluded the agent's position remains the same (event = "stationary")
        elif w.cells[new_cell_id].occluded == True:
            new_coord = current_coord
            new_cell_id = cell_id
            event = "stationary"

    # cell it does not exist in world so agent's position remains the same (event = "stationary")
    else:
        new_coord = current_coord
        new_cell_id = cell_id
        event = "stationary"
    # print agent action
    #print(event)

    return {'id': new_cell_id, 'coordinate': new_coord,'location': w.cells[new_cell_id].location, 'event': event}


def move_cell(cell_id, move, w):
    '''
    DESCRIPTION
        Given the chosen move (our agent can move in 6 directions) this function passes the change in x and y
        to the get_new_cell function. Based on the conditonals in get_new_cell function it returns the new position
        and orientation of the agent.
    PARAMETERS
        cell_id (int): id of current location in cell world
        move (str):
        w (World): instance of world class
    RETURNS
        cell (dict): information about the agent's action and position
        rotation (int): orientation of agent

    '''

    # create dictionary to store orientation of agent and delta x&y
    direction_dict = {'ne':[{'x': 1, 'y': 1}, 30],'e':[{'x': 2, 'y': 0}, 90],
                      'se':[{'x': 1, 'y': -1}, 150],'sw':[{'x': -1, 'y': -1}, 210],
                      'w':[{'x': -2, 'y': 0}, 270],'nw':[{'x': -1, 'y': 1}, 330]}

    delta_xy = direction_dict[move][0]
    rotation = direction_dict[move][1]

    # aquire new position of agent based on specified move
    cell = get_new_cell(cell_id, delta_xy, w)
    #print(rotation)

    return cell, rotation



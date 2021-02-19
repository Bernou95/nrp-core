#!/usr/bin/env python

""" Main file for human arm simulation in opensim. """
import numpy as np
#import farms_pylog as pylog
from osim_rl import OsimModel
import numpy as np
import time
import os
import matplotlib.pyplot as plt
#pylog.set_level('debug')

def main():
    """ Main file. """

    max_steps = 5000

    #: Controller outputs
    flexor = 0.5
    extensor = 0.0

    #: osim model
    model = OsimModel('./arm26_ground_offset.osim', visualize=True)

    #: Initialize
    model.reset()
    model.reset_manager()

    #: List model components
    model.list_elements()

    #: Integrate
    for j in range(0, max_steps):
        #: Step controller here if any
        #############################
        #: Actuate the model from control outputs
        model.actuate(
            [extensor, 0.0, 0.0, flexor, 0.0, 0.0]
        )
        #: Integration musculoskeletal system
        model.integrate()
        #: Results from the integration
        res = model.compute_state_desc()
    model.model.setUseVisualizer(False)

if __name__ == '__main__':
    main()

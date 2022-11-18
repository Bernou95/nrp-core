from lava.magma.core.model.py.type import LavaPyType
from lava.magma.core.model.py.ports import PyInPort, PyOutPort
from lava.magma.core.resources import CPU
# Import parent classes for ProcessModels for Hierarchical Processes.
from lava.magma.core.model.py.model import PyLoihiProcessModel

# Import execution protocol.
from lava.magma.core.sync.protocols.loihi_protocol import LoihiProtocol

# Import decorators.
from lava.magma.core.decorator import implements, tag, requires
# Import Process level primitives.
from lava.magma.core.process.process import AbstractProcess
from lava.magma.core.process.variable import Var
from lava.magma.core.process.ports.ports import InPort, OutPort

from scipy.special import erf
import numpy as np


class EINetwork(AbstractProcess):
    """Network of recurrently connected neurons.
    """
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        shape_exc = kwargs.pop("shape_exc", (1,))
        bias_exc = kwargs.pop("bias_exc", 1)
        shape_inh = kwargs.pop("shape_inh", (1,))
        bias_inh = kwargs.pop("bias_inh", 1)
        # Factor controlling strength of inhibitory synapses relative to excitatory synapses.
        self.g_factor = kwargs.pop("g_factor", 4)
        # Factor controlling response properties of network.
        # Larger q_factor implies longer lasting effect of provided input.
        self.q_factor = kwargs.pop("q_factor", 1)
        weights = kwargs.pop("weights")

        full_shape = shape_exc + shape_inh

        self.state = Var(shape=(full_shape,), init=0)
        # Variable for possible alternative state.
        self.state_alt = Var(shape=(full_shape,), init=0)
        # Biases provided to neurons.
        self.bias_exc = Var(shape=(shape_exc,), init=bias_exc)
        self.bias_inh = Var(shape=(shape_inh,), init=bias_inh)
        self.weights = Var(shape=(full_shape, full_shape), init=weights)

        # Ports for receiving input or sending output.
        self.inport = InPort(shape=(full_shape,))
        self.outport = OutPort(shape=(full_shape,))

@implements(proc=EINetwork, protocol=LoihiProtocol)
@tag('rate_neurons') # Tag allows for easy selection of ProcessModel in case multiple are defined.
@requires(CPU)
class RateEINetworkModel(PyLoihiProcessModel):

    outport: PyOutPort = LavaPyType(PyOutPort.VEC_DENSE, float)
    inport: PyInPort = LavaPyType(PyInPort.VEC_DENSE, float)
    state : np.ndarray = LavaPyType(np.ndarray, float)
    state_alt : np.ndarray = LavaPyType(np.ndarray, float)
    bias_exc : np.ndarray = LavaPyType(np.ndarray, float)
    bias_inh : np.ndarray = LavaPyType(np.ndarray, float)
    weights : np.ndarray = LavaPyType(np.ndarray, float)

    def __init__(self, proc_params):
        super().__init__(proc_params=proc_params)

        self.dr_exc = proc_params.get('dr_exc')
        self.dr_inh = proc_params.get('dr_inh')

        self.shape_exc = proc_params.get('shape_exc')
        self.shape_inh = proc_params.get('shape_inh')

        self.proc_params = proc_params

        self.got_decay = False
        self.got_bias = False
        self.weights_scaled = False

    def get_decay(self):
        '''Construct decay factor.
        '''
        dr_full = np.array([self.dr_exc] * self.shape_exc + [self.dr_inh] * self.shape_inh)
        self.decay = 1 - dr_full

        self.got_decay= True

    def get_bias(self):
        '''Construce biases.
        '''
        self.bias_full = np.hstack([self.bias_exc, self.bias_inh])
        self.got_bias = False

    def scale_weights(self):
        '''Scale the weights with integration time step.
        '''

        self.weights[:, self.shape_exc:] *= self.dr_exc
        self.weights[:, :self.shape_exc] *= self.dr_inh
        self.proc_params.overwrite('weights', self.weights)

        self.weights_scaled = True

    def state_update(self, state):
        """Update network state according to:
            r[i + 1] = (1 - dr)r[i] + Wr[i]*r*dr + bias*dr
        """
        state_new = self.decay * state # Decay the state.
        state_new += self.bias_full # Add the bias.
        state_new += self.weights @ erf(state) # Add the recurrent input.
        return state_new

    def run_spk(self):
        """The run function that performs the actual computation during
        execution orchestrated by a PyLoihiProcessModel using the
        LoihiProtocol.
        """

        if not self.got_decay:
            self.get_decay()

        if not self.got_bias:
            self.get_bias()

        if not self.weights_scaled:
            self.scale_weights()

        a_in = self.inport.recv()
        self.state = self.state_update(self.state) + a_in
        self.outport.send(self.state)

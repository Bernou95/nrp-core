from NRPEventLoopEngine import *


@inputDummy('input1', 'idummy1', 10)
@inputDummy('input2', 'idummy2', 20)
@outputDummy('output', 'odummy1')
@FunctionalNode(name='function', outputs=["output", "output2"])
def f(input1, input2):
    print('function called! with {} {}'.format(input1, input2))
    return [input1, input2]


@inputFN('input1', '/function/output2')
@inputDummy('input2', 'idummy1', 88) # idummy1 already exists, so no new node is created
@outputDummy('output', 'odummy1')
@outputDummy('output2', 'odummy2')
@FunctionalNode(name='function2', outputs=["output", "output2"])
def f2(input1, input2):
    print('function 2 called! with {} {}'.format(input1, input2))
    return [input2, input1]